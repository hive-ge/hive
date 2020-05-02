#pragma once

#include "./shader_block.hpp"

namespace hive
{

    namespace gl
    {
        void SmartGLShaderBufferBlock::deleteUnderlyingGLResource() {}

        void SmartGLShaderBufferBlock::bind(VRAMBuffer & buffer, unsigned binding_point,
                                            unsigned offset, unsigned size)
        {
            if (buffer_binding < 0) return;

            if (size == 0) size = buffer.size();

            buffer.useRange(SHADER_STORAGE_BUFFER, binding_point, offset, size);
        }

        void SmartGLShaderBufferBlock::bind(VRAMBuffer & buffer, unsigned offset, unsigned size)
        {
            if (buffer_binding < 0) return;

            if (size == 0) size = buffer.size();

            buffer.useRange(SHADER_STORAGE_BUFFER, buffer_binding, offset, size);
        }


        void SmartGLShaderBufferBlock::release() {}

        bool SmartGLShaderBufferBlock::IS_USABLE() { return false; }
    } // namespace gl
} // namespace hive