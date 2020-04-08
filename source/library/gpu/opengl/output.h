#pragma once

#ifdef HIVE_USE_OPENGL

#include "gpu/opengl/buffer.h"
#include "gpu/opengl/glwrap.h"

namespace hive
{
    namespace gl
    {


        /**
            Wrapper around an attribute int.
        */
        struct SmartGLOutput : SmartGLint {

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
            SmartGLOutput() : SmartGLint(SmartGLType::Output){};

            SmartGLOutput(GLuint program_pointer, int index)
                : SmartGLint(SmartGLType::Output, program_pointer, true)
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
                    GL_LOCATION_INDEX,                    // int
                };

                const int prop_count = sizeof(properties) >> 2;
                int actual;
                int vals[prop_count];

                glGetProgramResourceiv(program_pointer, GL_PROGRAM_OUTPUT, index, prop_count,
                                       properties, prop_count, &actual, vals);

                type      = vals[0];
                primitive = getGLSLTypePrimitive(type);
                size      = getGLSLTypeElementCount(type);
                bytes     = getGLSLTypeSize(type);
                length    = vals[1];
                location  = vals[2];
                flags     = (vals[4] << 0) | (vals[5] << 2) | (vals[5] << 4) | (vals[5] << 8) |
                        (vals[6] << 16);
            };

            ~SmartGLOutput() { decreaseReferenceCount(); }

            SmartGLOutput(const SmartGLOutput & obj) : SmartGLint(obj)
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
            virtual void use(VRAMBuffer & buffer, GLenum type, unsigned stride, unsigned offset,
                             unsigned divisor = 0, bool normalize = false);
            virtual void release() override;
            virtual bool IS_USABLE() override;
        };

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
    } // namespace gl
} // namespace hive

#endif