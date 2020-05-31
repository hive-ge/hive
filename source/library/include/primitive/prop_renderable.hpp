#pragma once

#include <vector>

#include "include/primitive/core_drone.hpp"
#include "include/primitive/core_math.hpp"
#include "include/primitive/core_prop.hpp"


namespace hive
{
    using namespace hive::math;

    static const DroneFlagTemplate<2> DRONE_FLAG_CAN_RENDER;
    static const DroneFlagTemplate<3> DRONE_FLAG_NEED_RENDER_UPDATE;

    /**::HIVE DRONE_PROP::*/
    struct RenderableProp : Prop {
        CONSTRUCT_PROP(RenderableProp, "PropRender");

      private:
        bool CAN_RENDER = false;

      public:
        RenderableProp() : Prop() {}

        ~RenderableProp() {}

        /** Implemented in active GPU library */
        bool SET_RENDER_STATE(bool FLAG);

        bool ALLOW_RENDER() const;
    };

    REGISTER_PROP(RenderableProp)
} // namespace hive