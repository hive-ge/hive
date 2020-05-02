#include "./glwrap.hpp"

//#define GL_GLEXT_PROTOTYPES
//#define HIVE_USE_GLFW

using namespace hive;
using namespace hive::gl;

int hive::gl::SKGL_ARRAY_BUFFER_SET              = -1;
int hive::gl::SKGL_ATOMIC_COUNTER_BUFFER_SET     = -1;
int hive::gl::SKGL_COPY_READ_BUFFER_SET          = -1;
int hive::gl::SKGL_COPY_WRITE_BUFFER_SET         = -1;
int hive::gl::SKGL_DISPATCH_INDIRECT_BUFFER_SET  = -1;
int hive::gl::SKGL_DRAW_INDIRECT_BUFFER_SET      = -1;
int hive::gl::SKGL_ELEMENT_ARRAY_BUFFER_SET      = -1;
int hive::gl::SKGL_PIXEL_PACK_BUFFER_SET         = -1;
int hive::gl::SKGL_PIXEL_UNPACK_BUFFER_SET       = -1;
int hive::gl::SKGL_QUERY_BUFFER_SET              = -1;
int hive::gl::SKGL_SHADER_STORAGE_BUFFER_SET     = -1;
int hive::gl::SKGL_TEXTURE_BUFFER_SET            = -1;
int hive::gl::SKGL_TRANSFORM_FEEDBACK_BUFFER_SET = -1;
int hive::gl::SKGL_UNIFORM_BUFFER_SET            = -1;

/**
 * returns true if major and minor versions of GL are equal to or greater than
 * the passed in values.
 */
static bool checkGLVersion(int & major, int & minor)
{
    return true; // gl3wIsSupported(major, minor);
};

#ifdef HIVE_USE_SDL
inline SDL_Window * initGL()
{

    SDL_Window * window;

    SDL_CreateWindowAndRenderer(640, 480, 0, &window, nullptr);

    return window;
}
#endif

HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE * hive::gl::getReferenceCounter(int search_size,
                                                                     int start_index)
{

    if (search_size == 1) {
        if (reference_counts[start_index] == 0)
            return reference_counts + start_index;
        else
            return nullptr;
    }

    auto size = search_size >> 1;

    HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE * rc = getReferenceCounter(size, start_index);
    if (!rc) return getReferenceCounter(size, start_index + size);
    return rc;
}

// Quick primitive type reference for GLSL types.
unsigned hive::gl::getGLSLTypePrimitive(GLint t)
{
    switch (t) {
    case GL_FLOAT:
        return 2;
    case GL_FLOAT_VEC2:
        return 2;
    case GL_FLOAT_VEC3:
        return 2;
    case GL_FLOAT_VEC4:
        return 2;
    case GL_FLOAT_MAT2:
        return 2;
    case GL_FLOAT_MAT3:
        return 2;
    case GL_FLOAT_MAT4:
        return 2;
    case GL_FLOAT_MAT2x3:
        return 2;
    case GL_FLOAT_MAT3x2:
        return 2;
    case GL_FLOAT_MAT2x4:
        return 2;
    case GL_FLOAT_MAT4x2:
        return 2;
    case GL_FLOAT_MAT3x4:
        return 2;
    case GL_FLOAT_MAT4x3:
        return 2;
    case GL_INT:
        return 1;
    case GL_INT_VEC2:
        return 1;
    case GL_INT_VEC3:
        return 1;
    case GL_INT_VEC4:
        return 1;
    case GL_UNSIGNED_INT:
        return 0;
    case GL_UNSIGNED_INT_VEC2:
        return 0;
    case GL_UNSIGNED_INT_VEC3:
        return 0;
    case GL_UNSIGNED_INT_VEC4:
        return 0;
    case GL_DOUBLE:
        return 3;
    case GL_DOUBLE_VEC2:
        return 3;
    case GL_DOUBLE_VEC3:
        return 3;
    case GL_DOUBLE_VEC4:
        return 3;
    case GL_DOUBLE_MAT2:
        return 3;
    case GL_DOUBLE_MAT3:
        return 3;
    case GL_DOUBLE_MAT4:
        return 3;
    case GL_DOUBLE_MAT2x3:
        return 3;
    case GL_DOUBLE_MAT3x2:
        return 3;
    case GL_DOUBLE_MAT2x4:
        return 3;
    case GL_DOUBLE_MAT4x2:
        return 3;
    case GL_DOUBLE_MAT3x4:
        return 3;
    case GL_DOUBLE_MAT4x3:
        return 3;
    }
    return 0;
};

