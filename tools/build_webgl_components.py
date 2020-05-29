#!/usr/bin/env python3.6
import prop_parser


file_header = """/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
}"""

structs = prop_parser.getPropStructs("source/library/include/primitive", ["-I./source/library/"])

for struct in structs:
    # sort by dependency graph
    print ("emscripten::class_<{}{}>(\"{}\")".format(struct.name, ",emscripten::base<{}>".format(struct.base_class.name) if struct.base_class else "", struct.name))

    for method in struct.methods:
        if method.name == "construct":
            print((".constructor(&{}::{},emscripten::allow_raw_pointers())".format(struct.name, method.name)))
        else:
            print((".function(\"{}\", &{}::{})".format(method.name, struct.name, method.name)))
