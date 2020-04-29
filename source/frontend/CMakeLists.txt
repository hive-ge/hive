cmake_minimum_required(VERSION 3.15)

#SET (CMAKE_CXX_STANDARD 17)

SET (CMAKE_C_FLAGS_INIT                "-std=c11")
SET (CMAKE_C_FLAGS_DEBUG_INIT          "-O0 -g -w")
SET (CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")


SET (CMAKE_CXX_FLAGS_INIT                "-std=c++17 -pthread -fPIC")
SET (CMAKE_CXX_FLAGS_DEBUG          	 "-O0 -g")
SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-O2 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")


#set(CMAKE_POSITION_INDEPENDENT_CODE ON)
SET (CMAKE_EXPORT_COMPILE_COMMANDS 1)
#set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols")

set (ruminate_VERSION_MAJOR 0)
set (ruminate_VERSION_MINOR 1)
set (ruminate_VERSION_PATCH 0)

# define NPI_VERSION
add_definitions(-DNAPI_VERSION=5)

###########################################
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


###########################################
# HIVE FRONT END JS LINKAGE
###########################################

set( PROJECT_NAME "hive_dev")

project(${PROJECT_NAME})

##########################################
## Define the library
include_directories("../library")

include_directories(${CMAKE_JS_INC})

add_library(${PROJECT_NAME} SHARED "hive_frontend.cpp" "./interfaces.hpp" ${CMAKE_JS_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

target_link_libraries(${PROJECT_NAME} hive)





