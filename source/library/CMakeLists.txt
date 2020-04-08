cmake_minimum_required(VERSION 3.15)

#SET (CMAKE_CXX_STANDARD 17)

SET (CMAKE_C_FLAGS_INIT                "-std=c11 ")
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

set (HIVE_VERSION_MAJOR 0)
set (HIVE_VERSION_MINOR 1)
set (HIVE_VERSION_PATCH 0)


###########################################
# HIVE LIBRARY
###########################################

set( HIVE_LIBRARY_PROJECT "hive" )

project(${HIVE_LIBRARY_PROJECT})

SET(hive_library_base_path "${CMAKE_CURRENT_LIST_DIR}")

list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS hpp cpp)

###########################################
# LOCATE DEPENDENCIES
#link_libraries(${CMAKE_DL_LIBS})

#find_package(OpenGL REQUIRED)

if (OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
	link_libraries(${OPENGL_LIBRARIES})
	
    link_libraries(glfw)
    INCLUDE_DIRECTORIES("glfw/include")
endif()

find_package(GLEW REQUIRED)

if (GLEW_FOUND)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})
endif()


find_package(ZLIB REQUIRED)

if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    link_libraries(${ZLIB_LIBRARIES})
endif()

#find_package(Vulkan REQUIRED)

## Use all the *.cpp files we found under this folder for the project
FILE(GLOB SRCS
	"./3rdparty/libspng/spng.c" 
	"./*.cpp"
 	"./audio/**/*.cpp"
	"./filesystem/**/*.cpp"
	"./gpu/**/*.cpp"
	"./interface/**/*.cpp"
	"./network/**/*.cpp"
	"./primitive/**/*.cpp"
	"./script/**/*.cpp"
) 

FILE(GLOB INCLUDES
	"./3rdparty/libspng/spng.h" 
	"./*.h*"
 	"./audio/**/*.h*"
	"./filesystem/**/*.h*"
	"./gpu/**/*.h*"
	"./interface/**/*.h*"
	"./network/**/*.h*"
	"./primitive/**/*.h*"
	"./script/**/*.h*"
) 

##########################################
## Add Include Objects

## Internal
INCLUDE_DIRECTORIES( ${hive_library_base_path})
## External
#INCLUDE_DIRECTORIES(${Vulkan_INCLUDE_DIR})

## Project Internal
include_directories( "./3rdparty")
include_directories( "./3rdparty/libspng")
include_directories( "./audio")
include_directories( "./filesystem")

include_directories( "./gpu")
include_directories( "./gpu/opengl")

include_directories("./resource")

include_directories("./interface")
include_directories("./interface/input/keyboard")
include_directories("./interface/input/mouse")
include_directories("./interface/input/controller")

include_directories( "./network")
include_directories( "./script")
include_directories( "./primitive/math")
include_directories( "./script")
include_directories( "./node")

##########################################
## Define the library

add_library(${HIVE_LIBRARY_PROJECT} ${SRCS} ${INCLUDES})
