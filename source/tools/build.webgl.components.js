#!/usr/bin/env node
import * as utils from "./util.functions.js";
import fs from "fs";
const fsp = fs.promises;

const prop_interfaces = [], prop_names = new Map;

utils.onPropParseStart(() => { prop_interfaces.length = 0; });

utils.onHandleProp(struct => {
    console.dir({ struct }, { depth: 8 });
    struct.h_ind = 0;
    prop_names.set(struct.name, struct);
});

utils.onPropParseEnd(async () => {

    console.log([...prop_names.keys()]);

    [...prop_names.entries()].map(([a, struct]) => {
        for (const prop of struct.heritage) {
            if (prop_names.has(prop)) {
                prop_names.get(prop).h_ind++;
            }
        }
        return struct;
    }).sort((a, b) => a.h_ind > b.h_ind ? -1 : 1).forEach(createJSInterface);

    const out_data =

        `
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    DO NOT MODIFY - This file is automatically created by the build:props script.

    COPYRIGHT 2020 Anthony C Weathersby
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

#pragma once
#include <thread>
#include <vector>
#include <hive.h>
#include <emscripten/bind.h>


namespace hive{
    namespace js{

        using namespace hive::all;

        EMSCRIPTEN_BINDINGS(hive_property_bindings) {
            ${prop_interfaces.flatMap(p => `${p.string};`).join("\n\n")} 
        }
    }
}`;

    prop_names.clear();

    try {
        await fsp.writeFile("./source/webassembly/interfaces.hpp", out_data, { encoding: "utf8" });
    } catch (e) {
        console.error(e);
        process.exit(1);
    }

    process.exit(0);
});

function createJSInterface(struct) {

    const name = getPropName(struct.name);

    let v = `emscripten::class_<${name}${struct.heritage.length > 0 ? `, emscripten::base<${struct.heritage[0]}>` : ""}>("${name}")`;

    // Every prop has an instantiation and wrap method.
    // The instantiation method creates a new class instance of 
    // the property, serving as a static factory for the object.

    // The class wrap method serves as an initilizer of getters, setters, and methods
    // of a hive property. This functions is called when either a new instance of 
    // a property is created in JS land, or when an object getter is accessed for 
    // a particular prop that returns an instance of another prop. 

    // Properties defined as private / protected should not show up in the interfaces
    // of script objects.

    // Functions and getter/setters need to be defined in the class instantiation/wrapping method. 
    // 

    const methods = [], property_descriptors = [];

    for (const prop of struct.properties) {
        if (prop.access_type !== "public") continue;
        if (prop.specifiers.includes("static")) continue;
    }



    for (const method of struct.functions) {

        const member_name = getPropName(method.name);

        if (member_name == "construct") {

            v += `\n\t.constructor(&${name}::construct, emscripten::allow_raw_pointers())`;

            break;
        };

    }

    for (const method of struct.functions) {

        //Ignore private &  protected methods and class methods
        if (method.template || method.operator || method.access_type !== "public" || method.type.includes("static")) continue;

        const member_name = getPropName(method.name);

        if (member_name == "construct" || member_name == "destruct") continue;

        const raw_pointers = method.params
            && method.params.filter(p => p.name.type && p.name.type == "ptr").length > 0
            || (method.name.type);

        v += `\n\t.function("${member_name}", &${name}::${member_name}${raw_pointers ? ", emscripten::allow_raw_pointers()" : ""})`;
    }

    prop_interfaces.push({ string: v });
}

function getPropName(name_obj) {
    if (typeof name_obj == "object") {
        if (name_obj.type)
            return name_obj.name.v;
        return name_obj.v;
    }
    return name_obj;
}

utils.start();