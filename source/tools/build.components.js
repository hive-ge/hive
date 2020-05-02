#!/usr/bin/env node
import * as utils from "./util.functions.js";
import fs from "fs";
const fsp = fs.promises;

const templates = {
    get_callback_data(arg_size = 0) {
        return `  void * data;

        napi_value this_arg;

        std::vector<napi_value> args(${arg_size});

        size_t arg_count = ${arg_size};

        napi_get_cb_info(env, info, &arg_count, args.data(), &this_arg, nullptr);

        napi_unwrap(env, this_arg, &data);
    `;
    }
};

const prop_interfaces = [], prop_names = new Map;

utils.onPropParseStart(() => { prop_interfaces.length = 0; });

utils.onHandleProp(struct => {
    console.log(struct);
    struct.h_ind = 0;

    prop_names.set(struct.name, struct);
});

utils.onPropParseEnd(async () => {

    [...prop_names.entries()].map(([a, struct]) => {
        for (const prop of struct.heritage) {
            if (prop_names.has(prop)) {
                prop_names.get(prop).h_ind++;
            }
        }
        return struct;
    }).sort((a, b) => a.h_ind > b.h_ind ? -1 : 1).forEach(createJSInterface);

    const out_data =

        `/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    DO NOT MODIFY - This file is automatically created by the build:props script.

    COPYRIGHT 2020 Anthony C Weathersby
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
 #pragma once
#include <thread>
#include <vector>

#include <nodejs/src/node_api.h>
#include <hive.h>

using namespace hive::all;

namespace hive{
    namespace js{

        template<class T>
        T * getPropPointer(napi_env env, napi_value value){
            void * ptr;

            napi_status status = napi_unwrap(env, value, &ptr);

            if(status != napi_ok)
                return nullptr;
            
            return static_cast<T *>(ptr);
        }

        napi_value JSError(napi_env env, const char * message){
            napi_throw_error(env, nullptr, message);
            return nullptr;
        }

        void napi_inherits(napi_env env, napi_value ctor, napi_value super_ctor) {
            napi_value global, global_object, set_proto, ctor_proto_prop, super_ctor_proto_prop;
            napi_value args[2];
        
            napi_get_global(env, &global);
            napi_get_named_property(env, global, "Object", &global_object);
            napi_get_named_property(env, global_object, "setPrototypeOf", &set_proto);
            napi_get_named_property(env, ctor, "prototype", &ctor_proto_prop);
            napi_get_named_property(env, super_ctor, "prototype", &super_ctor_proto_prop);
        
            args[0] = ctor_proto_prop;
            args[1] = super_ctor_proto_prop;
            napi_call_function(env, global, set_proto, 2, args, NULL);
        
            args[0] = ctor;
            args[1] = super_ctor;
            napi_call_function(env, global, set_proto, 2, args, NULL);
        }

        ${prop_interfaces.flatMap(p => `napi_ref ${p.name}_reference;`).join("\n")} 

        //Forward Declarations

        ${prop_interfaces.flatMap(p => p.forward_declarations).join("\n")}

        ${prop_interfaces.flatMap(p => p.functions).join("\n")}
        
        void RegisterInterfaces(napi_env env, napi_value registry){
            ${prop_interfaces.map(p => `${p.name}_Register(env, registry);\n`).join("")}
        }
    }
}`;

    prop_names.clear();

    try {
        await fsp.writeFile("./source/frontend/interfaces.hpp", out_data, { encoding: "utf8" });
    } catch (e) {
        console.error(e);
        process.exit(1);
    }

    process.exit(0);
});


const arg_names = "ABCDEFGHIJKLMNOP";

