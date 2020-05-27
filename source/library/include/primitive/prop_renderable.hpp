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

    //::HIVE DRONE_PROP
    struct RenderableProp : Prop {

      private:
        bool CAN_RENDER    = false;
        DronePointer drone = 0;

      public:
        static RenderableProp * construct() { return new RenderableProp(); }

        RenderableProp() : Prop("PROP_RENDER", sizeof(RenderableProp)) {}

        ~RenderableProp() {}

        /** Implemented in active GPU library */
        bool SET_RENDER_STATE(bool FLAG);

        bool ALLOW_RENDER() const;

      protected:
        virtual void onConnect(DronePointer drone);
    };
} // namespace hive