#include <chrono>
#include <hive.h>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace hive::all;

#define PIXEL_COUNT 8192 * 4

int main(int arg_count, char ** arguments)
{

    BigBadBoss boss;

    // Setup Bosses
    GLFWBoss interface_boss(1920, 1080);
    DrawBoss draw_boss;
    TextureBoss texture_boss;
    SpriteBoss sprite_boss;

    boss.setup();

    Drone & drone = boss.createDrone();

    drone.connect(sprite_boss.loadSprite("../test_sheet.png"));
    drone.connect(draw_boss.compileBasicProgram("//@spriteable"
                                                "//@billboard"
                                                "#version 430"
                                                "in vec3 vert;"
                                                "in vec2 UV;"
                                                "out vec2 texcoords;"
                                                "void main(){"
                                                "texcoords = UV;"
                                                "gl_Position = vec4(vert, 1.0);"
                                                "}",
                                                "#version 430"
                                                "//@spriteable"
                                                "//@billboard"
                                                "in vec2 texcoords;"
                                                "layout(location = 0) out vec4 color;"
                                                "uniform sampler2D texture1;"
                                                "void main(){"
                                                "color = texture(texture1, texcoords);"
                                                "}"));

    draw_boss.allowDraw(drone);
    // draw_boss.barDraw();

    // sprite_boss.registerDraw(drone);

    while (boss.update())
        ;
}