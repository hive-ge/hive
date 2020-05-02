#pragma once

#include "./texture.hpp"

namespace hive
{

    using namespace gl;

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