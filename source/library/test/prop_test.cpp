
#include "includes.hpp"
#include <hive.h>

using namespace hive;

int main(int arg_ct, char ** args)
{
    // Create pool for drone's and prop's
    hive::DroneDataPool pool(120);

    Drone::Ref drone    = pool.createObjectReturnRef<ImageProp>();
    ImageProp::Ref prop = pool.createObjectReturnRef<ImageProp>();

    prop->setTag("test");

    ASSERT(prop->tag == StringHash64("test"))
    ASSERT(prop->tag == "test")

    drone->connect(prop);

    // pool->group([drone]);

    SUCCESS();
}