#pragma once

#include "gpu/opengl/glwrap.hpp"
#include "primitive/log.hpp"
#include <GL/gl.h>

namespace hive
{

    using namespace gl;

    struct VRAMBuffer : SmartGLint {

      protected:
        unsigned buffer_size = 0;

        unsigned short BOUND_TO = 0;

        GLenum usage;

        bool MAPPED = false;

        virtual void deleteUnderlyingGLResource() override;

      public:
        VRAMBuffer(GLenum usage = GL_STATIC_DRAW) : SmartGLint(SmartGLType::Buffer), usage(usage)
        {

            glCreateBuffers(1, (GLuint *)(&pointer));

            GLenum error = glGetError();

            IS_READY = (error == GL_NO_ERROR && pointer != 0);

//############################## DEBUG
#ifdef HIVE_DEBUG

            switch (error) {

            case GL_INVALID_VALUE:
                // is generated if n is negative.

                __ERROR("Number of buffers to create cannot be negative");
                break;
            }

//############################## END DEBUG
#endif
        };


        ~VRAMBuffer() { decreaseReferenceCount(); }

        VRAMBuffer(const VRAMBuffer & obj) : SmartGLint(obj)
        {
            buffer_size = obj.buffer_size;
            BOUND_TO    = obj.BOUND_TO;
            usage       = obj.usage;
        }

        void setData(void *, unsigned int = 0, unsigned int = 0);

        void use(SKGLB);


        /**
         * Binds buffer data to an indexed range of GLSL elements.
         *
         * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBufferRange.xhtml
         *
         *- target
         *
         * Specify the target of the bind operation. target must be one of
         * GL_ATOMIC_COUNTER_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER, or
         * GL_SHADER_STORAGE_BUFFER. index Specify the index of the binding point within
         * the array specified by target.
         *
         *- offset
         *
         * The starting offset in basic machine units into the buffer object buffer.
         *
         *- size
         *
         * The amount of data in machine units that can be read from the buffer object
         * while used as an indexed target.
         *
         * -------------------------------------------
         *
         * Notes:
         *
         * The GL_ATOMIC_COUNTER_BUFFER target is available only if the GL version is 4.2 or
         * greater.
         *
         * The GL_SHADER_STORAGE_BUFFER target is available only if the GL version is 4.3 or
         * greater.
         */

        void useRange(const SKGLB buffer_binding, unsigned index, unsigned offset, unsigned size);
        /**
         * Bind buffer to an indexed buffer target.
         *
         * binding_point is the index of the binding point used
         *
         * SKGLB should be one of:
         *     ATOMIC_COUNTER_BUFFER
         *     SHADER_STORAGE_BUFFER
         *     TRANSFORM_FEEDBACK_BUFFER
         *     UNIFORM_BUFFER
         */
        void useBase(const SKGLB buffer_binding, unsigned binding_point);

        virtual void release() override;

        inline bool DATA_LOADED() { return buffer_size > 0; }

        inline unsigned size() { return buffer_size; }

        inline void * GPU_Pointer(unsigned size, GLbitfield bf)
        {
            MAPPED = true;

            glMapBufferRange(pointer, 1024, 0, bf);

            void * p = glMapNamedBufferRange(pointer, 0, 1024, bf);

            if (p == NULL) {
                throw("Unable to acquire direct mapped gl buffer.");

                return NULL;


                GLenum error = glGetError();

                if (error != GL_NO_ERROR) IS_READY = false;

//############################## DEBUG
#ifdef HIVE_DEBUG

                switch (error) {
                case GL_INVALID_ENUM:
                    // is generated by glMapBufferRange if target is not one of the buffer
                    // binding targets listed above.

                    __ERROR("Buffer target not valid.");
                    break;


                case GL_INVALID_VALUE:
                    /**
                     *   is generated if offset or length is negative, if offset+length is
                     *   greater than the value of GL_BUFFER_SIZE for the buffer object, or if
                     *     access has any bits set other than those defined above.
                     */

                    __ERROR("Mapped buffer size is negative, or data is larger than buffer "
                            "size, or data is too dirty.");
                    break;

                case GL_INVALID_OPERATION:

                    /**
                     * is generated by glMapNamedBufferRange if buffer is not the name of an
                     * existing buffer object.
                     *
                     * is generated by glMapBufferRange if zero is bound to target.
                     *
                     *   is generated for any of the following conditions:
                     *
                     *   length is zero.
                     *
                     *   The buffer object is already in a mapped state.
                     *
                     *   Neither GL_MAP_READ_BIT nor GL_MAP_WRITE_BIT is set.
                     *
                     *   GL_MAP_READ_BIT is set and any of GL_MAP_INVALIDATE_RANGE_BIT,
                     *   GL_MAP_INVALIDATE_BUFFER_BIT or GL_MAP_UNSYNCHRONIZED_BIT is set.
                     *
                     *   GL_MAP_FLUSH_EXPLICIT_BIT is set and GL_MAP_WRITE_BIT is not set.
                     *
                     *   Any of GL_MAP_READ_BIT, GL_MAP_WRITE_BIT, GL_MAP_PERSISTENT_BIT, or
                     *   GL_MAP_COHERENT_BIT are set, but the same bit is not included in the
                     *   buffer's storage flags.
                     */


                    __ERROR("Invalid use of buffer object: Either buffer name is 0, buffer "
                            "name\n"
                            "does not exist, or buffer is immutable.");
                    break;
                }

//############################## END DEBUG
#endif
            }

            return p;
        }
    };
} // namespace hive