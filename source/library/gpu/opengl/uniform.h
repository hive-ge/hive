#pragma once

#include "gpu/opengl/glwrap.h"

namespace hive
{

    namespace gl
    {

        void SmartGLUniform::deleteUnderlyingGLResource() {}

        void SmartGLUniform::use() {}

        void SmartGLUniform::release() {}

        bool SmartGLUniform::IS_USABLE() { return false; }
    } // namespace gl
} // namespace hive