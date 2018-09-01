find_package(Threads REQUIRED)

# Download and unpack googletest at configure time
configure_file(googletest.cmake ${EXTERNAL}/googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${EXTERNAL}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${EXTERNAL}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${EXTERNAL}/googletest-src
                 ${EXTERNAL}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

add_executable(
        ComponentTests
        ComponentTest.cpp
        ${SRC}/Component.cpp
        ${SRC}/TypeMap.cpp
        ${SRC}/GameObject.cpp
        ${SRC}/Logger.cpp
)
target_link_libraries(ComponentTests gtest gtest_main pthread)

add_executable(
        GameObjectTests
        GameObjectTest.cpp
        ${SRC}/Component.cpp
        ${SRC}/TypeMap.cpp
        ${SRC}/GameObject.cpp
)
target_link_libraries(GameObjectTests gtest gtest_main pthread)

add_executable(
        TransformTests
        TransformTest.cpp
        ${SRC}/Transform.cpp
        ${SRC}/Component.cpp
        ${SRC}/Logger.cpp
        ${SRC}/TypeMap.cpp
)
target_link_libraries(TransformTests gtest gtest_main pthread)
