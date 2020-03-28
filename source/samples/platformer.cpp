
#include <chrono>


#include <hive.h>
#include <iostream>
#include <stdio.h>

int main(int arg_count, char ** arguments)
{

    hive::interface::GLFWBoss boss(200, 200);

    if (!boss.init()) return -1;


    hive::graphic::Quadrilateral box;

    auto keyboard   = boss.getKeyboard();
    auto controller = boss.getController(0);

    if (!controller.isALIVE()) return -1;

    // making background color black as first
    // 3 arguments all are 0.0
    glClearColor(0.0, 0.0, 0.0, 1.0);

    while (true) {
        controller.poll();
        if (controller.hasChanged()) {
            std::cout << controller << std::endl;
        }


        int dur = keyboard.getKeyPressDuration(hive::interface::KeyName::A);

        if (dur > 0 && !keyboard.isKeyPressed(hive::interface::KeyName::A)) {
            std::cout << "Key A was pressed for " << std::to_string(dur) << "ms" << std::endl;
        }

        boss.update();
        box.draw();
    }
}