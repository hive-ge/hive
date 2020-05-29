#include "include/hive.h"

#include <atomic>
#include <mutex>
#include <thread>

using namespace hive;

bool Boss::SHOULD_EXIT = false;

std::vector<Boss *> Boss::bosses;
std::vector<Drone *> Boss::drones;

Boss::Boss(const unsigned _id) : id(_id)
{
    auto this_id = std::this_thread::get_id();

    Boss::bosses.push_back((Boss *)this);

    std::cout << (unsigned long long)this << std::endl;

    setIndex();
}

void hive::BigBadBoss::setup()
{
    Drone::setBoss(this);

    for (auto boss : Boss::bosses) {
        if (boss == this) continue;

        static_cast<Boss *>(boss)->setup();
    }
};

void BigBadBoss::teardown(){};

int BigBadBoss::priority() { return 0; };

BigBadBoss::BigBadBoss() : Boss(IDENTIFIER) {}

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

Drone * BigBadBoss::createDrone()
{

#warning "Naked new Allocation [Drone] in BigBadBoss::createDrone"

    int ptr = drones.size();

    Drone * drone = new Drone(ptr);

    drones.push_back(drone);

    return drone;
}

Drone * BigBadBoss::retrieveIndexedPointer(unsigned index) { return drones.at(index); };

void BigBadBoss::decrementIndexedReference(unsigned){

};
void BigBadBoss::incrementIndexedReference(unsigned){

};

void BigBadBoss::onMessage(StringHash64 message_id, const char * message_data,
                           const unsigned message_length){};