#pragma once


#define HIVE_USE_GLFW
#define HIVE_DEBUG
#define HIVE_USE_OPENGL
//#define HIVE_USE_VULKAN

#define HIVE_MAJOR_VERSION 0
#define HIVE_MINOR_VERSION 1
#define HIVE_PATCH_VERSION 2

#ifndef HIVE_APP_MAJOR_VERSION
#define HIVE_APP_MAJOR_VERSION 0
#endif

#ifndef HIVE_APP_MINOR_VERSION
#define HIVE_APP_MINOR_VERSION 0
#endif

#ifndef HIVE_APP_PATCH_VERSION
#define HIVE_APP_PATCH_VERSION 0
#endif


#include "primitive/typedef.h"

// GPU Systems
#include "gpu/opengl/gl.h"
#include "gpu/vulkan/vulkan_wrap.hpp"

// Primitives
#include "gpu/gpu_primitive.hpp"
#include "primitive/drone/drone.h"

// MATHS Stuff
#include "primitive/math/line.h"
#include "primitive/math/mat33.h"
#include "primitive/math/mat44.h"
#include "primitive/math/qbezier.h"
#include "primitive/math/quat.h"
#include "primitive/math/vec2.h"
#include "primitive/math/vec3.h"
#include "primitive/math/vec4.h"


// Interface - Accessed through an interface boss.
#include "interface/input/controller/controller.hpp"
#include "interface/input/keyboard/keyboard.hpp"
#include "interface/input/mouse/mouse.hpp"

// Bosses
//#include "graphic/draw_boss.hpp"
//#include "graphic/sprite_boss.hpp"
//#include "graphic/texture_boss.hpp"
#include "interface/glfw_interface_boss.hpp"
#include "interface/interface_boss.hpp"
#include "primitive/boss.hpp"

// Resource
#include "resource/load.hpp"

// Utility
#include "primitive/log.h"


namespace hive
{
    namespace all
    {
        using namespace hive;
        // using namespace hive::graphic;
        using namespace hive::interface;
        // using namespace gl;
    } // namespace all
} // namespace hive
