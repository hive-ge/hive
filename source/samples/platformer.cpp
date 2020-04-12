#include <chrono>
#include <hive.h>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace hive::all;

#define PIXEL_COUNT 8192 * 4

int main(int arg_count, char ** arguments)
{
    // Setup Bosses
    GLFWBoss interface_boss(720, 480);
    // DrawBoss draw_boss;
    // TextureBoss texture_boss;
    // SpriteBoss sprite_boss;

    boss.setup();
    // Setup vulkan
    /*

    Drone & drone = boss.createDrone();

    drone.connect(sprite_boss.loadSprite("../test_sheet.png"));

    drone.connect(draw_boss.compileBasicProgram("#version 430\n"
                                                "layout (location = 0) in vec3 vert;"
                                                "layout (location = 1) in vec2 UVs;"
                                                "out vec2 texcoord;"
                                                "void main(){"
                                                "   texcoord = UVs;"
                                                "   gl_Position = vec4(vert*5.0, 1.0);"
                                                "}\n",
                                                "#version 430\n\n"
                                                "in vec2 texcoord;"
                                                "uniform sampler2D texture1;"
                                                "out vec4 FragColor;"
                                                "void main(){"
                                                "   FragColor = texture(texture1, texcoord);"
                                                "}\n"));
*/
    // draw_boss.allowDraw(drone);
    // draw_boss.barDraw();

    // sprite_boss.registerDraw(drone);

    while (boss.update()) {
    }
}