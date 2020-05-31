#include "include/hive.h"

#include <iostream>

namespace hive
{


    Drone * Drone::construct()
    {
        if (!global_boss) return nullptr;

        Drone * drone = global_boss->createDrone();

        return drone;
    }

    void hive::Drone::connect(DroneDataHandle prop)
    {

        // TODO: Should lock drone for threading?

        DroneDataPool pool;

        if (prop) {


            if (props == DroneDataHandle::UNDEFINED) {
                props = prop;
            } else {
                Prop::Reference current = props;

                while (current.reinterpret<Prop>()->next != DroneDataHandle::UNDEFINED)
                    current = current.reinterpret<Prop>()->next;

                current->next = prop;
            }
        } else {
            HIVE_DEBUG_WARN("Attempt to connect drone to undefined prop.");
        }
    };

    void hive::Drone::disconnect(DroneDataHandle prop)
    {
        HIVE_DEBUG_WARN("Drone::disconnect not yet implemented!");
    };

    DroneDataHandle hive::Drone::getProp(StringHash64 tag)
    {
        if (props) {

            DroneDataHandle prop = props, root = prop;

            while (prop && !prop.is<Drone>()) {
                Prop::Ref ref = prop;

                if (ref->tag == tag) return ref;

                prop = ref->next;
            }
        }
        return DroneDataHandle();
    }
} // namespace hive
