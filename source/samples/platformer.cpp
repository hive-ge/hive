
#include <chrono>


#include <hive.h>
#include <iostream>
#include <stdio.h>

int main(int arg_count, char ** arguments)
{

    hive::interface::GLFWScreen screen(200, 200);

    if (!screen.init()) return -1;


    hive::graphic::Quadrilateral box;

    hive::interface::keyboard::Keyboard keyboard;

    // making background color black as first
    // 3 arguments all are 0.0
    glClearColor(0.0, 0.0, 0.0, 1.0);

    while (true) {
        screen.update();
        box.draw();
    }
}