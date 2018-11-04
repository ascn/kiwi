cmake_minimum_required(VERSION 2.8.2)

project(assimp-dl NONE)

include(ExternalProject)
ExternalProject_Add(assimp-dl
        PREFIX            ${EXTERNAL}/assimp
        GIT_REPOSITORY    https://github.com/assimp/assimp.git
        GIT_TAG           v4.1.0
        BUILD_COMMAND     ""
        #CONFIGURE_COMMAND ""
        INSTALL_COMMAND   ""
        TEST_COMMAND      ""
        LOG_DOWNLOAD      ON
        )

# execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${EXTERNAL}/assimp/src/assimp-dl )
# if(result)
#   message(FATAL_ERROR "CMake step for assimp failed: ${result}")
# else()
#   message("CMake step for assimp succeeded")
# endif()
# execute_process(COMMAND ${CMAKE_COMMAND} --build .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${EXTERNAL}/assimp/src/assimp-dl )
# if(result)
#   message(FATAL_ERROR "Build step for assimp failed: ${result}")
# endif()

add_library(assimp UNKNOWN IMPORTED)
if(WIN32)
set_target_properties(assimp PROPERTIES
  	IMPORTED_LOCATION
  	${EXTERNAL}/assimp/src/assimp-dl-build/code/Debug/assimp-vc140-mt.lib)
elseif(UNIX)
set_target_properties(assimp PROPERTIES
	IMPORTED_LOCATION
	${EXTERNAL}/assimp/src/assimp-dl-build/code/${CMAKE_SHARED_LIBRARY_PREFIX}assimp${CMAKE_SHARED_LIBRARY_SUFFIX})
endif(WIN32)


add_custom_command(TARGET assimp-dl POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different
	"${EXTERNAL}/assimp/src/assimp-dl-build/include/assimp/config.h"
	"${EXTERNAL}/assimp/src/assimp-dl/include/assimp"
)