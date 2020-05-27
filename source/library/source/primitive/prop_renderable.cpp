#include "include/hive.h"

#include <iostream>

namespace hive
{
    /** Implemented in active GPU library */
    bool RenderableProp::SET_RENDER_STATE(bool FLAG)
    {
        std::cout << "Setting drone flag renderable" << FLAG << std::endl;

        if (CAN_RENDER != FLAG) {


            CAN_RENDER = FLAG;


            if (FLAG)
                drone->flag |= DRONE_FLAG_CAN_RENDER;
            else
                drone->flag ^= DRONE_FLAG_CAN_RENDER;

            drone->flag |= DRONE_FLAG_NEED_RENDER_UPDATE | DRONE_FLAG_CAN_RENDER;

            sendMessage("REN_BOSSES_TEST", "U_DRONES");
        }

        return CAN_RENDER;
    };

    bool RenderableProp::ALLOW_RENDER() const { return CAN_RENDER; };

    void RenderableProp::onConnect(DronePointer dr) { drone = dr; }
} // namespace hive