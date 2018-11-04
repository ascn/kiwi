include(ExternalProject)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

ExternalProject_Add(glfw-dl
        PREFIX            ${EXTERNAL}/glfw
        GIT_REPOSITORY    https://github.com/glfw/glfw.git
        GIT_TAG           3.2.1
        INSTALL_COMMAND   ""
        TEST_COMMAND      ""
        LOG_DOWNLOAD      ON
        )

set(GlfwSrcDir ${EXTERNAL}/glfw/src/glfw-dl)
set(GlfwBuildDir ${EXTERNAL}/glfw/src/glfw-dl-build)

# add_subdirectory(${EXTERNAL}/glfw/src/glfw-dl)
# ExternalProject_Get_Property(glfw-dl SOURCE_DIR)
# set(GlfwIncludeDir ${SOURCE_DIR}/include)

# ExternalProject_Get_Property(glfw-dl BINARY_DIR)
# set(GlfwBuildDir ${BINARY_DIR})

# execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${EXTERNAL}/glfw/src/glfw-dl )
# if(result)
#   message(FATAL_ERROR "CMake step for glfw failed: ${result}")
# endif()
# execute_process(COMMAND ${CMAKE_COMMAND} --build .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${EXTERNAL}/glfw/src/glfw-dl )
# if(result)
#   message(FATAL_ERROR "Build step for glfw failed: ${result}")
# endif()

add_library(glfw UNKNOWN IMPORTED)
if(WIN32)
set_target_properties(glfw PROPERTIES
  	IMPORTED_LOCATION
  	${EXTERNAL}/glfw/src/glfw-dl-build/src/Debug/glfw3.lib
)
elseif(UNIX)
  set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${GlfwBuildDir}/src/${CMAKE_SHARED_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX})
endif(WIN32)