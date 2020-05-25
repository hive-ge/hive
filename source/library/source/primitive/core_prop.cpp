#include "include/hive.h"
#include <iostream>

using namespace hive;

//*/

void Prop::connect(Drone * drone) { drone->connect(this); }

void Prop::disconnect() {}
