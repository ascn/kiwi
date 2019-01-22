cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)
project(KIWI)

set(SRC ${CMAKE_SOURCE_DIR}/src)
set(TEST ${CMAKE_SOURCE_DIR}/test)
set(EXTERNAL ${CMAKE_BINARY_DIR}/external)

# Including external dependencies
include(glm.cmake)
include(spdlog.cmake)
configure_file(assimp.cmake ${EXTERNAL}/assimp/assimp-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${EXTERNAL}/assimp/assimp-download )
if(result)
  message(FATAL_ERROR "CMake step for assimp failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${EXTERNAL}/assimp/assimp-download )
if(result)
  message(FATAL_ERROR "Build step for assimp failed: ${result}")
endif()
add_subdirectory(${EXTERNAL}/assimp/src/assimp-dl "${EXTERNAL}/assimp/src/assimp-dl-build")

include_directories("${EXTERNAL}/assimp/src/assimp-dl/include")

include(glfw.cmake)
include(pthread-win.cmake)
include(glew.cmake)
include(soil.cmake)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${GlmIncludeDir})
include_directories(${SpdlogIncludeDir})
include_directories("${EXTERNAL}/glfw/src/glfw-dl/include")

add_subdirectory(src)
add_subdirectory(test)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/src/log)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test/log)