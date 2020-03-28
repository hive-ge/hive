#pragma once

//#define GL_GLEXT_PROTOTYPES
//#define HIVE_USE_GLFW

#ifndef HIVE_USE_SDL
#ifndef HIVE_USE_GLFW
#warning                                                                                           \
    "NO GL FRAMEWORK SELECTED - #define HIVE_USE_GLFW or #define HIVE_USE_SDL before including glwrap.h"
#define HIVE_USE_GLFW
#endif
#endif

#ifdef HIVE_USE_SDL
#include <SDL.h>
#include <SDL_opengl.h>
#endif // HIVE_USE_SDL

#ifdef HIVE_USE_GLFW
#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#endif // HIVE_USE_GLFW


#include <string>
#include <unordered_map>
#include <vector>

#include "primitive/math/math.h"

//#include "../elements/elements.h"

#ifndef HIVE_GL_REFERENCE_COUNT_MAX
#define HIVE_GL_REFERENCE_COUNT_MAX 1024
#endif // !HIVE_GL_REFERENCE_COUNT_MAX

#ifndef HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE
#define HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE unsigned short
#endif // !HIVE_GL_REFERENCE_COUNT_MAX

#ifndef HIVE_GL_REFERENCE_COUNT_MAX_VALUE
#define HIVE_GL_REFERENCE_COUNT_MAX_VALUE 255
#endif // !HIVE_GL_REFERENCE_COUNT_MAX

namespace hive
{
    namespace gl
    {

#ifdef HIVE_USE_SDL
        inline SDL_Window * initGL()
        {

            SDL_Window * window;

            SDL_CreateWindowAndRenderer(640, 480, 0, &window, nullptr);

            return window;
        }
#endif

        static HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE
            reference_counts[HIVE_GL_REFERENCE_COUNT_MAX * 5];

        static HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE tex_1D_references[128];

        static unsigned tex_2D_references[128];

        static HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE tex_3D_references[128];

        // Master buffer binds;
        static unsigned int bound_program;

        int SKGL_ARRAY_BUFFER_SET              = -1;
        int SKGL_ATOMIC_COUNTER_BUFFER_SET     = -1;
        int SKGL_COPY_READ_BUFFER_SET          = -1;
        int SKGL_COPY_WRITE_BUFFER_SET         = -1;
        int SKGL_DISPATCH_INDIRECT_BUFFER_SET  = -1;
        int SKGL_DRAW_INDIRECT_BUFFER_SET      = -1;
        int SKGL_ELEMENT_ARRAY_BUFFER_SET      = -1;
        int SKGL_PIXEL_PACK_BUFFER_SET         = -1;
        int SKGL_PIXEL_UNPACK_BUFFER_SET       = -1;
        int SKGL_QUERY_BUFFER_SET              = -1;
        int SKGL_SHADER_STORAGE_BUFFER_SET     = -1;
        int SKGL_TEXTURE_BUFFER_SET            = -1;
        int SKGL_TRANSFORM_FEEDBACK_BUFFER_SET = -1;
        int SKGL_UNIFORM_BUFFER_SET            = -1;