// Number of elements per type
unsigned hive::gl::getGLSLTypeElementCount(GLint t)
{
    switch (t) {
    case GL_FLOAT:
        return 1;
    case GL_FLOAT_VEC2:
        return 2;
    case GL_FLOAT_VEC3:
        return 3;
    case GL_FLOAT_VEC4:
        return 4;
    case GL_DOUBLE:
        return 1;
    case GL_DOUBLE_VEC2:
        return 2;
    case GL_DOUBLE_VEC3:
        return 3;
    case GL_DOUBLE_VEC4:
        return 4;
    case GL_INT:
        return 1;
    case GL_INT_VEC2:
        return 2;
    case GL_INT_VEC3:
        return 3;
    case GL_INT_VEC4:
        return 4;
    case GL_UNSIGNED_INT:
        return 1;
    case GL_UNSIGNED_INT_VEC2:
        return 2;
    case GL_UNSIGNED_INT_VEC3:
        return 3;
    case GL_UNSIGNED_INT_VEC4:
        return 4;
    case GL_BOOL:
        return 1;
    case GL_BOOL_VEC2:
        return 2;
    case GL_BOOL_VEC3:
        return 3;
    case GL_BOOL_VEC4:
        return 4;
    case GL_FLOAT_MAT2:
        return 4;
    case GL_FLOAT_MAT3:
        return 9;
    case GL_FLOAT_MAT4:
        return 16;
    case GL_FLOAT_MAT2x3:
        return 6;
    case GL_FLOAT_MAT2x4:
        return 8;
    case GL_FLOAT_MAT3x2:
        return 6;
    case GL_FLOAT_MAT3x4:
        return 12;
    case GL_FLOAT_MAT4x2:
        return 8;
    case GL_FLOAT_MAT4x3:
        return 12;
    case GL_DOUBLE_MAT2:
        return 4;
    case GL_DOUBLE_MAT3:
        return 9;
    case GL_DOUBLE_MAT4:
        return 16;
    case GL_DOUBLE_MAT2x3:
        return 6;
    case GL_DOUBLE_MAT2x4:
        return 8;
    case GL_DOUBLE_MAT3x2:
        return 6;
    case GL_DOUBLE_MAT3x4:
        return 12;
    case GL_DOUBLE_MAT4x2:
        return 8;
    case GL_DOUBLE_MAT4x3:
        return 12;
    case GL_SAMPLER_1D:
        return 1;
    case GL_SAMPLER_2D:
        return 2;
    case GL_SAMPLER_3D:
        return 3;
    case GL_SAMPLER_CUBE:
        return 3;
    case GL_SAMPLER_1D_SHADOW:
        return 1;
    case GL_SAMPLER_2D_SHADOW:
        return 2;
    case GL_SAMPLER_1D_ARRAY:
        return 3;
    case GL_SAMPLER_2D_ARRAY:
        return 2;
    case GL_SAMPLER_1D_ARRAY_SHADOW:
        return 1;
    case GL_SAMPLER_2D_ARRAY_SHADOW:
        return 2;
    case GL_SAMPLER_2D_MULTISAMPLE:
        return 2;
    case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
        return 2;
    case GL_SAMPLER_CUBE_SHADOW:
        return 6;
    case GL_SAMPLER_BUFFER:
        return 1;
    case GL_SAMPLER_2D_RECT:
        return 2;
    case GL_SAMPLER_2D_RECT_SHADOW:
        return 2;
    case GL_INT_SAMPLER_1D:
        return 1;
    case GL_INT_SAMPLER_2D:
        return 2;
    case GL_INT_SAMPLER_3D:
        return 3;
    case GL_INT_SAMPLER_CUBE:
        return 6;
    case GL_INT_SAMPLER_1D_ARRAY:
        return 1;
    case GL_INT_SAMPLER_2D_ARRAY:
        return 2;
    case GL_INT_SAMPLER_2D_MULTISAMPLE:
        return 2;
    case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
        return 2;
    case GL_INT_SAMPLER_BUFFER:
        return 1;
    case GL_INT_SAMPLER_2D_RECT:
        return 2;
    case GL_UNSIGNED_INT_SAMPLER_1D:
        return 1;
    case GL_UNSIGNED_INT_SAMPLER_2D:
        return 2;
    case GL_UNSIGNED_INT_SAMPLER_3D:
        return 3;
    case GL_UNSIGNED_INT_SAMPLER_CUBE:
        return 6;
    case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
        return 1;
    case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
        return 2;
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
        return 2;
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
        return 2;
    case GL_UNSIGNED_INT_SAMPLER_BUFFER:
        return 1;
    case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
        return 2;
    }
    return 0;
}

