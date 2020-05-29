#!/usr/bin/env python3.6
""" Usage: call with <filename> <typename>
"""

import sys
import clang.cindex as clang
import os
import logging
import threading
import time

'''Get CWD
'''
cwd = os.path.abspath("./")

class Type(object):
    def __init__(self, type_object):
        self.is_pointer = type_object.get_pointee().kind != clang.TypeKind.INVALID
        self.name = type_object.spelling

    def __str__(self):
        return ("{}: is_pointer: {}".format(self.name, self.is_pointer))

class DestructorDeclaration(object):
    def __init__(self, node):
        self.name = node.spelling
        self.arguments = list(map(lambda node: Type(node.type), node.get_arguments()))
        self.return_type = Type(node.type)

    def __str__(self):
        return str(self.__dict__)

class ConstructorDeclaration(object):
    def __init__(self, node):
        self.name = node.spelling
        self.arguments = map(lambda node: Type(node.type), node.get_arguments())
        self.return_type = Type(node.type)

    def __str__(self):
        return str(self.__dict__)

class MethodDeclaration(object):
    def __init__(self, node):
        self.name = node.spelling
        self.arguments = map(lambda node: Type(node.type), node.get_arguments())
        self.return_type = Type(node.type)
        self.uses_pointers = len(list(filter(lambda type: type.is_pointer,self.arguments))) > 0

    def __str__(self):
        return str(self.__dict__)

class StructDeclaration(object):
    def __init__(self, node, base_class, constructor, destructor, methods, properties):
        self.name = node.spelling
        self.base_class = base_class
        self.constructor = constructor
        self.destructor = destructor
        self.methods = methods
        self.properties = properties

    def __str__(self):
        return str(self.__dict__)

    

def setBaseClass(node):
    return Type(node.type)

def setConstructor(node):
    return ConstructorDeclaration(node)

def setDestructor(node):
    return DestructorDeclaration(node)
        
def setMethod(node):
    return MethodDeclaration(node)

def setProperty(node):
    return


def find_props(node, structs, location):

    if node.kind.is_declaration():

        
        if node.brief_comment and node.brief_comment == "::HIVE DRONE_PROP::" :
            # Grab All References, check for pointers and create objects that can be used
            # by integration parsers.
            #Check to make sure declaration is a class or struct:

            if node.kind == clang.CursorKind.STRUCT_DECL or node.kind == clang.CursorKind.CLASS_DECL:

                token_source_file = os.path.join(cwd, node.get_tokens().__next__().location.file.name)                
            

                if(token_source_file == location):

                    print(node.spelling)

                    base_class, constructor, destructor = None, None, None
                    methods, properties = [] , []
                    
                    for c in node.get_children():
                        if(c.kind == clang.CursorKind.CXX_BASE_SPECIFIER):
                            base_class = setBaseClass(c)
                        
                        elif(c.kind == clang.CursorKind.CONSTRUCTOR):
                            constructor = setConstructor(c)

                        elif(c.kind == clang.CursorKind.DESTRUCTOR):
                            destructor = setDestructor(c)

                        elif(c.kind == clang.CursorKind.CXX_METHOD):
                            methods.append(setMethod(c))

                        elif(c.kind == clang.CursorKind.FIELD_DECL):
                            properties.append(setProperty(c))

                    structs.append(StructDeclaration(node, base_class, constructor, destructor, methods, properties))
                
                # Recurse for children of this node
                # return;
                

    for c in node.get_children():
        find_props(c, structs, location)



def getPropStructs(folder, args):

    structs = []

    pwd = os.path.join(cwd, folder)
    
    index = clang.Index.create()

    dir_list = [f for f in os.listdir(pwd) if os.path.isfile(os.path.join(pwd, f))]

    for file in dir_list:
        
        location = os.path.join(pwd, file);

        print(location)
        
        tu = index.parse(location, args)
        
        find_props(tu.cursor, structs, location)
    
    return structs