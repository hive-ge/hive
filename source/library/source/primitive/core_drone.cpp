#include "core_type_information.hpp"
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

    void Drone::connect(DroneDataHandle prop)
    {
        auto ref = DroneDataPool::getReference(this);

        if (DroneDataPool::appendRefToEndOfChain(ref, prop)) {
            prop.reinterpret<Prop>()->drone = ref;
            cache += prop.getType();
        }
    };

    void Drone::disconnect(DroneDataHandle prop)
    {
        HIVE_DEBUG_WARN("Drone::disconnect not yet implemented!");
    };

    DroneDataHandle Drone::getProp(StringHash64 tag) const
    {

        auto prop = DroneDataPool::getReference(this);

        while (prop) {

            if (DroneDataPool::getImplementationData(prop) == tag) return prop;

            prop = DroneDataPool::getNextRef(prop);
        }

        return DroneDataHandle();
    }

    DronePropLU Drone::getCache() const { return cache; }
} // namespace hive
