#pragma once

#include "gpu/opengl/buffer.hpp"
#include "gpu/opengl/glwrap.hpp"
#include <GL/gl.h>

namespace hive
{
    namespace gl
    {


        /**
            Wrapper around an attribute int.
        */
        struct SmartGLInput : hive::gl::SmartGLint {

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
            virtual void use(VRAMBuffer & buffer, GLenum type, unsigned stride = 0,
                             unsigned offset = 0, unsigned divisor = 0, bool normalize = false);
            virtual void release() override;
            virtual bool IS_USABLE() override;
        };
    } // namespace gl
} // namespace hive
