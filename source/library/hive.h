#pragma once

#ifndef HIVE_HEADER_H
#define HIVE_HEADER_H

#define HIVE_USE_GLFW
#define HIVE_DEBUG
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

#include "primitive/typedef.hpp"


// GPU Systems
#ifdef HIVE_USE_OPENGL
#include "gpu/opengl/gl.hpp"
#endif

#ifdef HIVE_USE_VULKAN
#include "gpu/vulkan/vulkan_wrap.hpp"
#endif


// MATHS Stuff
#include "primitive/math/line.hpp"
#include "primitive/math/mat33.hpp"
#include "primitive/math/mat44.hpp"
#include "primitive/math/qbezier.hpp"
#include "primitive/math/quat.hpp"
#include "primitive/math/vec2.hpp"
#include "primitive/math/vec3.hpp"
#include "primitive/math/vec4.hpp"


// Interface - Accessed through an interface boss.
#include "interface/input/controller/controller.hpp"
#include "interface/input/keyboard/keyboard.hpp"
#include "interface/input/mouse/mouse.hpp"

// Bosses
#include "primitive/boss.hpp"
//#include "graphic/draw_boss.hpp"
#include "graphic/sprite_boss.hpp"
//#include "graphic/texture_boss.hpp"
#include "interface/glfw_interface_boss.hpp"
#include "interface/interface_boss.hpp"

// Primitives
#include "gpu/gpu_primitive.hpp"
#include "primitive/drone.hpp"
#include "primitive/mesh.hpp"
#include "primitive/prop.hpp"
#include "primitive/shader_program.hpp"

// Resource
#include "resource/load.hpp"

// Utility
#include "primitive/log.hpp"


namespace hive
{
    namespace all
    {
        using namespace hive;
        // using namespace hive::graphic;
        using namespace hive::interface;
        using namespace hive::math;
        // using namespace gl;
    } // namespace all
} // namespace hive

#endif