file(GLOB_RECURSE KIWI_SRC RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	"*.h"
	"*.cpp")

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

set (SHADOW_DEMO_DIR "ShadowDemo/")
foreach (TMP_PATH ${KIWI_SRC})
    string (FIND ${TMP_PATH} ${SHADOW_DEMO_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
        list (REMOVE_ITEM KIWI_SRC ${TMP_PATH})
    endif ()
endforeach(TMP_PATH)

set (SEED_DEMO_DIR "SeedDemo/")
foreach (TMP_PATH ${KIWI_SRC})
    string (FIND ${TMP_PATH} ${SEED_DEMO_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
        list (REMOVE_ITEM KIWI_SRC ${TMP_PATH})
    endif ()
endforeach(TMP_PATH)

set (RAYMARCH_DEMO_DIR "botw-proc/")
foreach (TMP_PATH ${KIWI_SRC})
    string (FIND ${TMP_PATH} ${RAYMARCH_DEMO_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
        list (REMOVE_ITEM KIWI_SRC ${TMP_PATH})
    endif ()
endforeach(TMP_PATH)

assign_source_group(${KIWI_SRC})

add_library(kiwi-lib STATIC ${KIWI_SRC})

file(GLOB_RECURSE SHADOW_DEMO_SRC RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	"ShadowDemo/*.h"
	"ShadowDemo/*.cpp")

assign_source_group(${SHADOW_DEMO_SRC})

file(GLOB_RECURSE SEED_DEMO_SRC RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	"SeedDemo/*.h"
	"SeedDemo/*.cpp")

assign_source_group(${SEED_DEMO_SRC})

file(GLOB_RECURSE RAYMARCH_DEMO_SRC RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	"botw-proc/*.h"
	"botw-proc/*.cpp")

assign_source_group(${RAYMARCH_DEMO_SRC})

add_executable(
	ShadowDemo
	${SHADOW_DEMO_SRC})

add_executable(
	SeedDemo
	${SEED_DEMO_SRC})

add_executable(
	RaymarchDemo
	${RAYMARCH_DEMO_SRC})

set_property(TARGET kiwi-lib
			 PROPERTY CXX_STANDARD 17)

set_property(TARGET ShadowDemo
			 PROPERTY CXX_STANDARD 17)

set_property(TARGET SeedDemo
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

add_dependencies(SeedDemo kiwi-lib)
target_link_libraries(SeedDemo kiwi-lib)

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

	add_custom_command(TARGET SeedDemo POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different
		"${EXTERNAL}/assimp/src/assimp-dl-build/code/Debug/assimp${LIBRARY_SUFFIX}.dll"
		$<TARGET_FILE_DIR:SeedDemo>
	)

	add_custom_command(TARGET SeedDemo POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different
		"${EXTERNAL}/glew/src/glew-dl/bin/Release/x64/glew32.dll"
		$<TARGET_FILE_DIR:SeedDemo>
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