#include "./buffer.hpp"

using namespace hive;
using namespace hive::gl;

void VRAMBuffer::deleteUnderlyingGLResource() { glDeleteBuffers(1, (GLuint *)(&pointer)); }

inline void VRAMBuffer::useBase(const SKGLB buffer_binding, unsigned binding_point)
{

//############################## DEBUG
#ifdef HIVE_DEBUG
    if (!IS_READY) {
        //  __ERROR("GL buffer is not ready to use.", 0, "buffer.cpp", __LINE__);
        return; // throw error
    }

    clearErrors();

//############################## END DEBUG
#endif

    switch (buffer_binding) {

    case (SKGLB::ATOMIC_COUNTER_BUFFER):
        if (SKGL_ATOMIC_COUNTER_BUFFER_SET != pointer) {
            SKGL_ATOMIC_COUNTER_BUFFER_SET = pointer;
            glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, binding_point, pointer);
            BOUND_TO |= SKGLB::ATOMIC_COUNTER_BUFFER;
        }
        break;

    case (SKGLB::SHADER_STORAGE_BUFFER):
        if (SKGL_SHADER_STORAGE_BUFFER_SET != pointer) {
            SKGL_SHADER_STORAGE_BUFFER_SET = pointer;
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point, pointer);
            BOUND_TO |= SKGLB::SHADER_STORAGE_BUFFER;
        }
        break;

    case (SKGLB::TRANSFORM_FEEDBACK_BUFFER):
        if (SKGL_TRANSFORM_FEEDBACK_BUFFER_SET != pointer) {
            SKGL_TRANSFORM_FEEDBACK_BUFFER_SET = pointer;
            glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, binding_point, pointer);
            BOUND_TO |= SKGLB::TRANSFORM_FEEDBACK_BUFFER;
        }
        break;

    case (SKGLB::UNIFORM_BUFFER):
        if (SKGL_UNIFORM_BUFFER_SET != pointer) {
            SKGL_UNIFORM_BUFFER_SET = pointer;
            glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, pointer);
            BOUND_TO |= SKGLB::UNIFORM_BUFFER;
        }
        break;
    default:

        IS_READY = false;

//############################## DEBUG
#ifdef HIVE_DEBUG
        __ERROR("Buffer target not valid for this operation.");

//############################## END DEBUG
#endif

        return;
    }


//############################## DEBUG
#ifdef HIVE_DEBUG

    GLenum error = glGetError();

    if (error != GL_NO_ERROR) IS_READY = false;

    switch (error) {
    case GL_INVALID_ENUM:
        // GL_INVALID_ENUM is generated if target is not GL_ATOMIC_COUNTER_BUFFER,
        // GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER or GL_SHADER_STORAGE_BUFFER.
        __ERROR("Buffer target not valid. Needs to be one of GL_ATOMIC_COUNTER_BUFFER,"
                " GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER or "
                "GL_SHADER_STORAGE_BUFFER.");
        break;


    case GL_INVALID_VALUE:
        // GL_INVALID_VALUE is generated if index is greater than or equal to the number of
        // target-specific indexed binding points. GL_INVALID_VALUE is generated if buffer
        // does not have an associated data store, or if the size of that store is zero.

        if (buffer_size <= 0) {
            __ERROR("Buffer store is empty.");
        } else {
            __ERROR("Index is out of range");
        }
        break;
    }

//############################## END DEBUG
#endif
}

void VRAMBuffer::useRange(const SKGLB buffer_binding, unsigned index, unsigned offset,
                          unsigned size)
{

//############################## DEBUG
#ifdef HIVE_DEBUG
    if (!IS_READY) {
        //__ERROR("GL buffer is not ready to use.", 0, "buffer.cpp", __LINE__);
        return; // throw error
    }
//############################## END DEBUG
#endif

    switch (buffer_binding) {

    case (SKGLB::ATOMIC_COUNTER_BUFFER):
        if (SKGL_ATOMIC_COUNTER_BUFFER_SET != pointer) use(ATOMIC_COUNTER_BUFFER);

        glBindBufferRange(GL_ATOMIC_COUNTER_BUFFER, index, pointer, offset, size);
        break;

    case (SKGLB::SHADER_STORAGE_BUFFER):
        if (SKGL_SHADER_STORAGE_BUFFER_SET != pointer) use(SHADER_STORAGE_BUFFER);

        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, index, pointer, offset, size);
        break;

    case (SKGLB::TRANSFORM_FEEDBACK_BUFFER):
        if (SKGL_TRANSFORM_FEEDBACK_BUFFER_SET != pointer) use(TRANSFORM_FEEDBACK_BUFFER);

        glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, index, pointer, offset, size);
        break;

    case (SKGLB::UNIFORM_BUFFER):
        if (SKGL_UNIFORM_BUFFER_SET != pointer) use(UNIFORM_BUFFER);

        glBindBufferRange(GL_UNIFORM_BUFFER, index, pointer, offset, size);
        break;
    default:

        IS_READY = false;

//############################## DEBUG
#ifdef HIVE_DEBUG
        __ERROR("Buffer target not valid for this operation.");

