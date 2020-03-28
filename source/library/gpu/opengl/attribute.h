#pragma once

#include "gpu/opengl/buffer.h"
#include "gpu/opengl/glwrap.h"

namespace hive
{
    namespace gl
    {


        /**
            Wrapper around an attribute int.
        */
        struct SmartGLAttribute : SmartGLint {

          protected:
            unsigned short glsl_array_size = 0;

            unsigned char glsl_object_size = 0;

            unsigned char glsl_object_type = 0;

            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLAttribute() : SmartGLint(SmartGLType::Attribute){};

            SmartGLAttribute(GLuint program_pointer, unsigned os, unsigned as, unsigned at,
                             bool ISREADY)
                : SmartGLint(SmartGLType::Attribute, program_pointer, ISREADY), glsl_array_size(as),
                  glsl_object_size(os), glsl_object_type(at){};

            ~SmartGLAttribute() { decreaseReferenceCount(); }

            SmartGLAttribute(const SmartGLAttribute & obj) : SmartGLint(obj)
            {
                glsl_array_size  = obj.glsl_array_size;
                glsl_object_size = obj.glsl_object_size;
                glsl_object_type = obj.glsl_object_type;
            }
            /**


            */
            virtual void use(SmartGLBuffer & buffer, GLenum type, unsigned stride, unsigned offset,
                             unsigned divisor = 0, bool normalize = false);

            virtual void release() override;

            virtual bool IS_USABLE() override;
        };

        void SmartGLAttribute::deleteUnderlyingGLResource() {}


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
        void SmartGLAttribute::use(SmartGLBuffer & buffer, GLenum type, unsigned stride,
                                   unsigned offset, unsigned divisor, bool normalize)
        {
            if (!IS_READY) {
                throw("Attribute pointer is not ready to be used.");
                return;
            }
            glVertexAttribDivisor(pointer, divisor);
            glEnableVertexAttribArray(pointer);

            buffer.use(SKGLB::ARRAY_BUFFER);

            switch (glsl_object_type) {
            case 0: // uint
            case 1: // int
                glVertexAttribIPointer(pointer, glsl_object_size, type, stride, (void *)offset);
                break;
            case 2: // float
                glVertexAttribPointer(pointer, glsl_object_size, type, normalize, stride,
                                      (void *)offset);
                break;
            case 3: // double
                glVertexAttribLPointer(pointer, glsl_object_size, type, stride, (void *)offset);
                break;
            }
        }

        void SmartGLAttribute::release() {}

        bool SmartGLAttribute::IS_USABLE() { return true; }
    } // namespace gl
} // namespace hive
