#include "include/hive.h"

#include <atomic>
#include <mutex>
#include <thread>

using namespace hive;

bool Boss::SHOULD_EXIT = false;

std::vector<Boss *> Boss::bosses;
std::vector<DroneData *> Boss::drones;

Boss::Boss(const unsigned _id) : id(_id)
{
    auto this_id = std::this_thread::get_id();

    Boss::bosses.push_back((Boss *)this);

    std::cout << (unsigned long long)this << std::endl;

    setIndex();

    std::cout << "Create :: Bosses Address " << (unsigned long long)&bosses << " " << bosses.size()
              << std::endl;

    std::cout << "Create :: Bosses Address " << (unsigned long long)&bosses << " " << bosses.size()
              << std::endl;

    std::cout << "Create :: Bosses Address " << (unsigned long long)&bosses << " " << bosses.size()
              << std::endl;

    std::cout << "Create :: Bosses Address " << (unsigned long long)&bosses << " " << bosses.size()
              << std::endl;
}

void hive::BigBadBoss::setup()
{

    std::cout << "Setup :: Bosses Address " << (unsigned long long)&bosses << " " << bosses.size()
              << std::endl;

    for (auto boss : Boss::bosses) {
        if (boss == this) continue;

        static_cast<Boss *>(boss)->setup();
    }
};

void BigBadBoss::teardown(){};

int hive::BigBadBoss::priority() { return 0; };


void BigBadBoss::update(float delta_t)
{
    if (SHOULD_EXIT) {

        for (auto boss : bosses) {

            if (boss == this) continue;

            static_cast<Boss *>(boss)->teardown();
        }

        teardown();

    } else {
        for (auto boss : bosses) {

            if (boss == this) continue;

            static_cast<Boss *>(boss)->update(delta_t);
        }
    }
}

bool BigBadBoss::update()
{

    update(0.0);

    if (SHOULD_EXIT) {

        for (auto boss : bosses) {

            if (boss == this) continue;

            static_cast<Boss *>(boss)->teardown();
        }

        teardown();

        return false;
    }

    return true;
}

DroneData * BigBadBoss::createDroneData(Drone * drone)
{
    BigBadBoss & boss = *Drone::getBoss();

    DroneData * data = new DroneData();

    drone->id.id = boss.drones.size();

    drones.push_back(data);

    drone->data = data;

    return data;
}

DroneData * BigBadBoss::getDroneData(Drone * drone)
{
    unsigned drone_index = drone->id.id;

    // Potentially do some indirection lookup to return either a
    // stub drone or drone that has been relocated.

    return drones[drone_index];
}
