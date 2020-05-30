cmake_minimum_required(VERSION 3.15)

#SET (CMAKE_CXX_STANDARD 17)

SET (CMAKE_C_FLAGS_INIT                "-std=c11")
SET (CMAKE_C_FLAGS_DEBUG_INIT          "-O0 -g4 -w")
SET (CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")


SET (CMAKE_CXX_FLAGS_INIT                "-std=c++17 -pthread -v  -fPIC ")
SET (CMAKE_CXX_FLAGS_DEBUG          	 "-O0 -g3 -std=c++17 -pthread -Wall -fcolor-diagnostics")
SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-O2 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")


#set(CMAKE_POSITION_INDEPENDENT_CODE ON)
option(JS_ONLY "Compiles to native JS (No WASM)" OFF)

SET (CMAKE_EXPORT_COMPILE_COMMANDS 1)

set (HIVE_VERSION_MAJOR 0) 
set (HIVE_VERSION_MINOR 1)
set (HIVE_VERSION_PATCH 0)


## Official emscripten information on using WebGL
## https://emscripten.org/docs/porting/multimedia_and_graphics/OpenGL-support.html


###########################################
# HIVE FRONT END JS LINKAGE
###########################################

set( PROJECT_NAME "hive_dev")

project(${PROJECT_NAME})

FILE(GLOB_RECURSE INTERFACE_DEPENDENCY_FILES "../library/include/primitive/prop_*.hpp" ) 

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/interfaces.hpp"
    DEPENDS ${INTERFACE_DEPENDENCY_FILES}
    COMMENT "Creating WebAssembly -> JavaScript interfaces.hpp file"
    COMMAND "./tools/build_webgl_components.py"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    USES_TERMINAL
)


##########################################
include_directories("../library/include")
include_directories("../library")
include_directories("./")

add_executable(${PROJECT_NAME} "hive_webassembly.cpp" "interfaces.hpp" $<TARGET_OBJECTS:hive>)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--bind -s USE_PTHREADS=1 -s DISABLE_EXCEPTION_CATCHING=2 -s USE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s USE_GLFW=3 -s WASM=1 -s EXPORTED_FUNCTIONS='[_main]'")

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".wasm.js")

set_target_properties( ${PROJECT_NAME} 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/web"
)

target_link_libraries(${PROJECT_NAME} PUBLIC hive)
