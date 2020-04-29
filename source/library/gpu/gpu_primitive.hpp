#pragma once

#ifdef HIVE_USE_VULKAN
#include "gpu/vulkan/vulkan_wrap.hpp"
#endif

#ifdef HIVE_USE_OPENGL
#include "gpu/opengl/gl.hpp"
#include "gpu/opengl/program.hpp"
#include "gpu/opengl/texture.hpp"
#endif