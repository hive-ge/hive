#pragma once

#include "gpu/opengl/glwrap.h"
#include "gpu/opengl/input.h"
#include "gpu/opengl/output.h"
#include "gpu/opengl/uniform.h"
#include "gpu/opengl/uniform_block.h"
#include "primitive/log.h"
#include "program.h"
#include <string>
#include <unordered_map>
#include <vector>


namespace hive
{

    namespace gl
    {

        struct SmartGLPipeline final : SmartGLint {

            std::vector<SmartGLProgram> programs;

          protected:
            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLPipeline() : SmartGLint(SmartGLType::Program) {}

            SmartGLPipeline(GLuint program_pointer, bool ISREADY,
                            std::vector<SmartGLProgram> programs)
                : SmartGLint(SmartGLType::Program, program_pointer, ISREADY){};

            ~SmartGLPipeline() { decreaseReferenceCount(); }

            SmartGLPipeline(const SmartGLPipeline & obj) : SmartGLint(obj) {}

            virtual void use();

            virtual void release() override;

            virtual bool IS_USABLE() override;
        };

        SmartGLPipeline createPipeline(std::vector<SmartGLProgram> programs)
        {
            unsigned pipeline;

            SmartGLPipeline smpipeline;

            glGenProgramPipelines(1, &pipeline);

            uint stage_bits = 0;

            if (pipeline > 0) {
                for (auto program : programs) {
                    if (program.IS_USABLE() && program.PIPELINEABLE()) {
                        uint program_stages = 0;
                        if (program.HAVE_COMP()) program_stages |= GL_COMPUTE_SHADER_BIT;
                        if (program.HAVE_FRAG()) program_stages |= GL_FRAGMENT_SHADER_BIT;
                        if (program.HAVE_EVAL()) program_stages |= GL_TESS_EVALUATION_SHADER_BIT;
                        if (program.HAVE_VERT()) program_stages |= GL_VERTEX_SHADER_BIT;
                        if (program.HAVE_GEOM()) program_stages |= GL_GEOMETRY_SHADER_BIT;
                        if (program.HAVE_TESS()) program_stages |= GL_TESS_CONTROL_SHADER_BIT;

                        if (program_stages & stage_bits) {
                            // TODO Some type of overlap error, or warning?
                        }

                        glUseProgramStages(pipeline, program_stages, program.gli());

                        stage_bits |= program_stages;
                    }
                }

                smpipeline = SmartGLPipeline(pipeline, true, programs);
            }

            return smpipeline;
        }

        bool SmartGLPipeline::IS_USABLE() { return IS_READY && pointer > -1; }

        void SmartGLPipeline::use()
        {
            if (!IS_READY) {
                // SKCH_ERROR("Attempt to use gl program that is not ready to be used.");
            } else if (bound_program != pointer) {
                glUseProgram(pointer);
                // glBindVertexArray(vertex_array_object);
                bound_program = pointer;
            }
        }

        void SmartGLPipeline::release()
        {
            if (pointer < 0) return;
            bound_program = -1;
            glUseProgram(0);
            glBindVertexArray(0);
        }

        void SmartGLPipeline::deleteUnderlyingGLResource()
        {
            if (pointer > -1) {
                // glDeleteVertexArrays(1, &vertex_array_object);
                unsigned pipe = pointer;
                glDeleteProgramPipelines(1, &pipe);
            }
            IS_READY        = false;
            reference_count = NULL;
        }
    } // namespace gl
} // namespace hive