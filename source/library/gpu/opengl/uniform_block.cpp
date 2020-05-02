#pragma once

#include "./uniform_block.hpp"

namespace hive
{

    namespace gl
    {

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
