#include <chrono>
#include <hive.h>
#include <iostream>
#include <stdio.h>


using namespace hive::all;

int main(int arg_count, char ** arguments)
{

    hive::interface::GLFWBoss boss(1280, 720);

    if (!boss.init()) return -1;
    // if (!GLEW_VERSION_4_3) return -1;


    hive::graphic::Quadrilateral box;

    auto keyboard   = boss.getKeyboard();
    auto controller = boss.getController(0);
    auto mouse      = boss.getMouse();

    mat44 camera = mat44::perspectiveProjectionMatrix(90, 1280.0 / 720.0, 0.01, 100);

    mat44 pos;

    pos.m12 = -500.0;


    std::string str = hive::resource::loadUTF8File(
        "/home/anthony/work/active/apps/hive/source/shader/compute/test.comp");

    hive::gl::createComputeShaderProgram(str);

    // making background color black at first
    // 3 arguments all are 0.0

    glClearColor(0.0, 0.0, 0.0, 1.0);
    while (boss.canRun()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // controller.poll();

        // std::cout << controller << std::endl;

        int dur = keyboard.getKeyPressDuration(hive::interface::KeyName::A);

        if (dur > 0 && !keyboard.isKeyPressed(hive::interface::KeyName::A)) {
            std::cout << "Key A was pressed for " << std::to_string(dur) << "ms" << std::endl;
        }

        pos.m12 = -((controller.getAxis(controller::AxisName::ONE) * 200) + 150);

        box.draw(camera * pos);
        boss.update();
    }
}