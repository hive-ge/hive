#pragma once

#include "gpu/opengl/glwrap.hpp"
#include "gpu/opengl/input.hpp"
#include "gpu/opengl/output.hpp"
#include "gpu/opengl/shader_block.hpp"
#include "gpu/opengl/uniform.hpp"
#include "gpu/opengl/uniform_block.hpp"
#include "primitive/log.hpp"

#include <GL/gl.h>
#include <gl3w/GL/glcorearb.h>
#include <string>
#include <unordered_map>


#define VERT_BIT 1 << 1
#define FRAG_BIT 1 << 2
#define GEOM_BIT 1 << 3
#define EVAL_BIT 1 << 4
#define TESS_BIT 1 << 5
#define COMP_BIT 1 << 6

namespace hive
{
    using namespace gl;

    inline bool HAVE_VERT(const int bits) { return (bits & VERT_BIT) > 0; }
    inline bool HAVE_FRAG(const int bits) { return (bits & FRAG_BIT) > 0; }
    inline bool HAVE_GEOM(const int bits) { return (bits & GEOM_BIT) > 0; }
    inline bool HAVE_EVAL(const int bits) { return (bits & EVAL_BIT) > 0; }
    inline bool HAVE_TESS(const int bits) { return (bits & TESS_BIT) > 0; }
    inline bool HAVE_COMP(const int bits) { return (bits & COMP_BIT) > 0; }

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

    struct Program final : SmartGLint {

      private:
        std::unordered_map<std::string, SmartGLInput> * inputs                    = NULL;
        std::unordered_map<std::string, SmartGLOutput> * outputs                  = NULL;
        std::unordered_map<std::string, SmartGLUniform> * uniforms                = NULL;
        std::unordered_map<std::string, SmartGLUniformBlock> * uniformblocks      = NULL;
        std::unordered_map<std::string, SmartGLShaderBufferBlock> * shaderbuffers = NULL;

        GLuint vertex_array_object = 0;

        bool SEPERABLE = false;

        int shader_bits = 0;

      protected:
        virtual void deleteUnderlyingGLResource() override;

      public:
        Program() : SmartGLint(SmartGLType::Program) {}

        Program(GLuint program_pointer, bool ISREADY, bool SEPERABLE, int shader_bits,
                std::unordered_map<std::string, SmartGLInput> * inputs                    = NULL,
                std::unordered_map<std::string, SmartGLOutput> * outputs                  = NULL,
                std::unordered_map<std::string, SmartGLUniform> * uni                     = NULL,
                std::unordered_map<std::string, SmartGLUniformBlock> * uniformblocks      = NULL,
                std::unordered_map<std::string, SmartGLShaderBufferBlock> * shaderbuffers = NULL)
            : SmartGLint(SmartGLType::Program, program_pointer, ISREADY), inputs(inputs),
              outputs(outputs), uniforms(uni), uniformblocks(uniformblocks),
              shaderbuffers(shaderbuffers), SEPERABLE(SEPERABLE), shader_bits(shader_bits)
        {
            glGenVertexArrays(1, &vertex_array_object);
        };

        ~Program() { decreaseReferenceCount(); }

        Program(const Program & obj) : SmartGLint(obj)
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

        inline SmartGLShaderBufferBlock getShaderBufferBlock(std::string str)
        {
            if (shaderbuffers != NULL) return (*shaderbuffers)[str];
            return SmartGLShaderBufferBlock(); // Null unused object;
        }
    };

    GLuint createShader(std::string & str, GLenum shader_type, const std::string info_label);

    void reportShaderProgramErrors(int program);
} // namespace hive
