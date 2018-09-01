cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)
project(KIWI)

set(SRC ${CMAKE_SOURCE_DIR}/src)
set(TEST ${CMAKE_SOURCE_DIR}/test)
set(EXTERNAL ${CMAKE_BINARY_DIR}/external)

include(glm.cmake)
include(spdlog.cmake)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${GlmIncludeDir})
include_directories(${SpdlogIncludeDir})

add_subdirectory(src)
add_subdirectory(test)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/log)