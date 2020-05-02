#include "primitive/drone.hpp"
#include "primitive/boss.hpp"
#include <iostream>

using namespace hive;

BigBadBoss * hive::Drone::boss = nullptr;

//*
Drone * Drone::construct()
{
    if (!boss) return nullptr;

    Drone * drone = new Drone();

    boss->createDroneData(drone);

    std::cout << "drone pointer" << (unsigned long long)(drone) << std::endl;

    return drone;
}
//*/

void hive::Drone::signalUpdate()
{
    getBoss()->getDroneData(this);

    if (data->flags) static_cast<BigBadBoss *>(boss)->prepareDroneUpdate(this);
}

void hive::Drone::connect(Prop * prop)
{
    getBoss()->getDroneData(this);

    if (prop->drone) {

        if (prop->drone == data) return;

        if (prop->drone != nullptr) prop->disconnect();
    }

    if (!data->props) {
        data->props = prop;
    } else {
        Prop * current = data->props;

        while (current->next) current = current->next;

        current->next = prop;

        prop->prev = current;
    }

    prop->drone = data;
};