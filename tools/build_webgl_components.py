#!/usr/bin/env python3.6
import prop_parser
import os


file_header = """/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    DO NOT MODIFY - This file is automatically created by the build:props script.

    COPYRIGHT 2020 Anthony C Weathersby
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

#pragma once
#include <thread>
#include <vector>
#include <hive.h>
#include <emscripten/bind.h>


namespace hive{{
    namespace js{{

        using namespace hive::all;

        EMSCRIPTEN_BINDINGS(hive_property_bindings) {{
            {}
        }}
    }}
}}"""

structs = prop_parser.getPropStructs("source/library/include/primitive", ["-I./source/library/"], "::HIVE DRONE_PROP::")


# sort by dependency graph
def sortStructPair(a,b):
    if( a.base_class.name == b.name):
        return -1 # a needs to be placed after b
    if( b.base_class.name == a.name):
        return 1 # b needs to be placed after a
    return 0 # no change needs to occur

def sortStructs(unsorted_struct_list, sorted_struct_list = [], index = 0):

    MADE_SORT = false

    a = unsorted_struct_list[index]

    for i in range(index, len(unsorted_struct_list)-1):
            
        b = unsorted_struct_list[i+1]

        result = sortStructPair(a,b)

        if(result == -1):
            #already in correct order
            sorted_struct_list.append(a)
            a = b
            unsorted_struct_list
        if(result == 1):
            sorted_struct_list.append(b)
            MADE_SORT = true
        if(result == 0):
            #order does not matter
            sorted_struct_list.append(a)
            a = b
    
    sorted_struct_list.append(a);
        
    if MADE_SORT:
        return sortStructs(sorted_struct_list)

    return sorted_struct_list


binding_strings = []

for struct in structs:


    struct_binding_string = ("emscripten::class_<{}{}>(\"{}\")".format(struct.name, ",emscripten::base<{}>".format(struct.base_class.name) if struct.base_class else "", struct.name))

    for method in struct.methods:
        if method.name == "construct":
            struct_binding_string += (".constructor(&{}::{},emscripten::allow_raw_pointers())".format(struct.name, method.name))
        else:
            struct_binding_string +=  (".function(\"{}\", &{}::{}{})".format(method.name, struct.name, method.name, ",emscripten::allow_raw_pointers()" if method.uses_pointers else ""))

    binding_strings.append(struct_binding_string +  ";")

template_file = open("./source/webassembly/interfaces.hpp", "w")
template_file.write(file_header.format("\n\n".join(binding_strings)))
template_file.close()
    



