#include "include/hive.h"

#include <iostream>

namespace hive
{
    /** Implemented in active GPU library */
    bool RenderableProp::SET_RENDER_STATE(bool FLAG)
    {
        if (CAN_RENDER != FLAG) {

            CAN_RENDER = FLAG;

            sendMessage("REN_BOSSES_TEST", "U_DRONES");
        }

        return CAN_RENDER;
    };

    bool RenderableProp::ALLOW_RENDER() { return CAN_RENDER; };
} // namespace hive