// Byte size of a single index of type.
unsigned hive::gl::getGLSLTypeSize(GLint t)
{
    switch (t) {
    case GL_FLOAT:
        return 4;
    case GL_FLOAT_VEC2:
        return 8;
    case GL_FLOAT_VEC3:
        return 12;
    case GL_FLOAT_VEC4:
        return 16;
    case GL_DOUBLE:
        return 8;
    case GL_DOUBLE_VEC2:
        return 16;
    case GL_DOUBLE_VEC3:
        return 24;
    case GL_DOUBLE_VEC4:
        return 32;
    case GL_INT:
        return 4;
    case GL_INT_VEC2:
        return 8;
    case GL_INT_VEC3:
        return 12;
    case GL_INT_VEC4:
        return 16;
    case GL_UNSIGNED_INT:
        return 4;
    case GL_UNSIGNED_INT_VEC2:
        return 8;
    case GL_UNSIGNED_INT_VEC3:
        return 12;
    case GL_UNSIGNED_INT_VEC4:
        return 16;
    case GL_BOOL:
        return 4;
    case GL_BOOL_VEC2:
        return 8;
    case GL_BOOL_VEC3:
        return 12;
    case GL_BOOL_VEC4:
        return 16;
    case GL_FLOAT_MAT2:
        return 16;
    case GL_FLOAT_MAT3:
        return 36;
    case GL_FLOAT_MAT4:
        return 64;
    case GL_FLOAT_MAT2x3:
        return 24;
    case GL_FLOAT_MAT2x4:
        return 32;
    case GL_FLOAT_MAT3x2:
        return 24;
    case GL_FLOAT_MAT3x4:
        return 42;
    case GL_FLOAT_MAT4x2:
        return 32;
    case GL_FLOAT_MAT4x3:
        return 42;
    case GL_DOUBLE_MAT2:
        return 32;
    case GL_DOUBLE_MAT3:
        return 72;
    case GL_DOUBLE_MAT4:
        return 128;
    case GL_DOUBLE_MAT2x3:
        return 48;
    case GL_DOUBLE_MAT2x4:
        return 64;
    case GL_DOUBLE_MAT3x2:
        return 48;
    case GL_DOUBLE_MAT3x4:
        return 84;
    case GL_DOUBLE_MAT4x2:
        return 64;
    case GL_DOUBLE_MAT4x3:
        return 84;
    case GL_SAMPLER_1D:
        return 4;
    case GL_SAMPLER_2D:
        return 8;
    case GL_SAMPLER_3D:
        return 12;
    case GL_SAMPLER_CUBE:
        return 12;
    case GL_SAMPLER_1D_SHADOW:
        return 4;
    case GL_SAMPLER_2D_SHADOW:
        return 8;
    case GL_SAMPLER_1D_ARRAY:
        return 12;
    case GL_SAMPLER_2D_ARRAY:
        return 8;
    case GL_SAMPLER_1D_ARRAY_SHADOW:
        return 4;
    case GL_SAMPLER_2D_ARRAY_SHADOW:
        return 8;
    case GL_SAMPLER_2D_MULTISAMPLE:
        return 8;
    case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
        return 8;
    case GL_SAMPLER_CUBE_SHADOW:
        return 24;
    case GL_SAMPLER_BUFFER:
        return 4;
    case GL_SAMPLER_2D_RECT:
        return 8;
    case GL_SAMPLER_2D_RECT_SHADOW:
        return 8;
    case GL_INT_SAMPLER_1D:
        return 4;
    case GL_INT_SAMPLER_2D:
        return 8;
    case GL_INT_SAMPLER_3D:
        return 12;
    case GL_INT_SAMPLER_CUBE:
        return 24;
    case GL_INT_SAMPLER_1D_ARRAY:
        return 4;
    case GL_INT_SAMPLER_2D_ARRAY:
        return 8;
    case GL_INT_SAMPLER_2D_MULTISAMPLE:
        return 8;
    case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
        return 8;
    case GL_INT_SAMPLER_BUFFER:
        return 4;
    case GL_INT_SAMPLER_2D_RECT:
        return 8;
    case GL_UNSIGNED_INT_SAMPLER_1D:
        return 4;
    case GL_UNSIGNED_INT_SAMPLER_2D:
        return 8;
    case GL_UNSIGNED_INT_SAMPLER_3D:
        return 12;
    case GL_UNSIGNED_INT_SAMPLER_CUBE:
        return 24;
    case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
        return 4;
    case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
        return 8;
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
        return 8;
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
        return 8;
    case GL_UNSIGNED_INT_SAMPLER_BUFFER:
        return 4;
    case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
        return 8;
    }
    return 0;
}

bool hive::gl::clearErrors()
{
    int gl_err = glGetError();
    bool err   = false;
    do {
        if (gl_err) err = true;
        gl_err = glGetError();
    } while (gl_err);
    return err;
}