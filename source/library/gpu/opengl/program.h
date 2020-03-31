#pragma once

#include "gpu/opengl/glwrap.h"
#include "gpu/opengl/input.h"
#include "gpu/opengl/output.h"
#include "gpu/opengl/uniform.h"
#include "gpu/opengl/uniform_block.h"
#include "primitive/log.h"
#include <GL/gl.h>
#include <string>
#include <unordered_map>

#ifdef HIVE_DEBUG
#define DEBUG_META(data) data;
#else
#define DEBUG_META(data) ;
#endif

namespace hive
{

    namespace gl
    {

#define VERT_BIT 1 << 1
#define FRAG_BIT 1 << 2
#define GEOM_BIT 1 << 3
#define EVAL_BIT 1 << 4
#define TESS_BIT 1 << 5
#define COMP_BIT 1 << 6

        bool HAVE_VERT(const int bits) { return (bits & VERT_BIT) > 0; }
        bool HAVE_FRAG(const int bits) { return (bits & FRAG_BIT) > 0; }
        bool HAVE_GEOM(const int bits) { return (bits & GEOM_BIT) > 0; }
        bool HAVE_EVAL(const int bits) { return (bits & EVAL_BIT) > 0; }
        bool HAVE_TESS(const int bits) { return (bits & TESS_BIT) > 0; }
        bool HAVE_COMP(const int bits) { return (bits & COMP_BIT) > 0; }

        struct ProgramDefinition {
            std::string * vert = nullptr;
            std::string * frag = nullptr;
            std::string * comp = nullptr;
            std::string * tess = nullptr;
            std::string * eval = nullptr;
            std::string * geom = nullptr;
        };

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

            bool SEPERABLE = false;

            int shader_bits = 0;

          protected:
            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLProgram() : SmartGLint(SmartGLType::Program) {}

            SmartGLProgram(
                GLuint program_pointer, bool ISREADY, bool SEPERABLE, int shader_bits,
                std::unordered_map<std::string, SmartGLInput> * inputs               = NULL,
                std::unordered_map<std::string, SmartGLOutput> * outputs             = NULL,
                std::unordered_map<std::string, SmartGLUniform> * uni                = NULL,
                std::unordered_map<std::string, SmartGLUniformBlock> * uniformblocks = NULL)
                : SmartGLint(SmartGLType::Program, program_pointer, ISREADY), inputs(inputs),
                  outputs(outputs), uniforms(uni), uniformblocks(uniformblocks),
                  SEPERABLE(SEPERABLE), shader_bits(shader_bits)
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

            inline bool HAVE_VERT() { return (shader_bits & VERT_BIT) > 0; }
            inline bool HAVE_FRAG() { return (shader_bits & FRAG_BIT) > 0; }
            inline bool HAVE_GEOM() { return (shader_bits & GEOM_BIT) > 0; }
            inline bool HAVE_EVAL() { return (shader_bits & EVAL_BIT) > 0; }
            inline bool HAVE_TESS() { return (shader_bits & TESS_BIT) > 0; }
            inline bool HAVE_COMP() { return (shader_bits & COMP_BIT) > 0; }
            inline bool PIPELINEABLE() { return SEPERABLE; }

            inline int numberOfLinkedShaders()
            {
                return HAVE_VERT() + HAVE_FRAG() + HAVE_GEOM() + HAVE_EVAL() + HAVE_TESS() +
                       HAVE_COMP();
            }

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

        GLuint createShader(std::string & str, GLenum shader_type, const char * info_label)
        {
            GLuint shader = glCreateShader(shader_type);

            if (shader > 0) { // Must be a non-zero value

                const char * shader_source = str.c_str();

                glShaderSource(shader, 1, &shader_source, NULL);

                glCompileShader(shader);

                // Checkout the shader bro!
                int result, info_length;

                glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);

                std::vector<char> shader_log(info_length > 1 ? info_length : 1);

                glGetShaderInfoLog(shader, info_length, NULL, &shader_log[0]);

                DEBUG_META(__LOG(&shader_log[0]););

                if (result == GL_TRUE) {
                    DEBUG_META(__LOG("Compiled " + std::to_string(*info_label)));
                } else {
                    DEBUG_META(__LOG("" + std::to_string(*info_label) + "failed to load \n ");
                               __LOG(&shader_log[0]);)
                }
            } else {
                DEBUG_META(__LOG("Failed to generate " + std::to_string(*info_label)));
            }

            return shader;
        };