//############################## END DEBUG
#endif

        return;
    }

//############################## DEBUG
#ifdef HIVE_DEBUG


    GLenum error = glGetError();

    if (error != GL_NO_ERROR) IS_READY = false;

    switch (error) {
    case GL_INVALID_ENUM:
        // GL_INVALID_ENUM is generated if target is not one of GL_ATOMIC_COUNTER_BUFFER,
        // GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER or GL_SHADER_STORAGE_BUFFER.

        __ERROR("Buffer target not valid.");
        break;


    case GL_INVALID_VALUE:

        // GL_INVALID_VALUE is generated if index is greater than or equal to the number of
        // target-specific indexed binding points.

        // GL_INVALID_VALUE is generated if size is less than or equal to zero, or if offset
        // + size is greater than the value of GL_BUFFER_SIZE.
        if (size <= 0) {
            __ERROR("Size is less than or equal to zero.");
        } else if (size + offset > buffer_size) {
            __ERROR("Buffer size" + std::to_string(buffer_size) + "  is smaller than the range: " +
                    std::to_string(offset) + "to: " + std::to_string(size + offset));
        } else {
            int alignment = 0;

            switch (buffer_binding) {

            case (SKGLB::SHADER_STORAGE_BUFFER):
                glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &alignment);
                break;

            case (SKGLB::UNIFORM_BUFFER):
                glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
                break;
            default:
                break;
            }

            if (offset % alignment && alignment > 0) {
                __ERROR("Offset alignment is incorrect, alignment should be on " +
                        std::to_string(alignment) + " aligned boundaries.");
            } else {

                __ERROR("Index is is greater or equal to the number of target specified "
                        "binding points");
            }
        }

        break;
    }

//############################## END DEBUG
#endif
}

inline void VRAMBuffer::use(const SKGLB buffer_binding)
{

//############################## DEBUG
#ifdef HIVE_DEBUG
    if (!IS_READY) {
        __ERROR("GL buffer is not ready to use.", 0, "buffer.cpp", __LINE__);
        return; // throw error
    }

    clearErrors();
//############################## END DEBUG
#endif

    switch (buffer_binding) {
    case (SKGLB::ARRAY_BUFFER):
        if (SKGL_ARRAY_BUFFER_SET != pointer) {
            SKGL_ARRAY_BUFFER_SET = pointer;
            glBindBuffer(GL_ARRAY_BUFFER, pointer);
            BOUND_TO |= SKGLB::ARRAY_BUFFER;
        }
        break;

    case (SKGLB::ATOMIC_COUNTER_BUFFER):
        if (SKGL_ATOMIC_COUNTER_BUFFER_SET != pointer) {
            SKGL_ATOMIC_COUNTER_BUFFER_SET = pointer;
            glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, pointer);
            BOUND_TO |= SKGLB::ATOMIC_COUNTER_BUFFER;
        }
        break;

    case (SKGLB::COPY_READ_BUFFER):
        if (SKGL_COPY_READ_BUFFER_SET != pointer) {
            SKGL_COPY_READ_BUFFER_SET = pointer;
            glBindBuffer(GL_COPY_READ_BUFFER, pointer);
            BOUND_TO |= SKGLB::COPY_READ_BUFFER;
        }
        break;

    case (SKGLB::COPY_WRITE_BUFFER):
        if (SKGL_COPY_WRITE_BUFFER_SET != pointer) {
            SKGL_COPY_WRITE_BUFFER_SET = pointer;
            glBindBuffer(GL_COPY_WRITE_BUFFER, pointer);
            BOUND_TO |= SKGLB::COPY_WRITE_BUFFER;
        }
        break;

    case (SKGLB::DISPATCH_INDIRECT_BUFFER):
        if (SKGL_DISPATCH_INDIRECT_BUFFER_SET != pointer) {
            SKGL_DISPATCH_INDIRECT_BUFFER_SET = pointer;
            glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, pointer);
            BOUND_TO |= SKGLB::DISPATCH_INDIRECT_BUFFER;
        }
        break;

    case (SKGLB::DRAW_INDIRECT_BUFFER):
        if (SKGL_DRAW_INDIRECT_BUFFER_SET != pointer) {
            SKGL_DRAW_INDIRECT_BUFFER_SET = pointer;
            glBindBuffer(GL_DRAW_INDIRECT_BUFFER, pointer);
            BOUND_TO |= SKGLB::DRAW_INDIRECT_BUFFER;
        }
        break;

    case (SKGLB::ELEMENT_ARRAY_BUFFER):
        if (SKGL_ELEMENT_ARRAY_BUFFER_SET != pointer) {
            SKGL_ELEMENT_ARRAY_BUFFER_SET = pointer;
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pointer);
            BOUND_TO |= SKGLB::ELEMENT_ARRAY_BUFFER;
        }
        break;

    case (SKGLB::PIXEL_PACK_BUFFER):
        if (SKGL_PIXEL_PACK_BUFFER_SET != pointer) {
            SKGL_PIXEL_PACK_BUFFER_SET = pointer;
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pointer);
            BOUND_TO |= SKGLB::PIXEL_PACK_BUFFER;
        }
        break;

    case (SKGLB::PIXEL_UNPACK_BUFFER):
        if (SKGL_PIXEL_UNPACK_BUFFER_SET != pointer) {
            SKGL_PIXEL_UNPACK_BUFFER_SET = pointer;
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pointer);
            BOUND_TO |= SKGLB::PIXEL_UNPACK_BUFFER;
        }
        break;

    case (SKGLB::QUERY_BUFFER):
        if (SKGL_QUERY_BUFFER_SET != pointer) {
            SKGL_QUERY_BUFFER_SET = pointer;
            glBindBuffer(GL_QUERY_BUFFER, pointer);
            BOUND_TO |= SKGLB::QUERY_BUFFER;
        }
        break;

    case (SKGLB::SHADER_STORAGE_BUFFER):
        if (SKGL_SHADER_STORAGE_BUFFER_SET != pointer) {
            SKGL_SHADER_STORAGE_BUFFER_SET = pointer;
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, pointer);
            BOUND_TO |= SKGLB::SHADER_STORAGE_BUFFER;
        }
        break;

    case (SKGLB::TEXTURE_BUFFER):
        if (SKGL_TEXTURE_BUFFER_SET != pointer) {
            SKGL_TEXTURE_BUFFER_SET = pointer;
            glBindBuffer(GL_TEXTURE_BUFFER, pointer);
            BOUND_TO |= SKGLB::TEXTURE_BUFFER;
        }
        break;

    case (SKGLB::TRANSFORM_FEEDBACK_BUFFER):
        if (SKGL_TRANSFORM_FEEDBACK_BUFFER_SET != pointer) {
            SKGL_TRANSFORM_FEEDBACK_BUFFER_SET = pointer;
            glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, pointer);
            BOUND_TO |= SKGLB::TRANSFORM_FEEDBACK_BUFFER;
        }
        break;

    case (SKGLB::UNIFORM_BUFFER):
        if (SKGL_UNIFORM_BUFFER_SET != pointer) {
            SKGL_UNIFORM_BUFFER_SET = pointer;
            glBindBuffer(GL_UNIFORM_BUFFER, pointer);
            BOUND_TO |= SKGLB::UNIFORM_BUFFER;
        }
        break;
    }


