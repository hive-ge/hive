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

//#ifdef HIVE_USE_GLFW
//#define GLEW_STATIC
//#include "/usr/local/include/gl3w/GL/gl3w.h"
#include <GL/glew.h>

//#endif // HIVE_USE_GLFW


#include <string>
#include <unordered_map>
#include <vector>

#include "include/primitive/core_math.hpp"
#include "primitive/log.hpp"


#ifndef HIVE_GL_REFERENCE_COUNT_MAX
#define HIVE_GL_REFERENCE_COUNT_MAX 2048
#endif // !HIVE_GL_REFERENCE_COUNT_MAX

#ifndef HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE
#define HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE unsigned char
#endif // !HIVE_GL_REFERENCE_COUNT_MAX

#ifndef HIVE_GL_REFERENCE_COUNT_MAX_VALUE
#define HIVE_GL_REFERENCE_COUNT_MAX_VALUE 255
#endif // !HIVE_GL_REFERENCE_COUNT_MAX

namespace hive
{
    namespace gl
    {


        /**
         * returns true if major and minor versions of GL are equal to or greater than
         * the passed in values.
         */
        static bool checkGLVersion(int & major, int & minor);

#ifdef HIVE_USE_SDL
        SDL_Window * initGL();
#endif
        static HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE reference_counts[HIVE_GL_REFERENCE_COUNT_MAX] =
            {0};

        HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE *
        getReferenceCounter(int = HIVE_GL_REFERENCE_COUNT_MAX, int = 0);

        static HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE tex_1D_references[128];

        static unsigned tex_2D_references[128];

        static HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE tex_3D_references[128];

        // Master buffer binds;
        static unsigned int bound_program;

        extern int SKGL_ARRAY_BUFFER_SET;
        extern int SKGL_ATOMIC_COUNTER_BUFFER_SET;
        extern int SKGL_COPY_READ_BUFFER_SET;
        extern int SKGL_COPY_WRITE_BUFFER_SET;
        extern int SKGL_DISPATCH_INDIRECT_BUFFER_SET;
        extern int SKGL_DRAW_INDIRECT_BUFFER_SET;
        extern int SKGL_ELEMENT_ARRAY_BUFFER_SET;
        extern int SKGL_PIXEL_PACK_BUFFER_SET;
        extern int SKGL_PIXEL_UNPACK_BUFFER_SET;
        extern int SKGL_QUERY_BUFFER_SET;
        extern int SKGL_SHADER_STORAGE_BUFFER_SET;
        extern int SKGL_TEXTURE_BUFFER_SET;
        extern int SKGL_TRANSFORM_FEEDBACK_BUFFER_SET;
        extern int SKGL_UNIFORM_BUFFER_SET;

        // Quick primitive type reference for GLSL types.
        unsigned getGLSLTypePrimitive(GLint t);

        // Number of elements per type
        unsigned getGLSLTypeElementCount(GLint t);

        // Byte size of a single index of type.
        unsigned getGLSLTypeSize(GLint t);

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

        enum class SmartGLType : unsigned char {
            Null,
            Program,
            Input,
            Output,
            Uniform,
            Texture,
            Buffer,
            UniformBlock,
            ShaderBufferBlock,
        };

        bool clearErrors();

        /* Wrapper around GLint pointers for OO style GL programming. Works for
         * Gluint style pointers as well*/
        struct SmartGLint {
          private:
            HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE * reference_count = NULL;
            SmartGLType sm_gl_type                                 = SmartGLType::Null;

          protected: // Properties
            GLint pointer = -1;
            bool IS_READY = false;

          protected: // Function Members
            inline void increaseReferenceCount()
            {
                if (reference_count != NULL) {

                    if (reference_count[0] == HIVE_GL_REFERENCE_COUNT_MAX_VALUE) {
                        throw("Max number of resource references reached");
                    } else {
                        reference_count[0]++;
                    }
                }
            };


            inline void decreaseReferenceCount()
            {
                if (reference_count != NULL) {

                    if (reference_count[0] == 1) {
                        deleteUnderlyingGLResource();
                        reference_count[0] = 0;
                        reference_count    = nullptr;
                    } else {
                        reference_count[0]--;
                    }
                }
            };

            inline void setPointer(GLint p)
            {
                if (pointer != -1) decreaseReferenceCount();

                pointer = p;

                if (pointer > -1) {
                    reference_count = getReferenceCounter();

                    if (!reference_count) {

                        __LOG("Unable to obtain a reference counter for SmartGLint type " +
                              std::to_string((int)sm_gl_type) +
                              " rcs:" + std::to_string(HIVE_GL_REFERENCE_COUNT_MAX));

                        throw(-1);
                    }

                    increaseReferenceCount();
                } else
                    reference_count = NULL;
            }

            virtual void deleteUnderlyingGLResource() = 0;

          public: // Function Members
            SmartGLint() {}
            SmartGLint(SmartGLType t) : sm_gl_type(t) {}
            SmartGLint(SmartGLType t, GLint program_pointer, bool ISREADY) : sm_gl_type(t)
            {
                setPointer(program_pointer);
                IS_READY = (pointer > -1 && ISREADY);
            };

            operator GLuint() const { return pointer; }
            operator GLint() const { return pointer; }

            SmartGLint(const SmartGLint & obj)
            {
                if (obj.sm_gl_type != sm_gl_type) sm_gl_type = SmartGLType::Null;
                pointer         = obj.pointer;
                reference_count = obj.reference_count;
                IS_READY        = obj.IS_READY;
                sm_gl_type      = obj.sm_gl_type;
                increaseReferenceCount();
            }

            SmartGLint & operator=(const SmartGLint & obj)
            {
                if (obj.sm_gl_type != sm_gl_type)
                    __ERROR("Incompatible SmartGLint assignment. Types do not match.");
                pointer         = obj.pointer;
                reference_count = obj.reference_count;
                IS_READY        = obj.IS_READY;
                sm_gl_type      = obj.sm_gl_type;
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