        // Quick size reference for GLSL types.
        unsigned getGLSLTypeBase(GLint t)
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
                return 13;
            case GL_DOUBLE_MAT2x3:
                return 3;
            case GL_DOUBLE_MAT3x2:
                return 3;
            case GL_DOUBLE_MAT2x4:
                return 3;
            case GL_DOUBLE_MAT4x2:
                return 3;
            case GL_DOUBLE_MAT3x4:
                return 13;
            case GL_DOUBLE_MAT4x3:
                return 13;
            }
            return 0;
        }

        unsigned getGLSLTypeSize(GLint t)
        {
            switch (t) {
            case GL_FLOAT:
                return 4;
            case GL_FLOAT_VEC2:
                return 2;
            case GL_FLOAT_VEC3:
                return 3;
            case GL_FLOAT_VEC4:
                return 4;
            case GL_FLOAT_MAT2:
                return 4;
            case GL_FLOAT_MAT3:
                return 9;
            case GL_FLOAT_MAT4:
                return 16;
            case GL_FLOAT_MAT2x3:
                return 6;
            case GL_FLOAT_MAT3x2:
                return 6;
            case GL_FLOAT_MAT2x4:
                return 8;
            case GL_FLOAT_MAT4x2:
                return 8;
            case GL_FLOAT_MAT3x4:
                return 12;
            case GL_FLOAT_MAT4x3:
                return 12;
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
            case GL_DOUBLE:
                return 4;
            case GL_DOUBLE_VEC2:
                return 2;
            case GL_DOUBLE_VEC3:
                return 3;
            case GL_DOUBLE_VEC4:
                return 4;
            case GL_DOUBLE_MAT2:
                return 4;
            case GL_DOUBLE_MAT3:
                return 9;
            case GL_DOUBLE_MAT4:
                return 16;
            case GL_DOUBLE_MAT2x3:
                return 6;
            case GL_DOUBLE_MAT3x2:
                return 6;
            case GL_DOUBLE_MAT2x4:
                return 8;
            case GL_DOUBLE_MAT4x2:
                return 8;
            case GL_DOUBLE_MAT3x4:
                return 12;
            case GL_DOUBLE_MAT4x3:
                return 12;
            }
            return 0;
        }

        enum SKGLB : unsigned short {
            ARRAY_BUFFER              = 1,
            ATOMIC_COUNTER_BUFFER     = 2,
            COPY_READ_BUFFER          = 4,
            COPY_WRITE_BUFFER         = 8,
            DISPATCH_INDIRECT_BUFFER  = 16,
            DRAW_INDIRECT_BUFFER      = 32,
            ELEMENT_ARRAY_BUFFER      = 64,
            PIXEL_PACK_BUFFER         = 128,
            PIXEL_UNPACK_BUFFER       = 256,
            QUERY_BUFFER              = 512,
            SHADER_STORAGE_BUFFER     = 1024,
            TEXTURE_BUFFER            = 2048,
            TRANSFORM_FEEDBACK_BUFFER = 4096,
            UNIFORM_BUFFER            = 8192,
        };

        enum SmartGLType : unsigned char { Null, Program, Attribute, Uniform, Texture, Buffer };

        /* Wrapper around GLint pointers for OO style GL programming. Works for
         * Gluint style pointers as well*/
        struct SmartGLint {
          private:
          protected: // Properties
            GLint pointer                                          = -1;
            HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE * reference_count = NULL;
            bool IS_READY                                          = false;
            SmartGLType type                                       = SmartGLType::Null;

          protected: // Function Members
            inline void increaseReferenceCount()
            {
                if (reference_count != NULL)
                    if (reference_count[0] == HIVE_GL_REFERENCE_COUNT_MAX_VALUE) {
                        throw("Max number of resource references reached");
                    } else
                        reference_count[0]++;
            };


            inline void decreaseReferenceCount()
            {
                if (reference_count != NULL)
                    if (reference_count[0] == 1)
                        deleteUnderlyingGLResource();
                    else
                        reference_count[0]--;
            };


            inline void setPointer(GLint p)
            {
                if (pointer != -1) decreaseReferenceCount();

                pointer = p;

                if (pointer > -1) {
                    reference_count =
                        &reference_counts[pointer + (HIVE_GL_REFERENCE_COUNT_MAX * type)];
                    increaseReferenceCount();
                } else
                    reference_count = NULL;
            }

            virtual void deleteUnderlyingGLResource() = 0;

          public: // Function Members
            SmartGLint() {}
            SmartGLint(SmartGLType t) : type(t) {}
            SmartGLint(SmartGLType t, GLint program_pointer, bool ISREADY) : type(t)
            {
                setPointer(program_pointer);
                IS_READY = (pointer > -1 && ISREADY);
            };

            operator GLuint() const { return pointer; }
            operator GLint() const { return pointer; }

            SmartGLint(const SmartGLint & obj)
            {
                if (obj.type != type) type = SmartGLType::Null;
                pointer         = obj.pointer;
                reference_count = obj.reference_count;
                IS_READY        = obj.IS_READY;
                type            = obj.type;
                increaseReferenceCount();
            }

            SmartGLint & operator=(const SmartGLint & obj)
            {
                if (obj.type != type)
                    throw("Incompatible SmartGLint assignment. Types do not match.");
                pointer         = obj.pointer;
                reference_count = obj.reference_count;
                IS_READY        = obj.IS_READY;
                type            = obj.type;
                increaseReferenceCount();
                return *this;
            }

            ~SmartGLint(){};
            // Returns a GL pointer
            inline GLint gli() { return pointer; };
            inline GLuint glui() { return (GLuint)pointer; };

            virtual void release() = 0;
            virtual bool IS_USABLE() { return IS_READY; };
        };


    } // namespace gl
} // namespace hive
