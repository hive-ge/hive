#pragma once

#include "gpu/opengl/buffer.h"
#include "gpu/opengl/glwrap.h"
#include <GL/gl.h>

namespace hive
{
    namespace gl
    {


        /**
            Wrapper around an attribute int.
        */
        struct SmartGLInput : SmartGLint {

          protected:
            // Primitive type
            unsigned primitive;

            // Data type of input.
            unsigned type = 0;

            // Number of elements. If 1 then is a single
            unsigned size = 0;

            // Total byte size of element.
            unsigned bytes = 0;

            // Length of array
            unsigned length = 1;

            unsigned flags = 0;

            // Input (Attribute) location
            unsigned location = 0;

            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLInput() : SmartGLint(SmartGLType::Input){};

            SmartGLInput(GLuint program_pointer, int index)
                : SmartGLint(SmartGLType::Input, index, true)
            {
                const unsigned int properties[] = {
                    GL_TYPE,                              // int
                    GL_ARRAY_SIZE,                        // int
                    GL_LOCATION,                          // int
                    GL_LOCATION_COMPONENT,                // int
                    GL_REFERENCED_BY_VERTEX_SHADER,       // int = 1 | 0
                    GL_REFERENCED_BY_COMPUTE_SHADER,      // int = 1 | 0
                    GL_REFERENCED_BY_TESS_CONTROL_SHADER, // int = 1 | 0
                    GL_REFERENCED_BY_GEOMETRY_SHADER,     // int = 1 | 0
                    GL_REFERENCED_BY_FRAGMENT_SHADER,     // int = 1 | 0
                };

                const int prop_count = sizeof(properties) >> 2;
                int actual;
                int vals[prop_count];

                glGetProgramResourceiv(program_pointer, GL_PROGRAM_INPUT, index, prop_count,
                                       properties, prop_count, &actual, vals);

                type      = vals[0];
                primitive = getGLSLTypePrimitive(type);
                size      = getGLSLTypeElementCount(type);
                bytes     = getGLSLTypeSize(type);
                length    = vals[1];
                location  = vals[2];
                flags     = (vals[4] << 0) | (vals[5] << 2) | (vals[5] << 4) | (vals[5] << 8) |
                        (vals[6] << 16);

                pointer = location;
            };

            ~SmartGLInput() { decreaseReferenceCount(); }

            SmartGLInput(const SmartGLInput & obj) : SmartGLint(obj)
            {
                type      = obj.type;
                size      = obj.size;
                bytes     = obj.bytes;
                length    = obj.length;
                primitive = obj.primitive;
                flags     = obj.flags;
                location  = obj.location;
            }
            /**


            */
            virtual void use(SmartGLBuffer & buffer, GLenum type, unsigned stride, unsigned offset,
                             unsigned divisor = 0, bool normalize = false);
            virtual void release() override;
            virtual bool IS_USABLE() override;
        };

        void SmartGLInput::deleteUnderlyingGLResource() {}


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
        void SmartGLInput::use(SmartGLBuffer & buffer, GLenum type, unsigned stride, unsigned off,
                               unsigned divisor, bool normalize)
        {

#ifdef HIVE_DEBUG //############################## DEBUG

            clearErrors();
#endif //############################## END DEBUG

            if (!IS_READY) {
                throw("Attribute pointer is not ready to be used.");
                return;
            }

            buffer.use(SKGLB::ARRAY_BUFFER);

            glVertexAttribDivisor(pointer, divisor);

            glEnableVertexAttribArray(pointer);

            switch (primitive) {
            case 0: // uint
            case 1: // int
                glVertexAttribIPointer(pointer, size, type, stride, (void *)off);
                break;
            case 2: // float
                glVertexAttribPointer(pointer, size, type, normalize, stride, (void *)off);
                break;
            case 3: // double
                glVertexAttribLPointer(pointer, size, type, stride, (void *)off);
                break;
            }

#ifdef HIVE_DEBUG //############################## DEBUG

            GLenum error = glGetError();

            if (error != GL_NO_ERROR) IS_READY = false;

            switch (error) {
            case GL_INVALID_ENUM:
                // GL_INVALID_VALUE is generated if stride is negative.
                __ERROR("Stride is negative");
                break;


            case GL_INVALID_VALUE:
                /**
                 * GL_INVALID_VALUE is generated if index is greater than or equal to
                 * GL_MAX_VERTEX_ATTRIBS.
                 *
                 * GL_INVALID_VALUE is generated if size is not 1, 2, 3, 4 or (for
                 * glVertexAttribPointer), GL_BGRA.
                 */
                __ERROR("Buffer name is not valid.");
                break;

                /**
                 * GL_INVALID_OPERATION is generated if size is GL_BGRA and type is not
                 *GL_UNSIGNED_BYTE, GL_INT_2_10_10_10_REV or GL_UNSIGNED_INT_2_10_10_10_REV.
                 *
                 *GL_INVALID_OPERATION is generated if type is GL_INT_2_10_10_10_REV or
                 *GL_UNSIGNED_INT_2_10_10_10_REV and size is not 4 or GL_BGRA.
                 *
                 *GL_INVALID_OPERATION is generated if type is GL_UNSIGNED_INT_10F_11F_11F_REV and
                 *size is not 3.
                 *
                 *GL_INVALID_OPERATION is generated by glVertexAttribPointer if size is GL_BGRA and
                 *normalized is GL_FALSE.
                 *
                 *GL_INVALID_OPERATION is generated if zero is bound to the GL_ARRAY_BUFFER buffer
                 *object binding point and the pointer argument is not NULL.
                 */
            case GL_INVALID_OPERATION:
                __ERROR("TODO - invalid operation message in input.");
                break;
            }
#endif //############################## END DEBUG
        }

        void SmartGLInput::release() {}

        bool SmartGLInput::IS_USABLE() { return true; }
    } // namespace gl
} // namespace hive
