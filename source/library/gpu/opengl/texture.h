#pragma once

#ifdef HIVE_USE_OPENGL

#include "gpu/opengl/glwrap.h"
#include "gpu/opengl/uniform.h"
#include "primitive/log.h"
#include <gl3w/GL/glcorearb.h>

namespace hive
{

    using namespace gl;

    struct Texture : SmartGLint {

      protected:
        unsigned internal_format = GL_RGBA;

        unsigned buffer_size = 0;

        unsigned short width = 0;

        unsigned short height = 0;

        unsigned short depth = 0;

        unsigned short mip_level = 0;

        unsigned short BOUND_TO = 0;

        unsigned char unit = 0;

        GLenum target = GL_TEXTURE_2D;

      protected:
        virtual void deleteUnderlyingGLResource() override;

        //	void setDataArray(void *, unsigned int = 0, unsigned int = 0);
      public:
        Texture(GLenum target = GL_TEXTURE_2D, unsigned internal_format = GL_RGBA,
                unsigned unit = 0, unsigned width = 1, unsigned height = 1, unsigned depth = 1)
            : SmartGLint(SmartGLType::Texture), target(target), internal_format(internal_format),
              unit(unit), width(width), height(height), depth(depth)
        {
            glGenTextures(1, (GLuint *)(&pointer));
            IS_READY = (pointer != 0);
        };

        void setData(void *, GLenum format, GLenum type);

        ~Texture() { decreaseReferenceCount(); }

        Texture(const Texture & obj) : SmartGLint(obj)
        {
            target          = obj.target;
            width           = obj.width;
            height          = obj.height;
            depth           = obj.depth;
            unit            = obj.unit;
            internal_format = obj.internal_format;
        }

        inline void bind(unsigned unit = 0);

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
        /**
         * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindImageTexture.xhtml
         *
         * Access:
         *  One of GL_READ_ONLY, GL_WRITE_ONLY, or GL_READ_WRITE
         */
        inline void bindImageTexture(int access = GL_READ_WRITE, int format = GL_RGBA32F);
    };
    void Texture::deleteUnderlyingGLResource() {}

    void Texture::bind(unsigned unit)
    {
        if (tex_2D_references[unit] == pointer)
            return;
        else
            tex_2D_references[unit] = pointer;

        glActiveTexture(GL_TEXTURE0 + unit);

        glBindTexture(GL_TEXTURE_2D, pointer);
    }

    void Texture::bindImageTexture(int access, int format)
    {
        if (tex_2D_references[unit] == pointer)
            return;
        else
            tex_2D_references[unit] = pointer;

        glActiveTexture(GL_TEXTURE0 + unit);

        glBindImageTexture(GL_TEXTURE0 + unit, pointer, mip_level, GL_FALSE, 0, access,
                           internal_format);
    }

    void Texture::setData(void * data, GLenum format, GLenum type)
    {
        if (!IS_READY || width * height * depth == 0) {
            __ERROR("GL texture is not ready to use.", 0, "buffer.cpp", __LINE__);
            return; // throw error
        }

        bind();

        if (buffer_size == 0) {
            glTexImage2D(target, 0, internal_format, width, height, 0, format, type, data);
            buffer_size = width * height * depth * sizeof(unsigned char);
        }
    }

    void Texture::use(SmartGLUniform & uni)
    {
        bind();
        glUniform1i(uni.gli(), unit);
    }

    void Texture::release() {}

} // namespace hive

#endif