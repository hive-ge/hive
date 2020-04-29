#include "./drone.hpp"
#include "../boss.hpp"
#include <iostream>

using namespace hive;

void * hive::Drone::boss = nullptr;

//*
Drone * Drone::construct()
{
    if (!boss) return nullptr;

    Drone * drone = static_cast<BigBadBossA *>(boss)->createDrone();

    std::cout << "drone pointer" << (unsigned long long)(drone) << std::endl;

    return drone;
}
//*/

Drone * Drone::operator->()
{
    return &(static_cast<BigBadBossA *>(boss)->getDrone((unsigned long long)this));
}


void Prop::connect(Drone * drone)
{
    if (this->drone)
        if (this->drone == drone) return;

    drone->connect(this);
}

void Prop::disconnect()
{
    if (this->drone) this->drone->disconnect(this);
}


void hive::Drone::signalUpdate()
{
    if (flags) static_cast<BigBadBossA *>(boss)->prepareDroneUpdate(this);
}