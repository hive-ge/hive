import * as hc from "@candlefw/hydrocarbon";
import runner from "./compiler_runner.js";
import fs, { statSync } from "fs";
import path from "path";
import { inspect } from "util";

const fsp = fs.promises;
let parser_data = null, BUILD_PARSER_SENTINEL_PROMISE = null;
const grammar_file_path = "./source/tools/cpp.hcg", compile_data_file_path = "./source/tools/cpp.data.js";
let prop_function = null, start_function = null, end_function = null;


const env = {
    functions: {
        mergeChain(chain) {

            const array = new Array;

            let curr = chain, f = curr.f;
            if (curr) {
                while (f && f !== curr) {
                    array.push(f.v || f.id);
                    f = f.n;
                }

                array.push(curr.v);
            }

            return array;
        },
        defaultError: (tk, env, output, lex, prv_lex, ss, lu) => {

            //Comments
            if (lex.tx == "//" || lex.tx == "/*") {
                if (lex.tx == "//") {
                    while (!lex.END && lex.ty !== lex.types.nl)
                        lex.next();

                    if (lex.END) {
                        lex.tl = 0;
                        return 0;//lu({ tx: ";" });
                    }
                } else
                    if (lex.tx == "/*") {
                        //@ts-ignore
                        while (!lex.END && (lex.tx !== "*" || lex.pk.tx !== "/"))
                            lex.next();
                        lex.next(); //"*"
                    }

                return lu(lex.next());
            }
        },

        options: {
            integrate: false
        }
    }
};


// Grammar Parser

// This utility is designed to watch and update prop hpp files with necessary code to both
// link JS scripting functions into hive property structs and add serialization functionality
// to those same property structs.


// Load grammar and build parser from the data.

async function buildParserPromiseFunction() {
    try {
        const
            data = await fsp.readFile(grammar_file_path, { encoding: "utf8" }),

            grammar = await hc.grammarParser(data, ""),

            states = await runner(grammar, env, "", "", true, true),

            script_string = hc.LRParserCompiler(states, grammar, env);

        //compile states into a script
        parser_data = ((Function("return " + script_string))());

        //save new parser file
        fsp.writeFile(compile_data_file_path, "return " + script_string);

    } catch (e) {

        console.log("\nGrammar Build Error ----------------------------------------------------------\n");

        console.error(e);

        BUILD_PARSER_SENTINEL_PROMISE = null;

        throw (e);
    }

    BUILD_PARSER_SENTINEL_PROMISE = null;

    return parser_data;
}


function buildParser(grammar_string) {

    if (!BUILD_PARSER_SENTINEL_PROMISE)
        BUILD_PARSER_SENTINEL_PROMISE = buildParserPromiseFunction(grammar_string);

    return BUILD_PARSER_SENTINEL_PROMISE;
}

async function loadAndPossiblyParseGrammarFile() {
    //Get grammar file data and compare agains the saved file, if the grammar file's timestamp
    //is newer the the compiled file, re-compile, otherwise skip this step.

    try {

        const grammar_file_stat = await fsp.stat(grammar_file_path),
            compile_data_file_stat = await fsp.stat(compile_data_file_path);

        if (compile_data_file_stat && grammar_file_stat.mtime > compile_data_file_stat.mtime) {

            await buildParser();

        } else if (!parser_data) {

            parser_data = (new Function(await fsp.readFile(compile_data_file_path, { encoding: "utf8" })))();
        }
    } catch (e) {

        await buildParser();
    }
}

var WATCHED_FILE_GUARD = false, REPEAT_GUARD = false;


export async function start() {

    if (REPEAT_GUARD) return;

    REPEAT_GUARD = true;

    await loadAndPossiblyParseGrammarFile();

    loadAndCreatePropHandlers();

    WATCHED_FILE_GUARD = false;

    REPEAT_GUARD = false;
}


async function loadAndCreatePropHandlers(dir = "./source/library/primitive") {

    if (start_function)
        start_function();

    try {
        for (const name of await fsp.readdir(dir)) {
            const
                obj_path = path.resolve(dir, name),
                ext = path.extname(obj_path),
                stats = fsp.stat(obj_path);



            if ((await stats).isDirectory()) {
                loadAndCreatePropHandlers(obj_path);
            } else {
                if (ext == ".hpp" || ext == ".h") {
                    parseHPPHeader(obj_path);
                }
            }
        }
        if (end_function)
            end_function();
    } catch (e) {
        console.log(e);
    }


}

async function parseHPPHeader(obj_path) {

    if (parser_data) {
        try {
            const
                hpp = await fsp.readFile(obj_path, { encoding: "utf8" }),
                d = hc.lrParse(hpp, parser_data, env);

            if (d.value && d.value.length > 1) {

                for (const a of d.value) {

                    if (a && a.t == "HivePropStruct")
                        compileStruct(a.v);

                    //Keep an eye on this one.
                    if (!WATCHED_FILE_GUARD)
                        fs.watch(obj_path, start);
                }

            } else if (d.error)
                console.error(obj_path, "\n", d.error);

        } catch (e) {
            console.error(e);
        }
    }

}
// Watch Files

// Watch the grammar file
fs.watch("./source/tools/cpp.hcg", async data => {
    try {
        console.clear();
        start();
    } catch (e) {
        console.error(e);
    }
});

const type = {
    ACCESS: "access",
    PROPERTY: "member property",
    FUNCTION: "member function",
    DESTRUCTOR: "destructor function",
    CONSTRUCTOR: "constructor function"
};

function compileStruct(d) {


    console.log(d);

    console.log("--start--");
    // filter through props and isolate functions and properties
    const name = d.name.v, properties = [], functions = [];

    let access_type = "public";

    for (const prop of d.props) {
        switch (prop.t) {
            case type.ACCESS:
                access_type = prop.val.v;
                break;
            case type.PROPERTY:
                prop.access_type = access_type;
                properties.push(prop);
                break;
            case type.FUNCTION:
                prop.access_type = access_type;
                functions.push(prop);
                break;
            case type.DESTRUCTOR:
                break;
            case type.CONSTRUCTOR:
                break;
        }
    }

    prop_function({ name, properties, functions });
}
export function onPropParseEnd(fn) {
    end_function = fn;
}
export function onPropParseStart(fn) {
    start_function = fn;
}
export function onHandleProp(fn) {
    prop_function = fn;
}
//Start the whole processes
