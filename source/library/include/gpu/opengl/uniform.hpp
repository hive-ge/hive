#pragma once

#include "gpu/opengl/glwrap.hpp"

namespace hive
{

    namespace gl
    {

        struct SmartGLUniform : SmartGLint {
          protected:
            // Primitive type
            unsigned primitive     = 0;
            unsigned type          = 0;
            unsigned size          = 0;
            unsigned bytes         = 0;
            unsigned length        = 1;
            unsigned flags         = 0;
            unsigned array_size    = 0;
            unsigned offset        = 0;
            unsigned block_index   = 0;
            unsigned array_stride  = 0;
            unsigned matrix_stride = 0;
            unsigned is_row_major  = 0;
            unsigned location      = 0;
            unsigned index         = 0;

            virtual void deleteUnderlyingGLResource() override;

          public:
            SmartGLUniform() : SmartGLint(SmartGLType::Uniform){};

            SmartGLUniform(GLuint program_pointer, int index)
                : SmartGLint(SmartGLType::Uniform, program_pointer, true)
            {
                const unsigned int properties[] = {
                    GL_TYPE,                                 // int
                    GL_ARRAY_SIZE,                           // int
                    GL_OFFSET,                               // int
                    GL_BLOCK_INDEX,                          // int
                    GL_ARRAY_STRIDE,                         // int
                    GL_MATRIX_STRIDE,                        // int
                    GL_IS_ROW_MAJOR,                         // int
                    GL_LOCATION,                             // int
                    GL_REFERENCED_BY_VERTEX_SHADER,          // int 0 | 1
                    GL_REFERENCED_BY_TESS_CONTROL_SHADER,    // int 0 | 1
                    GL_REFERENCED_BY_TESS_EVALUATION_SHADER, // int 0 | 1
                    GL_REFERENCED_BY_GEOMETRY_SHADER,        // int 0 | 1
                    GL_REFERENCED_BY_FRAGMENT_SHADER,        // int 0 | 1
                    GL_REFERENCED_BY_COMPUTE_SHADER,         // int 0 | 1
                };

                const int prop_count = sizeof(properties) >> 2;
                int actual;
                int vals[prop_count];
                ;

                glGetProgramResourceiv(program_pointer, GL_UNIFORM, index, prop_count, properties,
                                       prop_count, &actual, vals);
                SmartGLUniform::index = index;
                type                  = vals[0];
                primitive             = getGLSLTypePrimitive(type);
                size                  = getGLSLTypeElementCount(type);
                bytes                 = getGLSLTypeSize(type);
                array_size            = vals[1];
                offset                = vals[2];
                block_index           = vals[3];
                array_stride          = vals[4];
                matrix_stride         = vals[5];
                is_row_major          = vals[6];
                location              = vals[7];
                pointer               = location;
            };

            ~SmartGLUniform() { decreaseReferenceCount(); }

            SmartGLUniform(const SmartGLUniform & obj) : SmartGLint(obj)
            {
                type          = obj.type;
                primitive     = obj.primitive;
                size          = obj.size;
                bytes         = obj.bytes;
                array_size    = obj.array_size;
                offset        = obj.offset;
                block_index   = obj.block_index;
                array_stride  = obj.array_stride;
                matrix_stride = obj.matrix_stride;
                is_row_major  = obj.is_row_major;
                location      = obj.location;
            }
            virtual void use();
            virtual void release() override;
            virtual bool IS_USABLE() override;


            inline friend void operator<<(SmartGLUniform & uni, hive::math::mat44 mat)
            {
                uni.setMat4V(&mat.m1);
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

            inline void setFloat1V(float * v) { glUniform1fv(pointer, length, v); };

            inline void setFloat2V(float * v) { glUniform2fv(pointer, length, v); };

            inline void setFloat3V(float * v) { glUniform3fv(pointer, length, v); };

            inline void setFloat4V(float * v) { glUniform4fv(pointer, length, v); };

            inline void setInt(int v) { glUniform1i(pointer, v); };

            inline void setInt(int v1, int v2) { glUniform2i(pointer, v1, v2); };

            inline void setInt(int v1, int v2, int v3) { glUniform3i(pointer, v1, v2, v3); };

            inline void setInt(int v1, int v2, int v3, int v4)
            {
                glUniform4i(pointer, v1, v2, v3, v4);
            }

            inline void setInt1V(int * v) { glUniform1iv(pointer, length, v); };

            inline void setInt2V(int * v) { glUniform2iv(pointer, length, v); };

            inline void setInt3V(int * v) { glUniform3iv(pointer, length, v); };

            inline void setInt4V(int * v) { glUniform4iv(pointer, length, v); };

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

            inline void setUint1V(unsigned int * v) { glUniform1uiv(pointer, length, v); };

            inline void setUint2V(unsigned int * v) { glUniform2uiv(pointer, length, v); };

            inline void setUint3V(unsigned int * v) { glUniform3uiv(pointer, length, v); };

            inline void setUint4V(unsigned int * v) { glUniform4uiv(pointer, length, v); };

            inline void setMat2V(float * v) { glUniformMatrix2fv(pointer, length, false, v); };

            inline void setMat3V(float * v) { glUniformMatrix3fv(pointer, length, false, v); };

            inline void setMat4V(float * v) { glUniformMatrix4fv(pointer, length, false, v); };

            inline void setMat23V(float * v) { glUniformMatrix2x3fv(pointer, length, false, v); };

            inline void setMat32V(float * v) { glUniformMatrix3x2fv(pointer, length, false, v); };

            inline void setMat24V(float * v) { glUniformMatrix2x4fv(pointer, length, false, v); };

            inline void setMat42V(float * v) { glUniformMatrix4x2fv(pointer, length, false, v); };

            inline void setMat34V(float * v) { glUniformMatrix3x4fv(pointer, length, false, v); };

            inline void setMat43V(float * v) { glUniformMatrix4x3fv(pointer, length, false, v); };
        };
    } // namespace gl
} // namespace hive