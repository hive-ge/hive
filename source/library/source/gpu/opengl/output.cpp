#include "./output.hpp"

using namespace hive;
using namespace hive::gl;

void SmartGLOutput::deleteUnderlyingGLResource() {}


/**
 *
 *    glVertexAttribPointer and glVertexAttribIPointer:
 *    GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT,
 *    GL_INT, and GL_UNSIGNED_INT
 *
 *    glVertexAttribPointer:
 *    GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV,
 *   GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV
 *
 *    glVertexAttribLPointer ONLY:
 *    GL_DOUBLE
 *
 */
void SmartGLOutput::use(VRAMBuffer & buffer, GLenum type, unsigned stride, unsigned offset,
                        unsigned divisor, bool normalize)
{
    if (!IS_READY) {
        throw("Attribute pointer is not ready to be used.");
        return;
    }

    glVertexAttribDivisor(pointer, divisor);

    glEnableVertexAttribArray(pointer);

    buffer.use(SKGLB::ARRAY_BUFFER);

    switch (primitive) {
    case 0: // uint
    case 1: // int
        glVertexAttribIPointer(pointer, size, type, stride, (void *)offset);
        break;
    case 2: // float
        glVertexAttribPointer(pointer, size, type, normalize, stride, (void *)offset);
        break;
    case 3: // double
        glVertexAttribLPointer(pointer, size, type, stride, (void *)offset);
        break;
    }
}

void SmartGLOutput::release() {}

bool SmartGLOutput::IS_USABLE() { return true; }