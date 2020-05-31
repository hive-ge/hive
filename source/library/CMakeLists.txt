cmake_minimum_required(VERSION 3.15)

#SET (CMAKE_CXX_STANDARD 17)

SET (CMAKE_C_FLAGS_INIT                "-std=c11 ")
SET (CMAKE_C_FLAGS_DEBUG_INIT          "-O0 -g -w")
SET (CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")


SET (CMAKE_CXX_FLAGS_INIT                "-std=c++17 -pthread -Wall -fcolor-diagnostics")
SET (CMAKE_CXX_FLAGS_DEBUG          	 "-O0 -g -std=c++17 -pthread -Wall -fcolor-diagnostics")
SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-O2 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

SET (CMAKE_EXPORT_COMPILE_COMMANDS 1)

set (HIVE_VERSION_MAJOR 0)
set (HIVE_VERSION_MINOR 1)
set (HIVE_VERSION_PATCH 0)

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols")

###########################################
# HIVE LIBRARY
###########################################

enable_testing()

set( HIVE_LIBRARY_PROJECT "hive" )

project(${HIVE_LIBRARY_PROJECT})

SET(hive_library_base_path "${CMAKE_CURRENT_LIST_DIR}")

list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS hpp cpp)

###########################################
# LOCATE DEPENDENCIES

FILE(GLOB_RECURSE INCLUDES
	"./include/**.hpp"
) 

## Use all the *.cpp files we found under this folder for the project
FILE(GLOB_RECURSE SRCS
	"./source/boss/**.cpp"
	"./source/primitive/**.cpp"
) 

##########################################
##### GPU LIBRARIES
##########################################

if(HIVE_GRAPHICS_API STREQUAL "DirectX")
	FILE(GLOB_RECURSE GPU_SRC_INC "./gpu/direct3d/**.*" ) 
endif()

if(HIVE_GRAPHICS_API STREQUAL "Metal")
endif()


if(HIVE_GRAPHICS_API STREQUAL "WebGL")

	add_definitions(-DHIVE_USE_WEBGL=1)
#
	FILE(GLOB_RECURSE GPU_SRC_INC "./source/gpu/webgl/**.*" ) 
	LIST(APPEND GPU_SRC_INC "./source/gpu/gl/gl.cpp" )
	LIST(APPEND GPU_SRC_INC "./source/gpu/gl/gl_glfw_interface_boss.cpp")
	
endif()



if(HIVE_GRAPHICS_API STREQUAL "Vulkan")

add_definitions(-DHIVE_USE_VULKAN=1 -DHIVE_USE_GLFW)
	
	find_package(Vulkan REQUIRED)

	if(Vulkan_FOUND)
		include_directories(${Vulkan_INCLUDE_DIRS})
		link_libraries(${Vulkan_LIBRARIES})
	endif()

	link_libraries(glfw)

	FILE(GLOB_RECURSE GPU_SRC_INC "gpu/vulkan/**.*"	) 

	include_directories( "./gpu/vulkan")
endif()



if(HIVE_GRAPHICS_API STREQUAL "OpenGL" OR NOT HIVE_GRAPHICS_API)


	add_definitions(-DHIVE_USE_OPENGL -DHIVE_USE_GLFW)

	find_package(OpenGL REQUIRED)

	if (OPENGL_FOUND)
		include_directories(${OPENGL_INCLUDE_DIR})
		link_libraries(${OPENGL_LIBRARIES})
	endif()

	find_package(GLEW REQUIRED)

	if (GLEW_FOUND)
		include_directories(${GLEW_INCLUDE_DIRS})
		link_libraries(${GLEW_LIBRARIES})
	endif()

	link_libraries(glfw)

	INCLUDE_DIRECTORIES("glfw/include")

	FILE(GLOB_RECURSE GPU_SRC_INC "./source/gpu/opengl/**.*" ) 

	MESSAGE("USING OPENGL GRAPHICS API")
	MESSAGE("${GPU_SRC_INC}")
endif()



##########################################
## Add Include Objects

## Internal
INCLUDE_DIRECTORIES( ${hive_library_base_path})
## External

## Project Internal
include_directories( "./include/gpu/vulkan")
include_directories( "./include/gpu/opengl")
include_directories( "./include/gpu/webgl")
include_directories( "./include/boss")
include_directories( "./include/primitive")
include_directories( "./include")

##########################################
## Define the library
add_library(${HIVE_LIBRARY_PROJECT} STATIC ${SRCS} ${INCLUDES} ${GPU_SRC_INC})

set_property(TARGET ${HIVE_LIBRARY_PROJECT} PROPERTY POSITION_INDEPENDENT_CODE ON)

set_target_properties( ${PROJECT_NAME} 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib"
)

add_subdirectory("./test")