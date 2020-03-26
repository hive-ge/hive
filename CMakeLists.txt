cmake_minimum_required(VERSION 3.15)

#SET (CMAKE_CXX_STANDARD 17)

SET (CMAKE_C_FLAGS_INIT                "-std=c99")
SET (CMAKE_C_FLAGS_DEBUG_INIT          "-O0 -g -Wall")
SET (CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")


SET (CMAKE_CXX_FLAGS_INIT                "-stdlib=libc++ -std=c++17 -fdeclspec -pthread")
SET (CMAKE_CXX_FLAGS_DEBUG          	 "-O0 -g -Wall")
SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-O2 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

set (ruminate_VERSION_MAJOR 0)
set (ruminate_VERSION_MINOR 1)
set (ruminate_VERSION_PATCH 0)

## Use the variable PROJECT_NAME for changing the target name
set( PROJECT_LIBRARY_NAME "hive_library" )

#set(Boost_INCLUDE_DIRS, "/usr/include/boost")
#set(Boost_LIBRARY_DIRS, "/usr/lib/boost")

## Set our project name
project(${PROJECT_LIBRARY_NAME})

###USE BOOST LIBRARY
#find_package(Boost
#	# Minimum or EXACT version e.g. 1.67.0
#  1.65      			
#  # Fail with error if Boost is not found
#  REQUIRED             		
#  # Boost libraries by their canonical name
#  COMPONENTS system
#  # e.g. "date_time" for "libboost_date_time"
#  )  

find_package(OpenGL)

# set(Boost_USE_MULTITHREADED ON)

list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS h)

## Use all the *.cpp files we found under this folder for the project
FILE(GLOB SRCS
	"/source/library/*"
 	"/source/library/audio/**/*"
	"/source/library/filesystem/**/*"
	"/source/library/gpu/**/*"
	"/source/library/interface/**/*"
	"/source/library/network/**/*"
	"/source/library/primitive/**/*"
	"/source/library/script/**/*"
) 

SET(BASEPATH "${CMAKE_SOURCE_DIR}/library")

INCLUDE_DIRECTORIES("${BASEPATH}")
INCLUDE_DIRECTORIES(OPENGL_INCLUDE_DIR)


## Add Include Directories
include_directories(${PROJECT_LIBRARY_NAME} PRIVATE "/source/library")
include_directories(${PROJECT_LIBRARY_NAME} PRIVATE "/source/library/audio")
include_directories(${PROJECT_LIBRARY_NAME} PRIVATE "/source/library/filesystem")
include_directories(${PROJECT_LIBRARY_NAME} PRIVATE "/source/library/gpu")
include_directories(${PROJECT_LIBRARY_NAME} PRIVATE "/source/library/interface")
include_directories(${PROJECT_LIBRARY_NAME} PRIVATE "/source/library/network")
include_directories(${PROJECT_LIBRARY_NAME} PRIVATE "/source/library/script")
include_directories(${PROJECT_LIBRARY_NAME} PRIVATE "/source/library/primitive/math")
include_directories(${PROJECT_LIBRARY_NAME} PRIVATE "/source/library/script")
include_directories(${PROJECT_LIBRARY_NAME} PRIVATE "/source/library/node")

#if($BOOST_FOUND)
#	include_directories(${PROJECT_LIBRARY_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
#endif()

## Define the executable
add_library(${PROJECT_LIBRARY_NAME} STATIC source/library/hive.cpp ${SRCS})

target_link_libraries( ${PROJECT_LIBRARY_NAME} OPENGL_LIBRARIES )

#target_link_libraries(${PROJECT_LIBRARY_NAME} Compiler FileDB ${Boost_LIBRARIES})

set_target_properties(${PROJECT_LIBRARY_NAME} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "C:\test")

#SIMPLE PLATFORMER LIBRARY
set( PROJECT_NAME "platformer")

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} "source/samples/platformer.cpp")


