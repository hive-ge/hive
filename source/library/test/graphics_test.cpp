#include "includes.hpp"

using namespace hive;

// Create Global Memory Object.
hive::DataPool hive::general_data_pool(4096);

int main(int arg_ct, char ** args)
{
    hive::DroneDataPool pool(120); // Allocate enough space for 120 data elements of 32 bytes.

    // Setup bosses

    BigBadBoss boss;

    hive::interface::InterfaceBoss interface_boss(1280, 720);

    RenderBoss render;
    // AudioBoss
    // AnimationBoss
    // AssetBoss

    boss.setup();

    int timout = 120;

    while (boss.update() && timout-- > 0)
        ;

    SUCCESS();
}