function createJSInterface(struct) {

    const name = struct.name;

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

        const
            type = getJSToCValue(prop);

        if (type.void) continue;

        const

            hive_prop_instance_name = getPropName(prop.name),

            setter_name = `${name}_${hive_prop_instance_name}_setter`,

            getter_name = `${name}_${hive_prop_instance_name}_getter`,

            getter_function = `\nnapi_value ${getter_name}(napi_env env, napi_callback_info info){
                napi_value value = nullptr;
                napi_status status;

                ${templates.get_callback_data(1)}

                ${name} * prop = (${name}*)data;

                ${type.render.getter(type.type, "value", "prop", hive_prop_instance_name)}

                return value;\n}`,

            setter_function = `\nnapi_value ${setter_name}(napi_env env, napi_callback_info info){
                napi_value value= nullptr;
                napi_status status;

                ${templates.get_callback_data(1)}

                ${name} * prop = (${name}*)data;

                ${type.render.setter(type.type, "args[0]", "prop", hive_prop_instance_name)}

                return value;\n}`,

            prop_assignment = `{
                "${hive_prop_instance_name}",
                NULL,
                NULL,
                ${getter_name},
                ${setter_name},
                NULL,
                (napi_property_attributes)(napi_writable | napi_enumerable),
                NULL
            }`;

        methods.push(getter_function, setter_function);
        property_descriptors.push(prop_assignment);
    }

    for (const method of struct.functions) {

        //Ignore overloading for now.
        if (method.operator || method.access_type !== "public" || method.type.includes("static")) continue;

        const
            member_name = getPropName(method.name);

        if (member_name == "construct" || member_name == "destruct") continue;

        const return_value = getJSToCValue(method),

            method_name = `${name}_${member_name}_method`,

            arg_actions = method.params ? method.params.map((v, i) => getJSToCValue(v)) : [];

        //Remove any method with incomplete argument access
        if (arg_actions.filter(v => v.void).length > 0) continue;

        const

            return_val = "null",

            js_method_function = `
        napi_value ${method_name}(napi_env env, napi_callback_info info){

            napi_value return_val = nullptr;
            napi_status status;

            // Create JS object.
            ${templates.get_callback_data(method.params ? method.params.length : 0)}

            ${arg_actions.map((v, i) => v.render.method_arg(v.type, arg_names[i], `args[${i}]`)).join("\n")}

            ${name} * prop = (${name}*)data;

            ${return_value.render.method_return(return_value.type, "return_val", `prop->${member_name}(${arg_actions.map((a, i) => arg_names[i]).join(",")});`)}

            return return_val;
        }`,

            prop_assignment = `{
            "${member_name}",
            NULL,
            ${method_name},
            NULL,
            NULL,
            NULL,
            (napi_property_attributes)(napi_enumerable),
            NULL
        }`;

        methods.push(js_method_function);
        property_descriptors.push(prop_assignment);
    }


    const props_definition = `const napi_property_descriptor ${name}_props[] = {${property_descriptors}};`;

    const destructor_function = `
    void ${name}_Destructor(napi_env env, void * finalize_data, void * finalize_hint){
        
        ${name} * prop = (${name} *)finalize_data;
        
        std::cout << "Deleting instance of ${name} " << (long long)prop << std::endl;
        
        delete prop;
    }`;

    const forward_declare_instance_wrap = `napi_value ${name}_wrapped_instance(void * hive_prop, napi_env env, napi_callback_info cb_info, bool APPLY_DESTRUCTOR);`;
    const wrap_function = `${forward_declare_instance_wrap.trim().slice(0, -1)}{
        
        napi_status status;
        napi_value constructor, object, pointer_arg;

        status = napi_get_reference_value(env, ${name}_reference, &constructor);
        
        if(status != napi_ok)
            std::cout << "Unable to dereference ${name}_reference" << std::endl;

        status = napi_create_external( env, hive_prop, nullptr, nullptr, &pointer_arg);

        if(status != napi_ok)
            std::cout << "Unable to create a raw pointer reference" << std::endl;
        
        status = napi_new_instance(env, constructor, 1, &pointer_arg, &object);

        if(status != napi_ok)
            std::cout << "Unable to create JS instance of ${name}" << std::endl; 
        
        return object;
    }`;

    const constructor_function = `
    napi_value ${name}_Constructor(napi_env env, napi_callback_info info){
        
        ${name} * hive_prop;

        // Create JS object.
        napi_status status;
        size_t arg_count = 1;
        napi_value this_arg, raw_arg;
        napi_valuetype arg_type;
        bool WRAP_EXISTING_POINTER = false;

        status = napi_get_cb_info(env, info, &arg_count, &raw_arg, &this_arg, nullptr);

        if(status != napi_ok)
            std::cout << "Unable to get callback info for ${name}_Constructor" << std::endl;
        
        if(arg_count == 1){

            status = napi_typeof(env, raw_arg, &arg_type);

            if(status != napi_ok)
                std::cout << "Unable to get callback info for ${name}_Constructor" << std::endl;
            else if(arg_type == napi_external){
                void * ptr;

                status = napi_get_value_external(env, raw_arg, &ptr);

                if(status == napi_ok){   

                    WRAP_EXISTING_POINTER = true;

                    hive_prop = static_cast<${name} *>(ptr);
                    
                    std::cout << "wrapping instance of ${name} " << (long long)hive_prop << std::endl;
                }
            }
        }
        
        if(!WRAP_EXISTING_POINTER){
            hive_prop = ${name}::construct();

            std::cout << "creating instance of ${name} " << (long long)hive_prop << std::endl;
        }

        if(WRAP_EXISTING_POINTER)
            status = napi_wrap(env, this_arg, hive_prop, nullptr, nullptr, nullptr);
        else
            status = napi_wrap(env, this_arg, hive_prop, ${name}_Destructor, nullptr, nullptr);

        if(status != napi_ok)
            std::cout << "Unable to wrap info for ${name}_Constructor" << std::endl;


        return this_arg;
    }`;

    const register_function = `
        void ${name}_Register(napi_env env, napi_value property_registry){
            
            napi_status status;
            
            napi_value result;

            status = napi_define_class(env, "${name}", NAPI_AUTO_LENGTH, &${name}_Constructor,
                nullptr, ${property_descriptors.length}, ${name}_props, &result);

            status = napi_set_named_property(env, property_registry, "${name}", result);

            if (status != napi_ok) {
                std::cout << "FAILED TO CREATE  CLASS CONSTRUCTOR FOR ${name}" << std::endl;
            } else {
                std::cout << "CREATED CLASS CONSTRUCTOR ${name}" << std::endl;
            }

            status = napi_create_reference(env, result, 1, &${name}_reference);


        ${name == "SpriteProp"
            ? `
    napi_value super_constr, constr;

    status = napi_get_reference_value(env, ${"Prop"}_reference, &super_constr);

    if (status != napi_ok) {JSError(env, "Unable to dereference ${"Prop"}_reference"); return; };

    status = napi_get_reference_value(env, ${name}_reference, &constr);

    if (status != napi_ok)  {JSError(env, "Unable to dereference ${name}_reference"); return; };

    napi_inherits(env, constr, super_constr);
    
    `
            : ""}

            if (status != napi_ok) {
                std::cout << "FAILED TO CREATE REFERENCE FOR ${name}" << std::endl;
            } else {
                std::cout << "CREATED CLASS CONSTRUCTOR ${name}" << std::endl;
            }
        }`;

    prop_interfaces.push(
        {
            name,
            forward_declarations: [
                forward_declare_instance_wrap
            ],
            functions: [
                ...methods,
                props_definition,
                destructor_function,
                wrap_function,
                constructor_function,
                register_function
            ]
        }
    );
}

