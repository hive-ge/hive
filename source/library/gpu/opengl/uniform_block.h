#pragma once

#ifdef HIVE_USE_OPENGL

#include "buffer.h"
#include "gpu/opengl/glwrap.h"
#include <gl3w/GL/glcorearb.h>


namespace hive
{

    namespace gl
    {
        /**
         * Buffer blocks need to be rebound. This data is not saved
         * to the with vertex array object
         */
        struct SmartGLUniformBlock : SmartGLint {

          protected:
            unsigned program              = 0;
            unsigned buffer_binding       = 0;
            unsigned buffer_data_size     = 0;
            unsigned num_active_variables = 0;
            unsigned active_variables     = 0;
            unsigned index                = 0;

            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLUniformBlock() : SmartGLint(SmartGLType::UniformBlock){};

            SmartGLUniformBlock(GLuint program_pointer, int ind)
                : SmartGLint(SmartGLType::UniformBlock, program_pointer, true)
            {
                const unsigned int properties[] = {
                    GL_BUFFER_BINDING,
                    GL_BUFFER_DATA_SIZE,
                    GL_NUM_ACTIVE_VARIABLES,
                    GL_REFERENCED_BY_VERTEX_SHADER,
                    GL_REFERENCED_BY_TESS_CONTROL_SHADER,
                    GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
                    GL_REFERENCED_BY_GEOMETRY_SHADER,
                    GL_REFERENCED_BY_FRAGMENT_SHADER,
                    GL_REFERENCED_BY_COMPUTE_SHADER,
                    GL_ACTIVE_VARIABLES,
                };

                const int prop_count = sizeof(properties) >> 2;

                int actual;

                int vals[prop_count + 32];

                glGetProgramResourceiv(program_pointer, GL_UNIFORM_BLOCK, ind, prop_count,
                                       properties, prop_count, &actual, vals);

                index                = ind;
                program              = program_pointer;
                buffer_binding       = vals[0];
                buffer_data_size     = vals[1];
                num_active_variables = vals[2];
                active_variables     = vals[3];
            };

            ~SmartGLUniformBlock() { decreaseReferenceCount(); }

            SmartGLUniformBlock(const SmartGLUniformBlock & obj) : SmartGLint(obj)
            {
                program              = obj.program;
                buffer_binding       = obj.buffer_binding;
                buffer_data_size     = obj.buffer_data_size;
                num_active_variables = obj.num_active_variables;
                active_variables     = obj.active_variables;
                index                = obj.index;
            }
            virtual void bind(VRAMBuffer & buffer, unsigned binding_point = 0, unsigned offset = 0,
                              unsigned size = 0);

            virtual void release() override;

            virtual bool IS_USABLE() override;
        }; // namespace gl

        void SmartGLUniformBlock::deleteUnderlyingGLResource() {}

        void SmartGLUniformBlock::bind(VRAMBuffer & buffer, unsigned binding_point, unsigned offset,
                                       unsigned size)
        {
            // buffer.use(UNIFORM_BUFFER);

            if (size == 0) size = buffer.size();

            glUniformBlockBinding(program, index, binding_point);

            buffer.useRange(UNIFORM_BUFFER, binding_point, offset, size);
        }

        void SmartGLUniformBlock::release() {}

        bool SmartGLUniformBlock::IS_USABLE() { return false; }
    } // namespace gl
} // namespace hive

#endif