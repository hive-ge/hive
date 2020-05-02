#include "./boss.hpp"
#include <atomic>
#include <mutex>
#include <thread>

using namespace hive;

bool Boss::SHOULD_EXIT = false;

std::vector<Boss *> Boss::bosses;

Boss::Boss(const unsigned _id) : id(_id)
{
    auto this_id = std::this_thread::get_id();

    for (auto boss : Boss::bosses) {

        std::cout << "Boss Address " << (unsigned long long)boss << std::endl;
    }

    Boss::bosses.push_back((Boss *)this);
    std::cout << (unsigned long long)this << std::endl;
    std::cout << "Bosses Address " << (unsigned long long)&bosses << std::endl;
    std::cout << "thread " << this_id << " isss...\n";

    setIndex();
}

void hive::BigBadBoss::setup()
{
    std::cout << "SEtup :: Bosses Address " << (unsigned long long)&bosses << std::endl;
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
