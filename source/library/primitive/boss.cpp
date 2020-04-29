#include "./boss.hpp"

using namespace hive;

bool Boss::SHOULD_EXIT = false;
std::vector<Boss *> Boss::bosses;

void hive::BigBadBossA::setup()
{
    for (auto boss : bosses) {

        if (boss == this) continue;

        boss->setup();
    }
};

void BigBadBossA::teardown(){

};

int hive::BigBadBossA::priority() { return 0; };


void BigBadBossA::update(float delta_t)
{
    if (SHOULD_EXIT) {

        for (auto boss : bosses) {

            if (boss == this) continue;

            boss->teardown();
        }

        teardown();

    } else {
        for (auto boss : bosses) {

            if (boss == this) continue;

            boss->update(delta_t);
        }
    }
}

bool BigBadBossA::update()
{

    update(0.0);

    if (SHOULD_EXIT) {

        for (auto boss : bosses) {

            if (boss == this) continue;

            boss->teardown();
        }

        teardown();

        return false;
    }

    return true;
}