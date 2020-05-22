#pragma once

#include "./program.hpp"

namespace hive
{
    using namespace gl;




    Program createShaderProgram(ProgramDefinition program_definition, const bool SEPERABLE = false)
    {

        Program smprogram;

        int vert = 0, frag = 0, tess = 0, comp = 0, eval = 0, geom = 0;

        const int shader_bits =
            ((program_definition.vert ? 1 : 0) << 1) | ((program_definition.frag ? 1 : 0) << 2) |
            ((program_definition.geom ? 1 : 0) << 3) | ((program_definition.tess ? 1 : 0) << 4) |
            ((program_definition.eval ? 1 : 0) << 5) | ((program_definition.comp ? 1 : 0) << 6) | 0;

        if (HAVE_COMP(shader_bits)) {
            if (shader_bits ^ (1 << 6)) {
                // Can't have compute shader with any other shaders.
            }

            comp = createShader(*program_definition.comp, GL_COMPUTE_SHADER, "compute shader");

        } else {
            vert = HAVE_VERT(shader_bits)
                       ? createShader(*program_definition.vert, GL_VERTEX_SHADER, "vertex shader")
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

            auto shader_buffers = new std::unordered_map<std::string, SmartGLShaderBufferBlock>;
            loadResource<SmartGLShaderBufferBlock>(GL_SHADER_STORAGE_BLOCK, program,
                                                   *shader_buffers);

            // Shaders do not need to be attached to the program anymore
            if (vert) glDetachShader(program, vert);
            if (frag) glDetachShader(program, frag);
            if (geom) glDetachShader(program, geom);
            if (eval) glDetachShader(program, eval);
            if (tess) glDetachShader(program, tess);
            if (comp) glDetachShader(program, comp);

            smprogram = Program(program, (result == GL_TRUE) ? true : false, SEPERABLE, shader_bits,
                                inputs, outputs, uniforms, uniform_blocks, shader_buffers);
        } else {
            // pullup any error
            DEBUG_META(reportShaderProgramErrors(program));
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

    bool Program::IS_USABLE() { return IS_READY && pointer > -1; }

    void Program::use()
    {
        if (!IS_READY) {
            // SKCH_ERROR("Attempt to use gl program that is not ready to be used.");
        } else if (bound_program != pointer) {
            glUseProgram(pointer);
            glBindVertexArray(vertex_array_object);
            bound_program = pointer;
        }
    }

    void Program::release()
    {
        if (pointer < 0) return;
        bound_program = -1;
        glUseProgram(0);
        glBindVertexArray(0);
    }

    void Program::deleteUnderlyingGLResource()
    {
        if (pointer > -1) {
            if (inputs != NULL) delete (inputs);
            if (outputs != NULL) delete (outputs);
            if (uniformblocks != NULL) delete (uniformblocks);
            if (uniforms != NULL) delete (uniforms);
            glDeleteVertexArrays(1, &vertex_array_object);
            glDeleteProgram(pointer);
        }
        IS_READY = false;
    }
} // namespace hive