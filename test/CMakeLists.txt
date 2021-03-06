
cmake_minimum_required(VERSION 3.5)

include("../src/CMakeVersion.txt")

set(BUILD_NAME test-kupid)

project(${BUILD_NAME} VERSION ${BUILD_MAJOR_VER}.${BUILD_MINOR_VER}.${BUILD_PATCH_VER} LANGUAGES CXX)

#add_definitions(-DTEST_XLARGE)

get_directory_property(DirDefs COMPILE_DEFINITIONS)
message("++ Compile definitions: ${DirDefs}")

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)
message("++ CMake build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD ${BUILD_CPP_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("++ C++ standard: ${CMAKE_CXX_STANDARD}")

if(CMAKE_BUILD_TYPE STREQUAL Debug )
    message("++ C++ flags: ${CMAKE_CXX_FLAGS_DEBUG}")
else()
    message("++ C++ flags: ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

find_first_id()

set(SOURCE_FILES "./src/main.cpp"
                 "./src/test_kbtree.cpp"
                 "./src/test_kbset.cpp"
                 "./src/test_kvector.cpp"
                 "./src/test_kset_inc.cpp"
                 "./src/test_kset_dec.cpp")

set(TEST_ARGS "")

add_executable(${BUILD_NAME} ${SOURCE_FILES})

find_package(GTest REQUIRED)
target_link_libraries(${BUILD_NAME} GTest::GTest GTest::Main)
set_target_properties(${BUILD_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../.)

#include(GoogleTest)
#
#gtest_discover_tests(${BUILD_NAME}
#                     EXTRA_ARGS ${TEST_ARGS}
#                     WORKING_DIRECTORY ../.
#                     TEST_PREFIX Test
#)
