
#include "includes.hpp"
#include "prop_matrix.hpp"
#include "prop_vector.hpp"
#include <hive.h>

using namespace hive;

hive::DataPool hive::general_data_pool(4096);

int main(int arg_ct, char ** args)
{
    // Create pool for drone's and prop's
    hive::DroneDataPool pool(12000000);

    Drone::Ref drone                      = pool.createObjectReturnRef<Drone>();
    ImageProp::Ref prop                   = pool.createObjectReturnRef<ImageProp>();
    RenderableProp::Ref render_prop       = pool.createObjectReturnRef<RenderableProp>();
    ImageProp::Ref prop3                  = pool.createObjectReturnRef<ImageProp>();
    Drone::Ref drone2                     = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone3                     = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone4                     = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone5                     = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone6                     = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone7                     = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone8                     = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone9                     = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone10                    = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone11                    = pool.createObjectReturnRef<Drone>();
    Drone::Ref drone12                    = pool.createObjectReturnRef<Drone>();
    RenderableProp::Ref render_prop3      = pool.createObjectReturnRef<RenderableProp>();
    unsigned indexA                       = drone12->getRef().getIndex();
    unsigned indexB                       = drone9->getRef().getIndex();
    const unsigned result_of_index_offset = indexA - indexB;

    ASSERT(result_of_index_offset == 3)

    prop->setTag("test");
    render_prop->setTag("render_tag");
    prop3->setTag("test");
    render_prop3->setTag("render_tag");

    ASSERT(prop->getTagHash() == StringHash64("test"))
    ASSERT(prop->getTag() == "test")

    ASSERT(render_prop->getTagHash() == StringHash64("render_tag"))

    std::string st = render_prop->getTag();
    ASSERT(st == "render-tag")

    drone->connect(prop);
    drone->connect(render_prop);

    drone3->connect(prop3);
    drone3->connect(render_prop3);

    // TODO: Move property memory around.
    // TODO: Concurrent access of properties.
    // TODO: Concurrent update of properties.

    // Cache flag comparison
    ASSERT(drone->hasProp(DronePropLU::render));
    ASSERT(drone->hasProp(DronePropLU::image));
    ASSERT(!drone->hasProp(DronePropLU::parent));

    // Lookup Property By Tag
    ASSERT(drone->getProp("test") == prop)
    ASSERT(drone->getProp("render_tag") == render_prop)
    ASSERT(!(bool)drone->getProp("render_greble"))

    // Out of order lookup
    ASSERT(drone3->getProp("test") == prop3)
    ASSERT(drone3->getProp("render_tag") == render_prop3)
    ASSERT(drone3->getProp("test") != prop)
    ASSERT(drone3->getProp("render_tag") == render_prop3)


    // Prop lookup cache
    ASSERT(drone3->getCache() == ImageProp::DroneDataType);
    ASSERT(drone3->getCache() == RenderableProp::DroneDataType);
    ASSERT(drone3->getCache() != MeshProp::DroneDataType);
    ASSERT(drone3->getCache() != RenderLayerProp::DroneDataType);
    ASSERT(drone3->getCache() != GridProp::DroneDataType);
    ASSERT(drone3->getCache() != DoubleMat44Prop::DroneDataType);
    ASSERT(drone3->getCache() != DoubleVec3Prop::DroneDataType);
    ASSERT(drone3->getCache() != Float64Prop::DroneDataType);


    for (int i = 0; i < 10000000; i++) pool.createObjectReturnRef<Drone>();

    std::cout << "\nfor( of : in ) loop " << std::endl;
    timeMeasureStart();
    unsigned long long start = getCPUCycles();
    unsigned found           = 0;
    DroneDataPool::TypeIterator<Drone> it;
    for (auto drone : it) found++;

    unsigned long long end = getCPUCycles();
    timeMeasureEndAndReport();
    std::cout << "A cycles: " << end - start << std::endl;
    std::cout << "A cycle per element: " << (end - start) / 12000000 << std::endl;


    std::cout << "\nfor( init : comp : iter ) loop " << std::endl;
    timeMeasureStart();
    start = getCPUCycles();
    found = 0;
    DroneDataPool::TypeIterator<Drone>::iterator a;
    DroneDataPool::TypeIterator<Drone>::iterator s = DroneDataPool::TypeIterator<Drone>::begin();
    DroneDataPool::TypeIterator<Drone>::iterator e = DroneDataPool::TypeIterator<Drone>::end();

    for (auto a = s; a != e; a++) found++;

    end = getCPUCycles();
    timeMeasureEndAndReport();
    std::cout << "B cycles: " << end - start << std::endl;
    std::cout << "B cycle per element: " << (end - start) / 12000000 << std::endl;

    ASSERT(found == 10000012);

    SUCCESS();
}