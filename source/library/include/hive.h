#pragma once

#ifndef HIVE_HEADER_H
#define HIVE_HEADER_H

#define HIVE_MAJOR_VERSION 0
#define HIVE_MINOR_VERSION 1
#define HIVE_PATCH_VERSION 2

#ifndef HIVE_APP_MAJOR_VERSION
#define HIVE_APP_MAJOR_VERSION 0
#endif

#ifndef HIVE_APP_MINOR_VERSION
#define HIVE_APP_MINOR_VERSION 0
#endif

#ifndef HIVE_APP_PATCH_VERSION
#define HIVE_APP_PATCH_VERSION 0
#endif

#include "include/primitive/core_debug.hpp"
#include "include/primitive/core_type_information.hpp"

// MATHS Stuff
#include "include/primitive/math_line.hpp"
#include "include/primitive/math_mat33.hpp"
#include "include/primitive/math_mat44.hpp"
#include "include/primitive/math_qbezier.hpp"
#include "include/primitive/math_quaternion.hpp"
#include "include/primitive/math_vec2.hpp"
#include "include/primitive/math_vec3.hpp"
#include "include/primitive/math_vec4.hpp"

// Interface - Accessed through the interface boss.
#include "include/primitive/interface_controller.hpp"
#include "include/primitive/interface_keyboard.hpp"
#include "include/primitive/interface_mouse.hpp"

// Bosses
#include "include/boss/boss.hpp"
#include "include/boss/interface_boss.hpp"
#include "include/boss/message_boss.hpp"
#include "include/boss/render_boss.hpp"

// Primitives
#include "include/primitive/core_drone.hpp"
#include "include/primitive/core_math.hpp"
#include "include/primitive/core_prop.hpp"
// Primitive Props
#include "include/primitive/prop_gpu_program.hpp"
#include "include/primitive/prop_image.hpp"
#include "include/primitive/prop_matrix.hpp"
#include "include/primitive/prop_mesh.hpp"
#include "include/primitive/prop_renderable.hpp"
#include "include/primitive/prop_vector.hpp"
// GPU Headers

// -- VULKAN
#include "include/gpu/vulkan/vulkan_wrap.hpp"

// -- WEBGL
#ifdef HIVE_USE_WEBGL
#include "include/gpu/webgl/webgl.hpp"
#endif

// -- Opengl

#ifdef HIVE_USE_OPENGL
#include "include/gpu/opengl/gl.hpp"
#endif
// -- DirectX

/*


PROP SYSTEM:

    TAGS for user and system defined orginization of drones. These may be reinforced by specifying
    certain other props that must be present for the tag to be valid.

    Out-of-band property descriptors


Script System
    Updates and queries of object props

Data Storage

    Data storage should be stored in monolith data files that read props directly from memory into
the file data. Shamelessly cop Blender's file storage system for performance.

Addressing System

        Props and drones use and internal addressing system to send data arround. The address is a
64 byte value that represents several bits of information.
        [[ StorageSystem 8 bits ][ GlobalAddress - 32 bits ][ StorageLocalAddress - 24 bits ]]

d object should be found. Stores proved their own lookup and redirect
    methods to allow objects to be moved between systems to allow optimal memory placement. Stores
themselves are accessed through a lookup system that provides a means to move whole blocks of data
elements around to optimize storage layout for different systems such as video data, mesh data, and
audio sampling data. If a store does not have an object, a global lookup operation can be performed
to locate an object. If the object is still not located with this operation, then the object is
deleted, and the reference should either be destroyed or made inactive.


        All addresses can be looked up by the storage local address. The global address is a 32 bit
    unique identifier that all objects MUST store. This represents the total ordering of all objects
    during a runtime session, and must be unique to allow global lookup of any object at any point
of operation.

        The StorageLocalAddress can be thought of the local index into the store which the object
    resides. It is a convience cached value, and is subject to change should there be a need to
rearange the under lying data. If the object is moved, a dereference of the handle will cause the
handle to be updated to point to the new address. This does imply a lookup of the existing object at
the StorageLocalAddress location for every derefenec, so other methods should be used to ensure that
    extrenous comparison operations are not performed during usual execution.

Message System

        Scripts, props, and bosses send messages to each other asynchrounously. Each message is a
light weight object that allows a large number of different data types can be described using the
message systems data structure, which provides a 32byte primitive for storing data. Message blocks
can be combined inorder to send larger data messages.

GRAPHIC SYSTEMS
    RENDER PIPELINE

    MAPPER
        Maps HIVE side data to GPU data

    GPU PROGRAM

        RENDER_FRAMES

            Additional Render Targets to Textures for later presentation to
            screen.

        PROGRAM STAGES
            **WEBGL COMPATIBLE**
            VERTEX
            FRAGEMENT
            **VULKAN, DIRECTX, OPENGL COMPATIBLE **
            Geometry
            Tesselation

        DEDICATED COMPUTE PROGRAMS

        PROGRAM INPUTS

            BUFFERS (TEXTURES, VERTICES)
                TEXTURES - SPECIAL GPU systems for texture lookup, mip maps, 3D and Cube Textures
                Vertex Data - Any Layout

            PROGRAM UNIFORMS
                Uniform buffers - Particular useful for compute

        PROGRAM OUTPUTS
            Buffers - Compute shader for instance
            TEXTURE
            RENDER BUFFER

        Submitting Data

            Submit Uniforms -
                Camera / Light View Matrices
                Object Model View matrices
                Material Uniforms
                Local Light data.
                Texture Bindings
                Alpha Blending


            Starting Render Pass
                DirectX - DrawIndexedPrimitive
                OpenGL/Vulcan - glDrawArrays


        Submission Details
            back to front ordering ( for alpha primitives )
            instancing
            batch rendering (for matching materials)
            culling - High level culling outside shaders - Fragment culling


    TEXTURES
        UPLOAD TO GPU
        DYNAMIC UPDATE (video or realtime editing)

    UNIFORMS
        Data structs
*/

namespace hive
{
    namespace all
    {
        using namespace hive;
        // using namespace hive::graphic;
        using namespace hive::interface;
        using namespace hive::math;
        // using namespace gl;
    } // namespace all
} // namespace hive

#endif