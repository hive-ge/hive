#pragma once

#include "gpu/opengl/attribute.h"
#include "gpu/opengl/glwrap.h"
#include "gpu/opengl/uniform.h"


namespace hive
{

    namespace gl
    {
        struct SmartGLProgram final : SmartGLint {

          private:
            std::unordered_map<std::string, SmartGLAttribute> * attributes = NULL;

            std::unordered_map<std::string, SmartGLUniform> * uniforms = NULL;

            GLuint vertex_array_object = 0;

          protected:
            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLProgram() : SmartGLint(SmartGLType::Program) {}

            SmartGLProgram(GLuint program_pointer, bool ISREADY,
                           std::unordered_map<std::string, SmartGLAttribute> * attr = NULL,
                           std::unordered_map<std::string, SmartGLUniform> * uni    = NULL)
                : SmartGLint(SmartGLType::Program, program_pointer, ISREADY), attributes(attr),
                  uniforms(uni)
            {
                glGenVertexArrays(1, &vertex_array_object);
            };

            ~SmartGLProgram() { decreaseReferenceCount(); }

            SmartGLProgram(const SmartGLProgram & obj) : SmartGLint(obj)
            {
                attributes          = obj.attributes;
                uniforms            = obj.uniforms;
                vertex_array_object = obj.vertex_array_object;
            }

            virtual void use();

            virtual void release() override;

            virtual bool IS_USABLE() override;

            inline SmartGLUniform getUniform(std::string str)
            {
                if (uniforms != NULL) return (*uniforms)[str];
                return SmartGLUniform(); // Null unused object
            }

            inline SmartGLAttribute getAttribute(std::string str)
            {
                if (attributes != NULL) return (*attributes)[str];
                return SmartGLAttribute(); // Null unused object;
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
            std::cout << "Compiling vertex shader."
                      << std::endl; // <================================================

            glShaderSource(vertShader, 1, &vertShaderSrc, NULL);

            glCompileShader(vertShader);

            // Check vertex shader
            glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);

            glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);

            glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);

            std::cout << &vertShaderError[0]
                      << std::endl; // <================================================

            // Compile fragment shader
            std::cout << "Compiling fragment shader."
                      << std::endl; // <================================================
            glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
            glCompileShader(fragShader);

            // Check fragment shader
            glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);

            glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> fragShaderError((logLength > 1) ? logLength : 1);

            glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);

            std::cout << &fragShaderError[0]
                      << std::endl; // <================================================

            std::cout << "Linking program"
                      << std::endl; // <================================================

            GLuint program = glCreateProgram();

            glAttachShader(program, vertShader);

            glAttachShader(program, fragShader);

            glLinkProgram(program);

            glValidateProgram(program);

            glGetProgramiv(program, GL_LINK_STATUS, &result);

            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<char> programError((logLength > 1) ? logLength : 1);

            glGetProgramInfoLog(program, logLength, NULL, &programError[0]);

            std::cout << &programError[0]
                      << std::endl; // <================================================

            std::cout << "Program link result: " << result
                      << std::endl; // <================================================

            // glDeleteShader(vertShader);
            // glDeleteShader(fragShader);

            std::unordered_map<std::string, SmartGLAttribute> * attributes = NULL;

            std::unordered_map<std::string, SmartGLUniform> * uniforms = NULL;

            // extract program information: Attributes and Uniforms

            if (result == GL_TRUE) {

                attributes = new std::unordered_map<std::string, SmartGLAttribute>;

                uniforms = new std::unordered_map<std::string, SmartGLUniform>;

                glUseProgram(program);

                int number_of_active_attributes;
                glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &number_of_active_attributes);

                for (auto i = 0; i < number_of_active_attributes; i++) {

                    char name[50];

                    int text_length, size;

                    GLenum type;

                    glGetActiveAttrib(program, i, 50, &text_length, &size, &type, name);

                    std::cout << "Program has attribute at index " << i << " named " << name
                              << " of type " // <================================================
                              << getGLSLTypeBase(type) << " with a size of "
                              << getGLSLTypeSize(type) * size << " whose location is "
                              << glGetAttribLocation(program, name) << std::endl;
                    ;
                    (*attributes)[name] =
                        SmartGLAttribute(glGetAttribLocation(program, name), getGLSLTypeSize(type),
                                         size, getGLSLTypeBase(type), true);
                    SmartGLAttribute test = (*attributes)["index"];
                }

                int number_of_active_uniforms;

                glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &number_of_active_uniforms);

                GLint * params_GL_UNIFORM_TYPE = new GLint[number_of_active_uniforms * 2];

                GLint * params_GL_UNIFORM_SIZE = params_GL_UNIFORM_TYPE + number_of_active_uniforms;

                GLuint indices[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                                    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};

                glGetActiveUniformsiv(program, number_of_active_uniforms, indices, GL_UNIFORM_TYPE,
                                      params_GL_UNIFORM_TYPE);

                glGetActiveUniformsiv(program, number_of_active_uniforms, indices, GL_UNIFORM_SIZE,
                                      params_GL_UNIFORM_SIZE);

                for (auto i = 0; i < number_of_active_uniforms; i++) {

                    char name[50];

                    int text_length;

                    glGetActiveUniformName(program, i, 50, &text_length, name);

                    //	std::cout << "Program has uniform at index " << i << " named " << name << "
                    // of type " //
                    //<================================================
                    //		<< getGLSLTypeSize(params_GL_UNIFORM_TYPE[i]) << " with a size of "
                    //<<
                    // params_GL_UNIFORM_SIZE[i] << " whose location is " <<
                    // glGetUniformLocation(program, name) << std::endl;
                    (*uniforms)[name] = SmartGLUniform(glGetUniformLocation(program, name),
                                                       getGLSLTypeSize(params_GL_UNIFORM_TYPE[i]),
                                                       params_GL_UNIFORM_SIZE[i], true);
                }

                delete[] params_GL_UNIFORM_TYPE;
            }

            return SmartGLProgram(program, (result == GL_TRUE) ? true : false, attributes,
                                  uniforms);
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
            glUseProgram(0);
        }

        void SmartGLProgram::deleteUnderlyingGLResource()
        {
            if (pointer > -1) {
                if (attributes != NULL) delete (attributes);
                if (uniforms != NULL) delete (uniforms);
                glDeleteVertexArrays(1, &vertex_array_object);
                glDeleteProgram(pointer);
            }
            IS_READY        = false;
            reference_count = NULL;
        }
    } // namespace gl
} // namespace hive