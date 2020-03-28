#pragma once


#define HIVE_USE_GLFW
#define HIVE_DEBUG

// Graphics
#include "gpu/opengl/gl.h"

#include "graphic/box.hpp"


// Interface
#include "interface/input/controller/controller.hpp"
#include "interface/input/keyboard/keyboard.hpp"
#include "interface/input/mouse/mouse.hpp"
#include "interface/interface_boss.hpp"

// Resource
#include "resource/load.hpp"

// Utility
#include "primitive/log.h"


// MATHS Stuff

#include "primitive/math/line.h"
#include "primitive/math/mat33.h"
#include "primitive/math/mat44.h"
#include "primitive/math/qbezier.h"
#include "primitive/math/quat.h"
#include "primitive/math/vec2.h"
#include "primitive/math/vec3.h"
#include "primitive/math/vec4.h"
