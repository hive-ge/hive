#pragma once

#include "gpu/opengl/glwrap.h"

namespace hive
{

    namespace gl
    {

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

        void SmartGLUniform::deleteUnderlyingGLResource() {}

        void SmartGLUniform::use() {}

        void SmartGLUniform::release() {}

        bool SmartGLUniform::IS_USABLE() { return false; }
    } // namespace gl
} // namespace hive