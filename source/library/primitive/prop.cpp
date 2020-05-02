#include "primitive/prop.hpp"
#include "primitive/drone.hpp"
#include <iostream>

using namespace hive;

//*/

void Prop::connect(Drone * drone) { drone->connect(this); }

void Prop::disconnect() {}
