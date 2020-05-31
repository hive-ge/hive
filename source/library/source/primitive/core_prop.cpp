#include "hive.h"
#include <iostream>

using namespace hive;

void Prop::disconnect(Drone::Ref drone) { drone->disconnect(DroneDataPool::getReference(this)); }

void Prop::connect(Drone::Ref new_drone) { new_drone->connect(DroneDataPool::getReference(this)); }
