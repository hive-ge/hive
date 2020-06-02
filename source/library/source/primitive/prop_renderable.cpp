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
                drone->setFlag(DRONE_FLAG_CAN_RENDER | DRONE_FLAG_NEED_RENDER_UPDATE);
            else
                drone->unsetFlag(DRONE_FLAG_CAN_RENDER);

            sendMessage("REN_BOSSES_TEST", "U_DRONES");
        }

        return CAN_RENDER;
    };


    bool RenderableProp::ALLOW_RENDER() const { return CAN_RENDER; };
} // namespace hive