        SmartGLProgram createShaderProgram(ProgramDefinition program_definition,
                                           const bool SEPERABLE = false)
        {

            SmartGLProgram smprogram;

            int vert = 0, frag = 0, tess = 0, comp = 0, eval = 0, geom = 0;

            // Compile vertex shader
            __LOG("Compiling vertex shader."); // <================================================

            const int shader_bits = ((program_definition.vert ? 1 : 0) << 1) |
                                    ((program_definition.frag ? 1 : 0) << 2) |
                                    ((program_definition.geom ? 1 : 0) << 3) |
                                    ((program_definition.tess ? 1 : 0) << 4) |
                                    ((program_definition.eval ? 1 : 0) << 5) |
                                    ((program_definition.comp ? 1 : 0) << 6) | 0;

            if (HAVE_COMP(shader_bits)) {
                if (shader_bits ^ (1 << 6)) {
                    // Can't have compute shader with any other shaders.
                }

                comp = createShader(*program_definition.comp, GL_COMPUTE_SHADER, "compute shader");

            } else {
                vert = HAVE_VERT(shader_bits) ? createShader(*program_definition.vert,
                                                             GL_VERTEX_SHADER, "vertex shader")
                                              : 0;
                frag = HAVE_FRAG(shader_bits) ? createShader(*program_definition.frag,
                                                             GL_FRAGMENT_SHADER, "fragment shader")
                                              : 0;
                geom = HAVE_GEOM(shader_bits) ? createShader(*program_definition.geom,
                                                             GL_GEOMETRY_SHADER, "geometry shader")
                                              : 0;
                tess = HAVE_TESS(shader_bits)
                           ? createShader(*program_definition.tess, GL_TESS_CONTROL_SHADER,
                                          "tessation control shader")
                           : 0;
                eval = HAVE_EVAL(shader_bits)
                           ? createShader(*program_definition.eval, GL_TESS_EVALUATION_SHADER,
                                          "tessation evaluation shader")
                           : 0;
            }

            GLuint program = glCreateProgram(), SHADERS_ACCEPTABLE = true;

            if (HAVE_VERT(shader_bits)) {
                if (vert)
                    glAttachShader(program, vert);
                else
                    SHADERS_ACCEPTABLE = false;
            }

            if (HAVE_FRAG(shader_bits)) {
                if (frag)
                    glAttachShader(program, frag);
                else
                    SHADERS_ACCEPTABLE = false;
            }

            if (HAVE_GEOM(shader_bits)) {
                if (geom)
                    glAttachShader(program, geom);
                else
                    SHADERS_ACCEPTABLE = false;
            }

            if (HAVE_EVAL(shader_bits)) {
                if (eval)
                    glAttachShader(program, eval);
                else
                    SHADERS_ACCEPTABLE = false;
            }

            if (HAVE_TESS(shader_bits)) {
                if (tess)
                    glAttachShader(program, tess);
                else
                    SHADERS_ACCEPTABLE = false;
            }

            if (HAVE_COMP(shader_bits)) {
                if (comp)
                    glAttachShader(program, comp);
                else
                    SHADERS_ACCEPTABLE = false;
            }

            if (SEPERABLE) glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);

            // Link our program
            glLinkProgram(program);

            int result;

            // Get the compiled status
            glGetProgramiv(program, GL_LINK_STATUS, &result);

            if (result == GL_TRUE && SHADERS_ACCEPTABLE) {
                glUseProgram(program);

                auto inputs = new std::unordered_map<std::string, SmartGLInput>;
                loadResource<SmartGLInput>(GL_PROGRAM_INPUT, program, *inputs);

                auto outputs = new std::unordered_map<std::string, SmartGLOutput>;
                loadResource<SmartGLOutput>(GL_PROGRAM_OUTPUT, program, *outputs);

                auto uniforms = new std::unordered_map<std::string, SmartGLUniform>;
                loadResource<SmartGLUniform>(GL_UNIFORM, program, *uniforms);

                auto uniform_blocks = new std::unordered_map<std::string, SmartGLUniformBlock>;
                loadResource<SmartGLUniformBlock>(GL_UNIFORM_BLOCK, program, *uniform_blocks);

                // Shaders do not need to be attached to the program anymore
                if (vert) glDetachShader(program, vert);
                if (frag) glDetachShader(program, frag);
                if (geom) glDetachShader(program, geom);
                if (eval) glDetachShader(program, eval);
                if (tess) glDetachShader(program, tess);
                if (comp) glDetachShader(program, comp);

                smprogram = SmartGLProgram(program, (result == GL_TRUE) ? true : false, SEPERABLE,
                                           shader_bits, inputs, outputs, uniforms, uniform_blocks);
            } else {
                // pullup any error
                DEBUG_META(reportShaderProgramErrors());
                clearErrors();
                glDeleteProgram(program);
            }

            // Delete shaders - Deferred 'till program deletion
            if (vert) glDeleteShader(vert);
            if (frag) glDeleteShader(frag);
            if (geom) glDeleteShader(geom);
            if (eval) glDeleteShader(eval);
            if (tess) glDeleteShader(tess);
            if (comp) glDeleteShader(comp);

            return smprogram;
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

        void reportShaderProgramErrors()
        {
            int err = glGetError();

            do {
                // TODO - Make this more informative.
                __LOG("GL_INVALID_VALUE is generated if either program or shader is not a value "
                      "generated by OpenGL.\n"
                      "GL_INVALID_OPERATION is generated if program is not a program object.\n"
                      "GL_INVALID_OPERATION is generated if shader is not a shader object.\n"
                      "GL_INVALID_OPERATION is generated if shader is already attached to "
                      "program.\n");

                err = glGetError();
            } while (err);
        }
    } // namespace gl
} // namespace hive