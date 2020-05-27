
/*
 * Copyright 2011 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#include <emscripten.h>

#include <GL/gl.h>
#include <hive.h>
#include <stdio.h>

#include "./interfaces.hpp"

using namespace hive::all;

BigBadBoss * boss;


void frame()
{
    if (boss->update()) {
    };
}

int main()
{
    BigBadBoss BBBoss;

    boss = &BBBoss;

    InterfaceBoss interface_boss(1280, 720);
    RenderBoss render_boss;

    boss->setup();

    glClearColor(0, 0, 0, 1.0);

    emscripten_set_main_loop(frame, 0, 1);

    boss = nullptr;
    std::cout << "DEAD!" << std::endl;
    return 0;
}
