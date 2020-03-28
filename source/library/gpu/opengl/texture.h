#pragma once

#include "gpu/opengl/glwrap.h"
#include "primitive/log.h"


namespace hive
{

    namespace gl
    {

        void SmartGLTexture::deleteUnderlyingGLResource() {}

        void SmartGLTexture::bind()
        {
            if (tex_2D_references[unit] == pointer)
                return;
            else
                tex_2D_references[unit] = pointer;
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(target, pointer);
        }

        void SmartGLTexture::setData(void * data, GLenum format, GLenum type)
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

        void SmartGLTexture::use(SmartGLUniform & uni)
        {
            bind();
            glUniform1i(uni.gli(), unit);
        }

        void SmartGLTexture::release() {}
    } // namespace gl
} // namespace hive