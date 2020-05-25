#pragma once

#include <GL/glew.h>

#include <GL/gl.h>

namespace hive
{
    // Byte size of a single index of type.
    unsigned getGLSLTypeSize(GLint t);
    unsigned getGLSLTypeElementCount(GLint t);
    unsigned getGLSLTypePrimitive(GLint t);
} // namespace hive