function getPropName(name_obj) {
    if (name_obj.type == "ptr")
        return name_obj.name.v;
    return name_obj.v;
}

const conversion_objects = {
    prop: {
        getter(hive_struct_name = "", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `${hive_struct_name} * v = ${hive_prop_instance_name} -> ${hive_prop_instance_member};

            if (v == nullptr) {
                status = napi_get_null(env, & ${ napi_value_name});
            } else {

                //Wrap the pointer within an accessor object. 
                ${ napi_value_name} = ${hive_struct_name}_wrapped_instance(v, env, info, false);
            }`;
        },

        setter(hive_struct_name = "", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `/** 
             * Need to test if the incoming value is an object, is a wrapped object, and is a property with a type that
             * matches the struct member type.
             */
            bool IS_INSTANCE = false;
            napi_valuetype value_type;

            status = napi_typeof(env, ${ napi_value_name}, &value_type);

            if (status != napi_ok) {
                //TODO - Throw JS error
            } else if (value_type == napi_null) {
                ${ hive_prop_instance_name} -> ${hive_prop_instance_member} = nullptr;
            } else {
                napi_value construct;

                status = napi_get_reference_value(env, ${ hive_struct_name}_reference, & construct);

                if (status != napi_ok) return JSError(env, "Unable to dereference ${hive_struct_name}_reference");

                status = napi_instanceof(env, ${ napi_value_name}, construct, & IS_INSTANCE);

                if (status == napi_ok && IS_INSTANCE) {

                    //Unbox the JS value;
                    ${ hive_struct_name} * val = getPropPointer < ${hive_struct_name}> (env, ${napi_value_name});

                    ${ hive_prop_instance_name} -> ${hive_prop_instance_member} = val;

                    std:: cout << "Assigning pointer val ${hive_prop_instance_member}" << (long long) val << std:: endl;
                } else
                    std:: cout << "Unable to to assign pointer ${hive_struct_name}" << (long long) ${napi_value_name} << std:: endl;
            } `;
        },

        method_arg(hive_struct_name = "", arg_name = "", napi_value_name) {
            return `bool IS_INSTANCE = false;
            napi_value construct;
                status = napi_get_reference_value(env, ${hive_struct_name}_reference, & construct);

                if (status != napi_ok) return JSError(env, "Unable to dereference ${hive_struct_name}_reference");

                status = napi_instanceof(env, ${napi_value_name}, construct, & IS_INSTANCE);

                if (status != napi_ok || !IS_INSTANCE) return JSError(env, "Expected instance of ${hive_struct_name} for argument ${arg_name}.");

                ${hive_struct_name} * ${arg_name} = getPropPointer < ${hive_struct_name}> (env, ${napi_value_name}); 
            `;
        },
        method_return(hive_struct_name = "", napi_return_value = "", method_call = "") {
            return `${hive_struct_name} * v = ${method_call};

                if (v == nullptr) { status = napi_get_null(env, & ${napi_return_value}); } else {
                    //Wrap the pointer within an accessor object. 
                    ${napi_return_value} = ${hive_struct_name}_wrapped_instance(v, env, info, false);
                }

                if(status != napi_ok) return JSError(env, "Could not write value as an instance of ${hive_struct_name}");`;
        }
    },

    string: {
        getter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_create_string_utf8(env, ${hive_prop_instance_name} -> ${hive_prop_instance_member}.c_str(), NAPI_AUTO_LENGTH, &${napi_value_name});
                if(status != napi_ok) return JSError(env, "Could not read value as a string");`;
        },
        setter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `
                status = napi_get_value_string_utf8(env, ${napi_value_name}, nullptr, 0, &${string_val_name});
                if (status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");

                std::vector<char> temp_${arg_name}(string_length + 1);

                status = napi_get_value_string_utf8(env, ${napi_value_name}, temp_${arg_name}.data(), temp_${arg_name}.size(), &${string_val_name});
                if (status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");

                std::string s(temp_${arg_name}.begin(), temp_${arg_name}.end());

                ${hive_prop_instance_name} -> ${hive_prop_instance_member} = s;

                if(status != napi_ok) return JSError(env, "Could not read value as a string");`;
        },
        method_arg(hive_struct_name = "", arg_name = "", napi_value_name) {
            const string_val_name = "string_length_" + arg_name;
            return `napi_valuetype number_val${arg_name};
                
                status = napi_typeof(env, ${ napi_value_name}, &number_val${arg_name});
                
                if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
                if(number_val${arg_name} != napi_string) return JSError(env, "Argument ${arg_name} is not a number type.");

                size_t ${string_val_name} = 0;
                
                status = napi_get_value_string_utf8(env, ${napi_value_name}, nullptr, 0, &${string_val_name});
                if (status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");

                std::vector<char> temp_${arg_name}(${string_val_name} + 1);

                status = napi_get_value_string_utf8(env, ${napi_value_name}, temp_${arg_name}.data(), temp_${arg_name}.size(), &${string_val_name});
                if (status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");

                std::string ${arg_name}(temp_${arg_name}.begin(), temp_${arg_name}.end());`;
        },
        method_return(hive_struct_name = "", napi_return_value = "", method_call = "") {
            return `string val = ${method_call}
                status = napi_create_string_utf8(env, val.c_str(), NAPI_AUTO_LENGTH, &${napi_return_value});
                if(status != napi_ok) return JSError(env, "Could not write value as a string");`;
        }
    },

    float: {
        getter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_create_double(env, static_cast<double>(${hive_prop_instance_name} -> ${hive_prop_instance_member}), &${napi_value_name});
            if(status != napi_ok) return JSError(env, "Could not read value as a float");`;
        },
        setter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `
                double temp;
                status = napi_get_value_double(env, ${napi_value_name}, &temp);
                ${hive_prop_instance_name} -> ${hive_prop_instance_member} = statice_cast<float>(temp);
                if(status != napi_ok) return JSError(env, "Could not read value as a float");`;
        },
        method_arg(hive_struct_name = "", arg_name = "", napi_value_name) {
            return `napi_valuetype number_val${arg_name};
                
                status = napi_typeof(env, ${ napi_value_name}, &number_val${arg_name});
                
                if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
                if(number_val${arg_name} != napi_number) return JSError(env, "Argument ${arg_name} is not a number type.");

                double temp_${arg_name};
                float ${arg_name};
    
                status = napi_get_value_double(env, ${napi_value_name}, &temp_${arg_name});

                if (status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
                
                ${arg_name} = static_cast<float>(temp_${arg_name});`;
        },
        method_return(hive_struct_name = "", napi_return_value = "", method_call = "") {
            return `float val = ${method_call}
                status = napi_create_double(env, static_cast<double>(val), &${napi_return_value});
                if(status != napi_ok) return JSError(env, "Could not write value as a float");`;
        }
    },

    double: {
        getter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_create_double(env, ${hive_prop_instance_name} -> ${hive_prop_instance_member}, & ${napi_value_name});
                    if(status != napi_ok) return JSError(env, "Could not read value as a double");`;
        },
        setter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_get_value_double(env, ${napi_value_name}, &(${hive_prop_instance_name} -> ${hive_prop_instance_member}));
                    if(status != napi_ok) return JSError(env, "Could not read value as a double");`;
        },
        method_arg(hive_struct_name = "", arg_name = "", napi_value_name) {
            return `napi_valuetype number_val${arg_name};
                
                status = napi_typeof(env, ${ napi_value_name}, &number_val${arg_name});
                
                if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
                if(number_val${arg_name} != napi_number) return JSError(env, "Argument ${arg_name} is not a number type.");
    
                double ${arg_name};
    
                status = napi_get_value_double(env, ${napi_value_name}, &${arg_name});
                if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");`;
        },
        method_return(hive_struct_name = "", napi_return_value = "", method_call = "") {
            return `double val = ${method_call}
                status = napi_create_double(env, val, &${napi_return_value});
                if(status != napi_ok) return JSError(env, "Could not write value as a double");`;
        }

    },

    bool: {
        getter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_create_uint32(env, static_cast<unsigned>(${hive_prop_instance_name} -> ${hive_prop_instance_member}), &${napi_value_name});
                if(status != napi_ok) return JSError(env, "Could not read value as an bool integer");`;
        },

        setter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return ` status = napi_get_value_bool(env, ${napi_value_name}, &(${hive_prop_instance_name}->${hive_prop_instance_member}));
                if(status != napi_ok) return JSError(env, "Could not read value as an bool integer");`;
        },

        method_arg(hive_struct_name = "", arg_name = "", napi_value_name) {
            return `napi_valuetype number_val${arg_name};
            
            status = napi_typeof(env, ${ napi_value_name}, &number_val${arg_name});

            if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
            if(number_val${arg_name} != napi_boolean) return JSError(env, "Argument ${arg_name} is not a number type.");

            bool ${arg_name};

            status = napi_get_value_bool(env, ${napi_value_name}, &${arg_name});

            if (status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");`;
        },

        method_return(hive_struct_name = "", napi_return_value = "", method_call = "") {
            return `bool val = ${method_call}
            status = napi_create_uint32(env, static_cast<unsigned>(val), &${napi_return_value});
            if(status != napi_ok) return JSError(env, "Could not write value as an unsigned integer");`;
        }
    },

    char: {
        getter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_create_int32(env, static_cast<int>(${hive_prop_instance_name} -> ${hive_prop_instance_member}), &${napi_value_name});
                if(status != napi_ok) return JSError(env, "Could not read value as a char integer");`;
        },

        setter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `int temp_int;
                status = napi_get_value_int32(env, ${ napi_value_name}, &temp_int);
                ${hive_prop_instance_name}->${hive_prop_instance_member} = static_cast<char>(${hive_prop_instance_name} -> ${hive_prop_instance_member});
                if(status != napi_ok) return JSError(env, "Could not read value as a char integer");`;
        },

        method_arg(hive_struct_name = "", arg_name = "", napi_value_name) {
            return `napi_valuetype number_val${arg_name};
            
            status = napi_typeof(env, ${ napi_value_name}, &number_val${arg_name});

            if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
            if(number_val${arg_name} != napi_number) return JSError(env, "Argument ${arg_name} is not a number type.");

            int temp_${arg_name};
            char ${arg_name};

            status = napi_get_value_int32(env, ${napi_value_name}, &temp_${arg_name});

            if (status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
            
            ${arg_name} = static_cast<char>(temp_${arg_name});`;
        },

        method_return(hive_struct_name = "", napi_return_value = "", method_call = "") {
            return `char val = ${method_call}
            status = napi_create_int32(env, static_cast<int>(val), &${napi_return_value});
            if(status != napi_ok) return JSError(env, "Could not write value as an int integer");`;
        }
    },

    short: {
        getter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_create_int32(env, static_cast<int>(${hive_prop_instance_name} -> ${hive_prop_instance_member}), &${napi_value_name});
                if(status != napi_ok) return JSError(env, "Could not read value as a short integer");`;
        },

        setter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `int temp_int;
                status = napi_get_value_int32(env, ${ napi_value_name}, &temp_int);
                ${hive_prop_instance_name}->${hive_prop_instance_member} = static_cast<short>(${hive_prop_instance_name} -> ${hive_prop_instance_member});
                if(status != napi_ok) return JSError(env, "Could not read value as a short integer");`;
        },

        method_arg(hive_struct_name = "", arg_name = "", napi_value_name) {
            return `napi_valuetype number_val${arg_name};
            
            status = napi_typeof(env, ${ napi_value_name}, &number_val${arg_name});

            if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
            if(number_val${arg_name} != napi_number) return JSError(env, "Argument ${arg_name} is not a number type.");

            int temp_${arg_name};
            short ${arg_name};

            status = napi_get_value_int32(env, ${napi_value_name}, &temp_${arg_name});
            program
            if (status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
            
            ${arg_name} = static_cast<short>(temp_${arg_name});`;
        },

        method_return(hive_struct_name = "", napi_return_value = "", method_call = "") {
            return `short val = ${method_call}
            status = napi_create_int32(env, static_cast<int>(val), &${napi_return_value});
            if(status != napi_ok) return JSError(env, "Could not write value as an int integer");`;
        }
    },

    unsigned_short: {
        getter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_create_uint32(env, static_cast<unsigned>(${hive_prop_instance_name} -> ${hive_prop_instance_member}), &${napi_value_name});
                if(status != napi_ok) return JSError(env, "Could not read value as an unsigned short integer");`;
        },

        setter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `unsigned temp_unsigned;
                status = napi_get_value_uint32(env, ${ napi_value_name}, (unsigned *) &temp_unsigned);
                ${hive_prop_instance_name}->${hive_prop_instance_member} = static_cast<unsigned short>(${hive_prop_instance_name} -> ${hive_prop_instance_member});
                if(status != napi_ok) return JSError(env, "Could not read value as an unsigned short integer");`;
        },

        method_arg(hive_struct_name = "", arg_name = "", napi_value_name) {
            return `napi_valuetype number_val${arg_name};
            
            status = napi_typeof(env, ${ napi_value_name}, &number_val${arg_name});

            if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
            if(number_val${arg_name} != napi_number) return JSError(env, "Argument ${arg_name} is not a number type.");

            unsigned temp_${arg_name};
            unsigned short ${arg_name};

            status = napi_get_value_uint32(env, ${napi_value_name}, &temp_${arg_name});

            if (status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
            
            ${arg_name} = static_cast<unsigned short>(temp_${arg_name});`;
        },

        method_return(hive_struct_name = "", napi_return_value = "", method_call = "") {
            return `unsigned short val = ${method_call}
            status = napi_create_uint32(env, static_cast<unsigned>(val), &${napi_return_value});
            if(status != napi_ok) return JSError(env, "Could not write value as an unsigned integer");`;
        }
    },

    unsigned: {

        getter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_create_uint32(env, ${hive_prop_instance_name} -> ${hive_prop_instance_member}, & ${napi_value_name});

                if(status != napi_ok) return JSError(env, "Could not read value as an unsigned integer");`;
        },
        setter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_get_value_uint32(env, ${napi_value_name}, (unsigned *) & (${hive_prop_instance_name} -> ${hive_prop_instance_member}));

                if(status != napi_ok) return JSError(env, "Could not read value as an unsigned integer");`;
        },
        method_arg(hive_struct_name = "", arg_name = "", napi_value_name) {
            return `napi_valuetype number_val${arg_name};
            
            status = napi_typeof(env, ${ napi_value_name}, &number_val${arg_name});
            
            if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
            if(number_val${arg_name} != napi_number) return JSError(env, "Argument ${arg_name} is not a number type.");

            unsigned ${arg_name};

            status = napi_get_value_uint32(env, ${napi_value_name}, &${arg_name});

            if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");`;
        },
        method_return(hive_struct_name = "", napi_return_value = "", method_call = "") {
            return `unsigned val = ${method_call}
            status = napi_create_uint32(env, val, &${napi_return_value});
            if(status != napi_ok) return JSError(env, "Could not write value as an unsigned integer");`;
        }
    },

    integer: {
        getter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_create_int32(env, ${hive_prop_instance_name} -> ${hive_prop_instance_member}, & ${napi_value_name});
                if(status != napi_ok) return JSError(env, "Could not read value as an integer");`;
        },
        setter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_get_value_int32(env, ${napi_value_name}, &(${hive_prop_instance_name} -> ${hive_prop_instance_member}));
                if(status != napi_ok) return JSError(env, "Could not read value as an integer");`;
        },
        method_arg(hive_struct_name = "", arg_name = "", napi_value_name) {
            return `napi_valuetype number_val${arg_name};
            
            status = napi_typeof(env, ${ napi_value_name}, &number_val${arg_name});
            
            if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
            if(number_val${arg_name} != napi_number) return JSError(env, "Argument ${arg_name} is not a number type.");

            int ${arg_name};

            status = napi_get_value_int32(env, ${napi_value_name}, &${arg_name});
            if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");`;
        },
        method_return(hive_struct_name = "", napi_return_value = "", method_call = "") {
            return `int val = ${method_call}
            status = napi_create_int32(env, val, &${napi_return_value});
            if(status != napi_ok) return JSError(env, "Could not write value as an integer");`;
        }
    },

    unsigned_long_integer: {
        getter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_create_int64(env, static_cast<long long>(${hive_prop_instance_name} -> ${hive_prop_instance_member}), &${napi_value_name});
                if(status != napi_ok) return JSError(env, "Could not read value as a long integer");`;
        },
        setter(hive_struct_name = "prop", napi_valueprogram_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `int64_t temp;
            status = napi_get_value_int64(env, ${napi_value_name}, &temp);
            if(status != napi_ok) return JSError(env, "Could not read value as a long integer");
            ${hive_prop_instance_name} -> ${hive_prop_instance_member} = static_cast<unsigned long long>(temp);`;
        },
        method_arg(hive_struct_name = "", arg_name = "", napi_value_name) {
            return `napi_valuetype number_val${arg_name};
            
            status = napi_typeof(env, ${ napi_value_name}, &number_val${arg_name});
            
            if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
            if(number_val${arg_name} != napi_number) return JSError(env, "Argument ${arg_name} is not a number type.");
            
            long long temp_${arg_name};
            
            unsigned long long ${arg_name};

            status = napi_get_value_int64(env, ${napi_value_name}, &temp_${arg_name});
            if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
            ${arg_name} = static_cast<unsigned long long>(temp);`;
        },
        method_return(hive_struct_name = "", napi_return_value = "", method_call = "") {
            return `unsigned long long temp_ull = ${method_call}
            status = napi_create_int64(env, static_cast<long long>(temp_ull), &${napi_return_value});
            if(status != napi_ok) return JSError(env, "Could not write value as a long integer");`;
        }
    },

    long_integer: {
        getter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_create_int64(env, ${hive_prop_instance_name} -> ${hive_prop_instance_member}, &${napi_value_name});
                if(status != napi_ok) return JSError(env, "Could not read value as a long integer");`;
        },
        setter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `status = napi_get_value_int64(env, ${napi_value_name}, &(${hive_prop_instance_name} -> ${hive_prop_instance_member}));
                if(status != napi_ok) return JSError(env, "Could not read value as a long integer");`;
        },
        method_arg(hive_struct_name = "", arg_name = "", napi_value_name) {
            return `napi_valuetype number_val${arg_name};
            
            status = napi_typeof(env, ${ napi_value_name}, &number_val${arg_name});
            
            if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");
            if(number_val${arg_name} != napi_number) return JSError(env, "Argument ${arg_name} is not a number type.");

            long int ${arg_name};

            status = napi_get_value_int64(env, ${napi_value_name}, &${arg_name});
            if(status != napi_ok) return JSError(env, "Could not read argument ${arg_name}.");`;
        },
        method_return(hive_struct_name = "", napi_return_value = "", method_call = "") {
            return `long int val = ${method_call}
            status = napi_create_int64(env, val, &${napi_return_value});
            if(status != napi_ok) return JSError(env, "Could not write value as a long integer");`;
        }
    },

    void: {
        getter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `/* VOID hive_struct_name: ${hive_struct_name} napi_value_name: ${napi_value_name} hive_prop_instance_name: ${hive_prop_instance_name} hive_prop_instance_member: ${hive_prop_instance_member}*/`;
        },
        setter(hive_struct_name = "prop", napi_value_name = "value", hive_prop_instance_name = "prop", hive_prop_instance_member = "undefined") {
            return `/* VOID hive_struct_name: ${hive_struct_name} napi_value_name: ${napi_value_name} hive_prop_instance_name: ${hive_prop_instance_name} hive_prop_instance_member: ${hive_prop_instance_member}*/`;
        },
        method_arg(hive_struct_name = "", arg_name = "", napi_value_name) {
            return `${hive_struct_name} * ${arg_name}; napi_throw_error(env, nullptr, "Cannot access unexported class ${hive_struct_name}");
            return nullptr; /* VOID hive_struct_name: ${hive_struct_name} arg_name: ${arg_name} napi_value_name: ${napi_value_name}*/`;
        },
        method_return(hive_struct_name = "", napi_return_value = "", method_call = "") {
            return `${method_call}`;
        }
    }
};

