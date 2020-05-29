#!/usr/bin/env python3.6
""" Usage: call with <filename> <typename>
"""

import sys
import clang.cindex
import os

'''Get CWD
'''
dir = os.path.dirname(__file__)

def find_typerefs(node, typename):
    """ Find all references to the type named 'typename'
    """
    if node.kind.is_reference():
        ref_node = clang.cindex.Cursor_ref(node)
        if ref_node.spelling == typename:
            print 'Found %s [line=%s, col=%s]' % (
                typename, node.location.line, node.location.column)
    # Recurse for children of this node
    for c in node.get_children():
        find_typerefs(c, typename)


dir = os.path.join(dir, sys.argv[1])

print "Parsing:", dir

index = clang.cindex.Index.create()

tu = index.parse(dir)

print 'Translation unit:', tu.spelling

find_typerefs(tu.cursor, sys.argv[2])