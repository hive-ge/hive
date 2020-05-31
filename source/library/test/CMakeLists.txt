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


set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test )

include_directories("../library/include")

FILE(GLOB_RECURSE TEST_UNITS "*.cpp" ) 


 foreach(TEST_PATH IN LISTS TEST_UNITS)

    string(REGEX REPLACE "([^\\\\/]*[\\\\/])+" "" FILE_NAME_WITH_EXT ${TEST_PATH})

    string(REGEX REPLACE "\\..*" "" TEST_NAME ${FILE_NAME_WITH_EXT})

    add_executable(${TEST_NAME} ${TEST_PATH})

    target_link_libraries(${TEST_NAME} PUBLIC hive)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR})

    set_tests_properties(${TEST_NAME} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed")

    set_tests_properties(${TEST_NAME} PROPERTIES FAIL_REGULAR_EXPRESSION "(Exception|Test Failed)")

    set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 120)

endforeach()