function getJSToCValue(param) {

    const name = param.name;

    const type = param.type.join(" ");

    if (name.type == "ptr") {

        const name = "a";

        if (prop_names.has(type)) {
            return { type, name, render: conversion_objects.prop };
        } else {
            return { type, name, render: conversion_objects.void, void: true };
        }

    } else {
        switch (type) {
            case "std::string":
            case "string":
                return { type, name, render: conversion_objects.string };
            case "char":
                return { type, name, render: conversion_objects.char };
            case "bool":
                return { type, name, render: conversion_objects.unsigned };
            case "short":
                return { type, name, render: conversion_objects.unsigned_short };
            case "ushort":
            case "unsigned short":
                return { type, name, render: conversion_objects.unsigned_short };
            case "short":
                return { type, name, render: conversion_objects.unsigned };
            case "double":
                return { type, name, render: conversion_objects.double };
            case "float":
                return { type, name, render: conversion_objects.float };
            case "long":
            case "int":
                return { type, name, render: conversion_objects.integer };
            case "long long":
            case "long long int":
                return { type, name, render: conversion_objects.long_integer };
            case "unsigned long long":
            case "unsigned long long int":
                return { type, name, render: conversion_objects.unsigned_long_integer };
            case "unsigned int":
            case "unsigned":
                return { type, name, render: conversion_objects.unsigned };
            default:
                return { type, name, render: conversion_objects.void };
        }
    }
}

utils.start();