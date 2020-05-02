#pragma once

#include "gpu/opengl/glwrap.hpp"
#include "gpu/opengl/input.hpp"
#include "gpu/opengl/output.hpp"
#include "gpu/opengl/uniform.hpp"
#include "gpu/opengl/uniform_block.hpp"
#include "primitive/log.hpp"
#include "program.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace hive
{

    namespace gl
    {

        struct SmartGLPipeline final : SmartGLint {

            std::vector<Program> programs;

          protected:
            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLPipeline() : SmartGLint(SmartGLType::Program) {}

            SmartGLPipeline(GLuint program_pointer, bool ISREADY, std::vector<Program> programs)
                : SmartGLint(SmartGLType::Program, program_pointer, ISREADY){};

            ~SmartGLPipeline() { decreaseReferenceCount(); }

            SmartGLPipeline(const SmartGLPipeline & obj) : SmartGLint(obj) {}

            virtual void use();

            virtual void release() override;

            virtual bool IS_USABLE() override;
        };

    } // namespace gl
} // namespace hive