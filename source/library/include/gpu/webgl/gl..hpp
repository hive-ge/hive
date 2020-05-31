#pragma once

#include <GL/glew.h>
/* intentional */
#include <GL/gl.h>

#include "include/primitive/prop_gpu_artifact.hpp"

namespace hive
{
    // Byte size of a single index of type.
    unsigned getGLSLTypeSize(GLint t);
    unsigned getGLSLTypeElementCount(GLint t);
    ShaderArtifact::ElementPrimitiveType getGLSLTypePrimitive(GLint t);
    ShaderArtifact::ArtifactType getGLSLSamplerArtifactType(GLint t);
    bool isUniformTypeSampler(GLint t);
} // namespace hive
