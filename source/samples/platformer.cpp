
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
    auto mouse      = boss.getMouse();

    if (!controller.isALIVE()) return -1;

    std::cout << hive::resource::loadUTF8File("/home/anthony/work/active/apps/hive/readme.md");

    // making background color black as first
    // 3 arguments all are 0.0
    glClearColor(0.0, 0.0, 0.0, 1.0);

    while (true) {

        std::cout << mouse << std::endl;

        int dur = keyboard.getKeyPressDuration(hive::interface::KeyName::A);

        if (dur > 0 && !keyboard.isKeyPressed(hive::interface::KeyName::A)) {
            std::cout << "Key A was pressed for " << std::to_string(dur) << "ms" << std::endl;
        }

        boss.update();
        box.draw();
    }
}