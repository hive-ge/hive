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

add_subdirectory("./source/library")
add_subdirectory("./source/frontend")

###########################################
# SIMPLE PLATFORMER EXE
###########################################

#set( PROJECT_NAME "platformer")

#project(${PROJECT_NAME})

###########################################
# LOCATE DEPENDENCIES


#find_package(Vulkan REQUIRED)
#INCLUDE_DIRECTORIES("source/library")
#INCLUDE_DIRECTORIES("source/sample")
#INCLUDE_DIRECTORIES("source/electron_frontend")

#INCLUDE_DIRECTORIES(${Vulkan_INCLUDE_DIR})


##########################################
## Define the executable

#add_executable(${PROJECT_NAME} "source/samples/platformer.cpp")

#target_link_libraries( ${PROJECT_NAME} hive)

