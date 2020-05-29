#include "include/hive.h"
#include <iostream>

using namespace hive;

void Prop::connect(DronePointer drone)
{

    if (!drone->props) {
        drone->props = this;
    } else {
        Prop * current = drone->props;

        while (current->next) current = current->next;

        current->next = this;

        prev = current;
    }

    onConnect(drone);
}

void Prop::disconnect(DronePointer drone) { onConnect(drone); }


