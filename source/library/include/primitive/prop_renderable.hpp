#pragma once

#include <vector>

#include "include/primitive/core_math.hpp"
#include "include/primitive/core_prop.hpp"

namespace hive
{
    using namespace hive::math;

    //::HIVE DRONE_PROP
    struct RenderableProp : Prop {

      private:
        bool CAN_RENDER = false;

      public:
        static RenderableProp * construct() { return new RenderableProp(); }

        RenderableProp() : Prop("PROP_RENDER", sizeof(RenderableProp)) {}

        ~RenderableProp() {}

        /** Implemented in active GPU library */
        bool SET_RENDER_STATE(bool FLAG);

        bool ALLOW_RENDER();
    };
} // namespace hive