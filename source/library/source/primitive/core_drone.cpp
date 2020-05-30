#include "include/hive.h"

#include <iostream>

using namespace hive;


//*
Drone * Drone::construct()
{
    if (!global_boss) return nullptr;

    Drone * drone = global_boss->createDrone();

    return drone;
}
//*/
void hive::Drone::connect(DroneDataHandle prop) { prop.reinterpret<Prop>()->connect(this); };

void hive::Drone::disconnect(DroneDataHandle prop) { prop.reinterpret<Prop>()->disconnect(this); };

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
