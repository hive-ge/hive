cmake_minimum_required(VERSION 3.15)

#SET (CMAKE_CXX_STANDARD 17)

SET (CMAKE_C_FLAGS_INIT                "-std=c11")
SET (CMAKE_C_FLAGS_DEBUG_INIT          "-O0 -g -w")
SET (CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")


SET (CMAKE_CXX_FLAGS_INIT                "-std=c++17 -pthread")
SET (CMAKE_CXX_FLAGS_DEBUG          	 "-O0 -g")
SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-O2 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

SET (CMAKE_EXPORT_COMPILE_COMMANDS 1)

set (ruminate_VERSION_MAJOR 0)
set (ruminate_VERSION_MINOR 1)
set (ruminate_VERSION_PATCH 0)

project("HIVE_SYSTEM")

#### PLATFORM SPECIFIC FLAGS

set(HIVE_USE_WEBGL 1)

include(CTest)

if(NOT HIVE_LIBRARY_MODE)

    MESSAGE(WARNING "HIVE_LIBRARY_MODE IS UNDEFINED \n SELECTING LBRARY MODE BASED ON CURRENT OPERATING SYSTEM")

    if (WIN32 OR MSVC)
        MESSAGE("HIVE_LIBRARY_MODE set to [windows]")
        set(HIVE_LIBRARY_MODE  windows)
    endif()

    if (UNIX)
        MESSAGE("HIVE_LIBRARY_MODE set to [linux]")
        set(HIVE_LIBRARY_MODE "linux")
    endif()

    if(APPLE)
        MESSAGE("HIVE_LIBRARY_MODE set to [apple]")
        set(HIVE_LIBRARY_MODE apple)
    endif()
endif()


if(HIVE_LIBRARY_MODE STREQUAL "WASM")

    set(CMAKE_CREATE_WASM TRUE)

    set(HIVE_GRAPHICS_API "WebGL")

    MESSAGE("CREATING PROJECT FOR WEBASSEMBLY")

elseif(HIVE_LIBRARY_MODE STREQUAL "linux")

    set(HIVE_GRAPHICS_API "OpenGL")

    MESSAGE("CREATING PROJECT FOR LINUX")

elseif(HIVE_LIBRARY_MODE STREQUAL "windows" OR HIVE_LIBRARY_MODE STREQUAL "xbox")

    set(HIVE_GRAPHICS_API "DirectX")

    if(HIVE_LIBRARY_MODE STREQUAL "windows")

        MESSAGE("CREATING PROJECT FOR WIDOWS")

    else()

        MESSAGE("CREATING PROJECT FOR XBOX")

    endif()

elseif(HIVE_LIBRARY_MODE STREQUAL "apple")

    set(HIVE_GRAPHICS_API "Metal")

    MESSAGE("CREATING PROJECT FOR APPLE")
else()
    MESSAGE( FATAL_ERROR "Unrecognized HIVE_LIBRARY_MODE [${HIVE_LIBRARY_MODE}]")
endif()


if(HIVE_USE_VULKAN)
    set(HIVE_GRAPHICS_API "Vulkan")
endif()

add_subdirectory("./source/library")

if (CMAKE_CREATE_NODE_EDITOR)
    add_subdirectory("./source/frontend")
endif()

if (CMAKE_CREATE_WASM)
    add_subdirectory("./source/webassembly")
endif()
