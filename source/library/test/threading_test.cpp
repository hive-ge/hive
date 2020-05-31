
#include "includes.hpp"
#include <hive.h>

using namespace hive;

hive::DataPool hive::general_data_pool(4096);

int main(int arg_ct, char ** args)
{

    // Create pool for drone's and prop's
    hive::DroneDataPool pool(120);

    Drone::Ref drone                = pool.createObjectReturnRef<Drone>();
    ImageProp::Ref prop             = pool.createObjectReturnRef<ImageProp>();
    RenderableProp::Ref render_prop = pool.createObjectReturnRef<RenderableProp>();
    ImageProp::Ref prop3            = pool.createObjectReturnRef<ImageProp>();
    Drone::Ref drone2               = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone3               = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone4               = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone5               = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone6               = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone7               = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone8               = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone9               = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone10              = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone11              = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone12              = pool.createObjectReturnRef<Drone>();

    RenderableProp::Ref render_prop3 = pool.createObjectReturnRef<RenderableProp>();

    // Access Same drone data at the same time.


    SUCCESS();
}