#ifdef HIVE_DEBUG //############################## DEBUG
    GLenum error = glGetError();

    if (error != GL_NO_ERROR) IS_READY = false;

    switch (error) {
    case GL_INVALID_ENUM:
        // is generated by glBufferData if target is not one of the
        // accepted buffer targets.

        // is generated if usage is not GL_STREAM_DRAW, GL_STREAM_READ,
        // GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY,
        // GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY.

        __ERROR("Buffer target not valid.");
        break;


    case GL_INVALID_VALUE:
        // is generated if buffer is not a name previously returned from a call to
        // glGenBuffers.


        __ERROR("Buffer name is not valid.");
        break;
    }

//############################## END DEBUG
#endif
}

void VRAMBuffer::release() {}

/**
 * Loads buffer data using glNamedBufferData if data has not been previously
 * set, or glNamedBufferSubData if the buffer already contains data.
 *
 * Reads and reports gl errors HIVE_DEBUG mode.
 */
void VRAMBuffer::setData(void * data, unsigned int size, unsigned int offset)
{

//############################## DEBUG
#ifdef HIVE_DEBUG
    if (!IS_READY) {
        //__ERROR("GL buffer is not ready to use.", 0, "buffer.cpp", __LINE__);
        return; // throw error
    }

    clearErrors();
//############################## END DEBUG
#endif

    if (buffer_size == 0 || buffer_size < size + offset) {
        glNamedBufferData(pointer, size, data, usage);
        buffer_size = size + offset;
    } else {
        glNamedBufferSubData(pointer, offset, size, data);
    }

//############################## DEBUG
#ifdef HIVE_DEBUG
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) IS_READY = false;

    switch (error) {
    case GL_INVALID_ENUM:
        // is generated by glBufferData if target is not one of the
        // accepted buffer targets.

        // is generated if usage is not GL_STREAM_DRAW, GL_STREAM_READ,
        // GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY,
        // GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY.

        __ERROR("Buffer target not valid.");
        break;


    case GL_INVALID_VALUE:
        // is generated if size is negative.

        __ERROR("Size of buffer cannot be negative");
        break;

    case GL_INVALID_OPERATION:
        // is generated by glBufferData if the reserved buffer object
        // name 0 is bound to target.

        // is generated by glNamedBufferData if buffer is not the
        // name of an existing buffer object.

        // is generated if the GL_BUFFER_IMMUTABLE_STORAGE flag of
        // the buffer object is GL_TRUE.

        __ERROR("Invalid use of buffer object: Either buffer name is 0, buffer name\n"
                "does not exist, or buffer is immutable.");
        break;

    case GL_OUT_OF_MEMORY:
        // is generated if the GL is unable to create a data store with
        // the specified size.

        __ERROR("Size of buffer cannot be negative");
        break;
    }

//############################## END DEBUG
#endif
}