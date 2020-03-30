#pragma once

#include "gpu/opengl/glwrap.h"
#include "gpu/opengl/input.h"
#include "gpu/opengl/output.h"
#include "gpu/opengl/uniform.h"
#include "gpu/opengl/uniform_block.h"
#include "primitive/log.h"
#include <string>
#include <unordered_map>


namespace hive
{

    namespace gl
    {

        template <class ObjectType>
        void loadResource(GLenum Enum, GLenum program,
                          std::unordered_map<std::string, ObjectType> & storage)
        {
            // loadProgramObjects
            int count, name_length;

            char name[50];

            glGetProgramInterfaceiv(program, Enum, GL_ACTIVE_RESOURCES, &count);
            glGetProgramInterfaceiv(program, Enum, GL_MAX_NAME_LENGTH, &name_length);

            for (int i = 0; i < count; i++) {

                glGetProgramResourceName(program, Enum, i, 50, &name_length, name);

                std::string name_string(name);

                storage[name_string] = ObjectType(program, i);
            }
        }

        struct SmartGLProgram final : SmartGLint {

          private:
            std::unordered_map<std::string, SmartGLInput> * inputs               = NULL;
            std::unordered_map<std::string, SmartGLOutput> * outputs             = NULL;
            std::unordered_map<std::string, SmartGLUniform> * uniforms           = NULL;
            std::unordered_map<std::string, SmartGLUniformBlock> * uniformblocks = NULL;

            GLuint vertex_array_object = 0;

          protected:
            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLProgram() : SmartGLint(SmartGLType::Program) {}

            SmartGLProgram(
                GLuint program_pointer, bool ISREADY,
                std::unordered_map<std::string, SmartGLInput> * inputs               = NULL,
                std::unordered_map<std::string, SmartGLOutput> * outputs             = NULL,
                std::unordered_map<std::string, SmartGLUniform> * uni                = NULL,
                std::unordered_map<std::string, SmartGLUniformBlock> * uniformblocks = NULL)
                : SmartGLint(SmartGLType::Program, program_pointer, ISREADY), inputs(inputs),
                  outputs(outputs), uniforms(uni), uniformblocks(uniformblocks)
            {
                glGenVertexArrays(1, &vertex_array_object);
            };

            ~SmartGLProgram() { decreaseReferenceCount(); }

            SmartGLProgram(const SmartGLProgram & obj) : SmartGLint(obj)
            {
                inputs        = obj.inputs;
                outputs       = obj.outputs;
                uniforms      = obj.uniforms;
                uniformblocks = obj.uniformblocks;
            }

            virtual void use();

            virtual void release() override;

            virtual bool IS_USABLE() override;

            inline SmartGLUniform getUniform(std::string str)
            {
                if (uniforms != NULL) return (*uniforms)[str];
                return SmartGLUniform(); // Null unused object
            }

            inline SmartGLInput getInput(std::string str)
            {
                if (inputs != NULL) return (*inputs)[str];
                return SmartGLInput(); // Null unused object;
            }

            inline SmartGLOutput getOutput(std::string str)
            {
                if (outputs != NULL) return (*outputs)[str];
                return SmartGLOutput(); // Null unused object;
            }

            inline SmartGLUniformBlock getUniformBlock(std::string str)
            {
                if (uniformblocks != NULL) return (*uniformblocks)[str];
                return SmartGLUniformBlock(); // Null unused object;
            }
        };

        SmartGLProgram createShaderProgram(std::string vector_shader, std::string frag_shader)
        {

            GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
            GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

            // Read shaders
            const char *vertShaderSrc = vector_shader.c_str(), *fragShaderSrc = frag_shader.c_str();

            GLint result = GL_FALSE;
            int logLength;

            // Compile vertex shader
            __LOG("Compiling vertex shader."); // <================================================

            glShaderSource(vertShader, 1, &vertShaderSrc, NULL);

            glCompileShader(vertShader);

            // Check vertex shader
            glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);

            glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);

            glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);

            __LOG(&vertShaderError[0]);

            // Compile fragment shader
            __LOG("Compiling fragment shader.");

            glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
            glCompileShader(fragShader);

            // Check fragment shader
            glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);

            glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> fragShaderError((logLength > 1) ? logLength : 1);

            glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);

            __LOG(&fragShaderError[0]);


            __LOG("Linking program");


            GLuint program = glCreateProgram();

            glAttachShader(program, vertShader);

            glAttachShader(program, fragShader);

            glLinkProgram(program);

            glValidateProgram(program);

            glGetProgramiv(program, GL_LINK_STATUS, &result);

            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> programError((logLength > 1) ? logLength : 1);

            glGetProgramInfoLog(program, logLength, NULL, &programError[0]);

            __LOG(&programError[0]);

            __LOG("Program link result: " + std::to_string((bool)result));

            if (result == GL_TRUE) {

                glUseProgram(program);

                auto inputs = new std::unordered_map<std::string, SmartGLInput>;
                loadResource<SmartGLInput>(GL_PROGRAM_INPUT, program, *inputs);

                auto outputs = new std::unordered_map<std::string, SmartGLOutput>;
                loadResource<SmartGLOutput>(GL_PROGRAM_OUTPUT, program, *outputs);

                auto uniforms = new std::unordered_map<std::string, SmartGLUniform>;
                loadResource<SmartGLUniform>(GL_UNIFORM, program, *uniforms);

                auto uniform_blocks = new std::unordered_map<std::string, SmartGLUniformBlock>;
                loadResource<SmartGLUniformBlock>(GL_UNIFORM_BLOCK, program, *uniform_blocks);

                return SmartGLProgram(program, (result == GL_TRUE) ? true : false, inputs, outputs,
                                      uniforms, uniform_blocks);
            }

            return SmartGLProgram();
        }

        bool SmartGLProgram::IS_USABLE() { return IS_READY && pointer > -1; }

        void SmartGLProgram::use()
        {
            if (!IS_READY) {
                // SKCH_ERROR("Attempt to use gl program that is not ready to be used.");
            } else if (bound_program != pointer) {
                glUseProgram(pointer);
                glBindVertexArray(vertex_array_object);
                bound_program = pointer;
            }
        }

        void SmartGLProgram::release()
        {
            if (pointer < 0) return;
            bound_program = -1;
            glUseProgram(0);
            glBindVertexArray(0);
        }

        void SmartGLProgram::deleteUnderlyingGLResource()
        {
            if (pointer > -1) {
                if (inputs != NULL) delete (inputs);
                if (outputs != NULL) delete (outputs);
                if (uniformblocks != NULL) delete (uniformblocks);
                if (uniforms != NULL) delete (uniforms);
                glDeleteVertexArrays(1, &vertex_array_object);
                glDeleteProgram(pointer);
            }
            IS_READY        = false;
            reference_count = NULL;
        }
    } // namespace gl
} // namespace hive