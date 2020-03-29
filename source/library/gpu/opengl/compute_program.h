#pragma once

#include "gpu/opengl/attribute.h"
#include "gpu/opengl/glwrap.h"
#include "gpu/opengl/uniform.h"
#include "primitive/math/vec2.h"
#include "primitive/math/vec3.h"


namespace hive
{

    namespace gl
    {
        struct SmartGLComputeProgram final : SmartGLint {

          private:
            math::vec3i work_groups = {1, 1, 1};

          protected:
            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLComputeProgram() : SmartGLint(SmartGLType::Program) {}

            SmartGLComputeProgram(GLuint program_pointer, bool ISREADY,
                                  std::unordered_map<std::string, SmartGLAttribute> * attr = NULL,
                                  std::unordered_map<std::string, SmartGLUniform> * uni    = NULL)
                : SmartGLint(SmartGLType::Program, program_pointer, ISREADY)
            {
                int maj, min;
                if (!checkGLVersion(maj, min)) {
                    __ERROR(
                        "ERROR creating compute shader. Expected GL version 4.3 or higher. Got " +
                        std::to_string(maj) + "." + std::to_string(min));
                }
            };

            ~SmartGLComputeProgram() { decreaseReferenceCount(); }

            SmartGLComputeProgram(const SmartGLComputeProgram & obj) : SmartGLint(obj) {}

            void setWorkGroups(int x, int y, int z)
            {

                math::vec3i max;

                glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, &max.x);

                if (x > max.x)
                    __ERROR("ERROR setting compute shader work groups. Dimension X cannot be "
                            "higher than " +
                            std::to_string(max.x) + ". Attempted to set value to " +
                            std::to_string(x));

                if (y > max.y)
                    __ERROR("ERROR setting compute shader work groups. Dimension Y cannot be "
                            "higher than " +
                            std::to_string(max.y) + ". Attempted to set value to " +
                            std::to_string(y));

                if (z > max.z)
                    __ERROR("ERROR setting compute shader work groups. Dimension Z cannot be "
                            "higher than " +
                            std::to_string(max.z) + ". Attempted to set value to " +
                            std::to_string(z));

                work_groups = math::vec3i(x, y, z);
            }

            virtual void use();

            virtual void release() override;

            virtual bool IS_USABLE() override;

            virtual void compute()
            {
                glDispatchCompute(work_groups.x, work_groups.y, work_groups.z);
            }
        };

        SmartGLComputeProgram createComputeShaderProgram(std::string vector_shader,
                                                         std::string frag_shader)
        {

            GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);

            // Read shaders
            const char * computeShaderSrc = vector_shader.c_str();

            GLint result = GL_FALSE;
            int logLength;

            // Compile compute shader
            std::cout << "Compiling compute shader."
                      << std::endl; // <================================================

            glShaderSource(computeShader, 1, &computeShaderSrc, NULL);

            glCompileShader(computeShader);

            // Check compute shader
            glGetShaderiv(computeShader, GL_COMPILE_STATUS, &result);

            glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> computeShaderError((logLength > 1) ? logLength : 1);

            glGetShaderInfoLog(computeShader, logLength, NULL, &computeShaderError[0]);

            std::cout << &computeShaderError[0]
                      << std::endl; // <================================================

            std::cout << "Linking program"
                      << std::endl; // <================================================

            GLuint program = glCreateProgram();

            glAttachShader(program, computeShader);

            glLinkProgram(program);

            glValidateProgram(program);

            glGetProgramiv(program, GL_LINK_STATUS, &result);

            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> programError((logLength > 1) ? logLength : 1);

            glGetProgramInfoLog(program, logLength, NULL, &programError[0]);

            std::cout << &programError[0]
                      << std::endl; // <================================================

            std::cout << "Compute Program link result: " << result
                      << std::endl; // <================================================

            // glDeleteShader(computeShader);
            // glDeleteShader(fragShader);

            std::unordered_map<std::string, SmartGLAttribute> * attributes = NULL;

            std::unordered_map<std::string, SmartGLUniform> * uniforms = NULL;

            // extract program information: Attributes and Uniforms

            if (result == GL_TRUE) {

                glUseProgram(program);
            }

            return SmartGLComputeProgram(program, (result == GL_TRUE) ? true : false);
        }

        bool SmartGLComputeProgram::IS_USABLE() { return IS_READY && pointer > -1; }

        void SmartGLComputeProgram::use()
        {
            if (!IS_READY) {
                // SKCH_ERROR("Attempt to use gl program that is not ready to be used.");
            } else if (bound_program != pointer) {
                glUseProgram(pointer);
                bound_program = pointer;
            }
        }

        void SmartGLComputeProgram::release()
        {
            if (pointer < 0) return;
            glUseProgram(0);
        }

        void SmartGLComputeProgram::deleteUnderlyingGLResource()
        {
            if (pointer > -1) glDeleteProgram(pointer);
            IS_READY        = false;
            reference_count = NULL;
        }
    } // namespace gl
} // namespace hive