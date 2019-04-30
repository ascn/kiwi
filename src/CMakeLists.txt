file(GLOB_RECURSE KIWI_SRC
	"*.h"
	"*.cpp")

set (SHADOW_DEMO_DIR "/ShadowDemo/")
message(${KIWI_SRC})
foreach (TMP_PATH ${KIWI_SRC})
    string (FIND ${TMP_PATH} ${SHADOW_DEMO_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
        list (REMOVE_ITEM KIWI_SRC ${TMP_PATH})
    endif ()
endforeach(TMP_PATH)
message(${KIWI_SRC})

set (RAYMARCH_DEMO_DIR "/botw-proc/")
foreach (TMP_PATH ${KIWI_SRC})
    string (FIND ${TMP_PATH} ${RAYMARCH_DEMO_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
        list (REMOVE_ITEM KIWI_SRC ${TMP_PATH})
    endif ()
endforeach(TMP_PATH)

add_library(kiwi-lib STATIC ${KIWI_SRC})

file(GLOB_RECURSE SHADOW_DEMO_SRC
	"ShadowDemo/*.h"
	"ShadowDemo/*.cpp")

file(GLOB_RECURSE RAYMARCH_DEMO_SRC
	"botw-proc/*.h"
	"botw-proc/*.cpp")

add_executable(
	ShadowDemo
	${SHADOW_DEMO_SRC})

add_executable(
	RaymarchDemo
	${RAYMARCH_DEMO_SRC})

set_property(TARGET kiwi-lib
			 PROPERTY CXX_STANDARD 17)

set_property(TARGET ShadowDemo
			 PROPERTY CXX_STANDARD 17)

set_property(TARGET RaymarchDemo
			 PROPERTY CXX_STANDARD 17)

FIND_PACKAGE(OpenGL REQUIRED)

if (WIN32)
	target_link_libraries(kiwi-lib ${OPENGL_LIBRARIES} glew glfw assimp pthread-win)
elseif (UNIX)
	target_link_libraries(kiwi-lib assimp pthread glfw)
endif (WIN32)
add_dependencies(kiwi-lib assimp glm spdlog glew soil glfw)

add_dependencies(ShadowDemo kiwi-lib)
target_link_libraries(ShadowDemo kiwi-lib)

add_dependencies(RaymarchDemo kiwi-lib)
target_link_libraries(RaymarchDemo kiwi-lib)

if (WIN32)
	add_custom_command(TARGET ShadowDemo POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different
		"${EXTERNAL}/assimp/src/assimp-dl-build/code/Debug/assimp${LIBRARY_SUFFIX}.dll"
		$<TARGET_FILE_DIR:ShadowDemo>
	)

	add_custom_command(TARGET ShadowDemo POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different
		"${EXTERNAL}/glew/src/glew-dl/bin/Release/x64/glew32.dll"
		$<TARGET_FILE_DIR:ShadowDemo>
	)

	add_custom_command(TARGET RaymarchDemo POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different
		"${EXTERNAL}/assimp/src/assimp-dl-build/code/Debug/assimp${LIBRARY_SUFFIX}.dll"
		$<TARGET_FILE_DIR:RaymarchDemo>
	)

	add_custom_command(TARGET RaymarchDemo POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different
		"${EXTERNAL}/glew/src/glew-dl/bin/Release/x64/glew32.dll"
		$<TARGET_FILE_DIR:RaymarchDemo>
	)
endif (WIN32)