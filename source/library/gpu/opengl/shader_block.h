#pragma once

#include "buffer.h"
#include "gpu/opengl/glwrap.h"
#include <gl3w/GL/glcorearb.h>

namespace hive
{

    namespace gl
    {

        struct SmartGLShaderBlock : SmartGLint {
          protected:
            // Primitive type
            unsigned program              = 0;
            unsigned buffer_binding       = 0;
            unsigned buffer_data_size     = 0;
            unsigned num_active_variables = 0;
            unsigned active_variables     = 0;
            unsigned index                = 0;

            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLShaderBlock() : SmartGLint(SmartGLType::Uniform){};

            SmartGLShaderBlock(GLuint program_pointer, int index)
                : SmartGLint(SmartGLType::UniformBlock, program_pointer, true)
            {
                const unsigned int properties[] = {
                    GL_BUFFER_BINDING,
                    GL_BUFFER_DATA_SIZE,
                    GL_NUM_ACTIVE_VARIABLES,
                    GL_ACTIVE_VARIABLES,
                    GL_REFERENCED_BY_VERTEX_SHADER,
                    GL_REFERENCED_BY_TESS_CONTROL_SHADER,
                    GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
                    GL_REFERENCED_BY_GEOMETRY_SHADER,
                    GL_REFERENCED_BY_FRAGMENT_SHADER,
                    GL_REFERENCED_BY_COMPUTE_SHADER,
                };

                const int prop_count = sizeof(properties) >> 2;
                int actual;
                int vals[prop_count];
                ;

                glGetProgramResourceiv(program_pointer, GL_SHADER_STORAGE_BLOCK, index, prop_count,
                                       properties, prop_count, &actual, vals);
                SmartGLShaderBlock::index = index;
                program                   = program_pointer;
                buffer_binding            = vals[0];
                buffer_data_size          = vals[1];
                num_active_variables      = vals[2];
                active_variables          = vals[3];
            };

            ~SmartGLShaderBlock() { decreaseReferenceCount(); }

            SmartGLShaderBlock(const SmartGLShaderBlock & obj) : SmartGLint(obj)
            {
                buffer_binding       = obj.buffer_binding;
                buffer_data_size     = obj.buffer_data_size;
                num_active_variables = obj.num_active_variables;
                active_variables     = obj.active_variables;
            }
            virtual void use(int pointer = 0);
            virtual void release() override;
            virtual bool IS_USABLE() override;
        }; // namespace gl

        void SmartGLShaderBlock::deleteUnderlyingGLResource() {}

        void SmartGLShaderBlock::use(int binding_point)
        {
            glShaderStorageBlockBinding(program, index, binding_point);
        }

        void SmartGLShaderBlock::release() {}

        bool SmartGLShaderBlock::IS_USABLE() { return false; }
    } // namespace gl
} // namespace hive