#include "include/hive.h"
#include <iostream>

using namespace hive;

void Prop::connect(Drone * drone)
{
    DroneDataPool pool;

    if (drone->props == DroneDataHandle::UNDEFINED) {
        drone->props = pool.getReference(this);
    } else {
        Prop::Reference current = drone->props;

        while (current.reinterpret<Prop>()->next != DroneDataHandle::UNDEFINED)
            current = current.reinterpret<Prop>()->next;

        current->next = pool.getReference(this);
    }
}

void Prop::disconnect(Drone * drone) {}
