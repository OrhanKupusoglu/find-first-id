
cmake_minimum_required(VERSION 3.5)

include("../src/CMakeVersion.txt")

set(BUILD_NAME bmark-kupid)

project(${BUILD_NAME} VERSION ${BUILD_MAJOR_VER}.${BUILD_MINOR_VER}.${BUILD_PATCH_VER} LANGUAGES CXX)

#add_definitions(-DUNIT_MS)
#add_definitions(-DBMARK_TEST_SIZE=1048576)
add_definitions(-DBMARK_TEST_SIZE=$ENV{BMARK_TEST_SIZE})

get_directory_property(DirDefs COMPILE_DEFINITIONS)
message("-- Compile definitions: ${DirDefs}")

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)
message("-- CMake build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD ${BUILD_CPP_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("-- C++ standard: ${CMAKE_CXX_STANDARD}")

if(CMAKE_BUILD_TYPE STREQUAL Debug )
    message("-- C++ flags: ${CMAKE_CXX_FLAGS_DEBUG}")
else()
    message("-- C++ flags: ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

set(SOURCE_FILES "./src/benchmark.cpp")

add_executable(${BUILD_NAME} ${SOURCE_FILES})

find_package(benchmark REQUIRED)
target_link_libraries(${BUILD_NAME} benchmark::benchmark)
set_target_properties(${BUILD_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../.)
