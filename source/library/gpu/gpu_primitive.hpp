#pragma once

#ifdef HIVE_USE_VULKAN
#include "gpu/vulkan/vulkan_wrap.hpp"
#endif

#ifdef HIVE_USE_OPENGL
#include "gpu/opengl/gl.h"
#include "gpu/opengl/program.h"
#include "gpu/opengl/texture.h"
#endif