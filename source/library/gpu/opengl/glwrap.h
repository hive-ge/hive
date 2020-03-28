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

        static unsigned HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE
            reference_counts[HIVE_GL_REFERENCE_COUNT_MAX * 5];

        static unsigned HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE tex_1D_references[128];

        static unsigned tex_2D_references[128];

        static unsigned HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE tex_3D_references[128];

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
            GLint pointer                                                   = -1;
            unsigned HIVE_GL_REFERENCE_COUNT_ELEMENT_TYPE * reference_count = NULL;
            bool IS_READY                                                   = false;
            SmartGLType type                                                = SmartGLType::Null;

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

        struct SmartGLUniform : SmartGLint {
          protected:
            unsigned short glsl_array_size = 1;
            unsigned char glsl_object_size = 1;

            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLUniform() : SmartGLint(SmartGLType::Uniform){};
            SmartGLUniform(GLuint program_pointer, unsigned os, unsigned as, bool ISREADY)
                : SmartGLint(SmartGLType::Uniform, program_pointer, ISREADY), glsl_object_size(os),
                  glsl_array_size(as){};
            ~SmartGLUniform() { decreaseReferenceCount(); }
            SmartGLUniform(const SmartGLUniform & obj) : SmartGLint(obj)
            {
                glsl_array_size  = obj.glsl_array_size;
                glsl_object_size = obj.glsl_object_size;
            }
            virtual void use();
            virtual void release() override;
            virtual bool IS_USABLE() override;


            inline friend void operator<<(SmartGLUniform & uni, hive::math::mat44 * mat)
            {
                uni.setMat4V((float *)mat);
            }


            inline friend void operator<<(SmartGLUniform & uni, hive::math::vec2 * vec)
            {
                uni.setFloat2V((float *)vec);
            }


            inline friend void operator<<(SmartGLUniform & uni, hive::math::vec3 * vec)
            {
                uni.setFloat3V((float *)vec);
            }


            inline friend void operator<<(SmartGLUniform & uni, hive::math::vec4 * vec)
            {
                uni.setFloat4V((float *)vec);
            }

            inline friend void operator<<(SmartGLUniform & uni, hive::math::vec2i * vec)
            {
                uni.setInt2V((int *)vec);
            }

            inline friend void operator<<(SmartGLUniform & uni, hive::math::vec3i * vec)
            {
                uni.setInt3V((int *)vec);
            }

            inline friend void operator<<(SmartGLUniform & uni, hive::math::vec4i * vec)
            {
                uni.setInt4V((int *)vec);
            }

            inline friend void operator<<(SmartGLUniform & uni, hive::math::vec2u * vec)
            {
                uni.setUint2V((unsigned int *)vec);
            }

            inline friend void operator<<(SmartGLUniform & uni, hive::math::vec3u * vec)
            {
                uni.setUint3V((unsigned int *)vec);
            }

            inline friend void operator<<(SmartGLUniform & uni, hive::math::vec4u * vec)
            {
                uni.setUint4V((unsigned int *)vec);
            }

            inline friend void operator<<(SmartGLUniform & uni, int * v) { uni.setInt1V(v); }

            inline friend void operator<<(SmartGLUniform & uni, unsigned int * v)
            {
                uni.setUint1V(v);
            }

            inline friend void operator<<(SmartGLUniform & uni, float * v) { uni.setFloat1V(v); }

            inline friend void operator<<(SmartGLUniform & uni, int v) { uni.setInt(v); }

            inline friend void operator<<(SmartGLUniform & uni, unsigned int v) { uni.setUint(v); }

            inline friend void operator<<(SmartGLUniform & uni, float v) { uni.setFloat(v); }

            inline void setFloat(float v) { glUniform1f(pointer, v); };

            inline void setFloat(float v1, float v2) { glUniform2f(pointer, v1, v2); };

            inline void setFloat(float v1, float v2, float v3)
            {
                glUniform3f(pointer, v1, v2, v3);
            };

            inline void setFloat(float v1, float v2, float v3, float v4)
            {
                glUniform4f(pointer, v1, v2, v3, v4);
            };

            inline void setFloat1V(float * v) { glUniform1fv(pointer, glsl_array_size, v); };

            inline void setFloat2V(float * v) { glUniform2fv(pointer, glsl_array_size, v); };

            inline void setFloat3V(float * v) { glUniform3fv(pointer, glsl_array_size, v); };

            inline void setFloat4V(float * v) { glUniform4fv(pointer, glsl_array_size, v); };

            inline void setInt(int v) { glUniform1i(pointer, v); };

            inline void setInt(int v1, int v2) { glUniform2i(pointer, v1, v2); };

            inline void setInt(int v1, int v2, int v3) { glUniform3i(pointer, v1, v2, v3); };

            inline void setInt(int v1, int v2, int v3, int v4)
            {
                glUniform4i(pointer, v1, v2, v3, v4);
            }

            inline void setInt1V(int * v) { glUniform1iv(pointer, glsl_array_size, v); };

            inline void setInt2V(int * v) { glUniform2iv(pointer, glsl_array_size, v); };

            inline void setInt3V(int * v) { glUniform3iv(pointer, glsl_array_size, v); };

            inline void setInt4V(int * v) { glUniform4iv(pointer, glsl_array_size, v); };

            inline void setUint(unsigned int v) { glUniform1ui(pointer, v); };

            inline void setUint(unsigned int v1, unsigned int v2)
            {
                glUniform2ui(pointer, v1, v2);
            };

            inline void setUint(unsigned int v1, unsigned int v2, unsigned int v3)
            {
                glUniform3ui(pointer, v1, v2, v3);
            };

            inline void setUint(unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4)
            {
                glUniform4ui(pointer, v1, v2, v3, v4);
            }

            inline void setUint1V(unsigned int * v) { glUniform1uiv(pointer, glsl_array_size, v); };

            inline void setUint2V(unsigned int * v) { glUniform2uiv(pointer, glsl_array_size, v); };

            inline void setUint3V(unsigned int * v) { glUniform3uiv(pointer, glsl_array_size, v); };

            inline void setUint4V(unsigned int * v) { glUniform4uiv(pointer, glsl_array_size, v); };

            inline void setMat2V(float * v)
            {
                glUniformMatrix2fv(pointer, glsl_array_size, false, v);
            };

            inline void setMat3V(float * v)
            {
                glUniformMatrix3fv(pointer, glsl_array_size, false, v);
            };

            inline void setMat4V(float * v)
            {
                glUniformMatrix4fv(pointer, glsl_array_size, false, v);
            };

            inline void setMat23V(float * v)
            {
                glUniformMatrix2x3fv(pointer, glsl_array_size, false, v);
            };

            inline void setMat32V(float * v)
            {
                glUniformMatrix3x2fv(pointer, glsl_array_size, false, v);
            };

            inline void setMat24V(float * v)
            {
                glUniformMatrix2x4fv(pointer, glsl_array_size, false, v);
            };

            inline void setMat42V(float * v)
            {
                glUniformMatrix4x2fv(pointer, glsl_array_size, false, v);
            };

            inline void setMat34V(float * v)
            {
                glUniformMatrix3x4fv(pointer, glsl_array_size, false, v);
            };

            inline void setMat43V(float * v)
            {
                glUniformMatrix4x3fv(pointer, glsl_array_size, false, v);
            };
        };

        struct SmartGLTexture : SmartGLint {
          protected:
            unsigned internal_format = GL_RGBA;
            unsigned buffer_size     = 0;
            unsigned short width     = 0;
            unsigned short height    = 0;
            unsigned short depth     = 0;
            unsigned short mip_level = 0;
            unsigned short BOUND_TO  = 0;
            unsigned char unit       = 0;
            GLenum target            = GL_TEXTURE_2D;

            virtual void deleteUnderlyingGLResource() override;

            void setData(void *, GLenum format, GLenum type);
            //	void setDataArray(void *, unsigned int = 0, unsigned int = 0);
          public:
            SmartGLTexture(GLenum target = GL_TEXTURE_2D, unsigned internal_format = GL_RGBA,
                           unsigned unit = 0, unsigned width = 1, unsigned height = 1,
                           unsigned depth = 1)
                : SmartGLint(SmartGLType::Texture), target(target),
                  internal_format(internal_format), unit(unit), width(width), height(height),
                  depth(depth)
            {
                glGenTextures(1, (GLuint *)(&pointer));
                IS_READY = (pointer != 0);
            };
            ~SmartGLTexture() { decreaseReferenceCount(); }
            SmartGLTexture(const SmartGLTexture & obj) : SmartGLint(obj)
            {
                target          = obj.target;
                width           = obj.width;
                height          = obj.height;
                depth           = obj.depth;
                unit            = obj.unit;
                internal_format = obj.internal_format;
            }

            inline void bind();

            void use(SmartGLUniform &);

            virtual void release() override;


            inline void setData(unsigned char * p, GLenum format)
            {
                setData(p, format, GL_UNSIGNED_BYTE);
            };

            inline void setData(char * p, GLenum format) { setData(p, format, GL_BYTE); };

            inline void setData(unsigned short * p, GLenum format)
            {
                setData(p, format, GL_UNSIGNED_SHORT);
            };

            inline void setData(short * p, GLenum format) { setData(p, format, GL_SHORT); };

            inline void setData(unsigned int * p, GLenum format)
            {
                setData(p, format, GL_UNSIGNED_INT);
            };

            inline void setData(int * p, GLenum format) { setData(p, format, GL_INT); };

            inline void setData(float * p, GLenum format) { setData(p, format, GL_FLOAT); };

            inline void setDataManual(void * p, GLenum format, GLenum type)
            {
                setData(p, format, type);
            };

            inline void setMagFilter(GLint filter)
            {
                bind();
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
            }

            inline void setMinFilter(GLint filter)
            {
                bind();
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
            }

            inline void setTexWrapS(GLint filter)
            {
                bind();
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, filter);
            }

            inline void setTexWrapT(GLint filter)
            {
                bind();
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, filter);
            };
        };

        struct SmartGLBuffer : SmartGLint {

          protected:
            unsigned buffer_size = 0;

            unsigned short BOUND_TO = 0;

            GLenum usage;

            bool MAPPED = false;

            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLBuffer(GLenum usage = GL_STATIC_DRAW)
                : SmartGLint(SmartGLType::Buffer), usage(usage)
            {
                glGenBuffers(1, (GLuint *)(&pointer));
                IS_READY = (pointer != 0);
            };
            ~SmartGLBuffer() { decreaseReferenceCount(); }
            SmartGLBuffer(const SmartGLBuffer & obj) : SmartGLint(obj)
            {
                buffer_size = obj.buffer_size;
                BOUND_TO    = obj.BOUND_TO;
                usage       = obj.usage;
            }
            void setData(void *, unsigned int = 0, unsigned int = 0);

            void use(SKGLB);

            virtual void release() override;

            inline bool DATA_LOADED() { return buffer_size > 0; }


            inline void * GPU_Pointer(unsigned size, GLbitfield bf)
            {
                MAPPED = true;
                glMapNamedBufferRangeEXT(pointer, 1024, 0, bf);
                void * p = glMapNamedBufferRangeEXT(pointer, 0, 1024, bf);
                if (p == NULL) {
                    throw("Unable to acquire direct mapped gl buffer.");
                    return NULL;
                }
                return p;
            }
        };


        /**
            Wrapper around an attribute int.
        */
        struct SmartGLAttribute : SmartGLint {

          protected:
            unsigned short glsl_array_size = 0;

            unsigned char glsl_object_size = 0;

            unsigned char glsl_object_type = 0;

            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLAttribute() : SmartGLint(SmartGLType::Attribute){};

            SmartGLAttribute(GLuint program_pointer, unsigned os, unsigned as, unsigned at,
                             bool ISREADY)
                : SmartGLint(SmartGLType::Attribute, program_pointer, ISREADY), glsl_array_size(as),
                  glsl_object_size(os), glsl_object_type(at){};

            ~SmartGLAttribute() { decreaseReferenceCount(); }

            SmartGLAttribute(const SmartGLAttribute & obj) : SmartGLint(obj)
            {
                glsl_array_size  = obj.glsl_array_size;
                glsl_object_size = obj.glsl_object_size;
                glsl_object_type = obj.glsl_object_type;
            }
            /**


            */
            virtual void use(SmartGLBuffer & buffer, GLenum type, unsigned stride, unsigned offset,
                             unsigned divisor = 0, bool normalize = false);

            virtual void release() override;

            virtual bool IS_USABLE() override;
        };


    } // namespace gl
} // namespace hive
