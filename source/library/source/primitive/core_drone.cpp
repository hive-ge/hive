#include "include/hive.h"

#include <iostream>

using namespace hive;

BigBadBoss * hive::Drone::boss = nullptr;

//*
Drone * Drone::construct()
{
    if (!boss) return nullptr;

    Drone * drone = boss->createDrone();

    return drone;
}
//*/
void hive::Drone::connect(Prop * prop) { prop->connect(this_ptr); };

void hive::Drone::disconnect(Prop * prop) { prop->connect(this_ptr); };
