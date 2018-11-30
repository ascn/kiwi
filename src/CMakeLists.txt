file(GLOB_RECURSE KIWI_SRC
	"*.h"
	"*.cpp")

add_executable(
	main
	${KIWI_SRC})

set_property(TARGET main
			 PROPERTY CXX_STANDARD 17)

FIND_PACKAGE(OpenGL REQUIRED)

if (WIN32)
	target_link_libraries(main ${OPENGL_LIBRARIES} glew glfw assimp pthread-win)
elseif (UNIX)
	target_link_libraries(main assimp pthread glfw)
endif (WIN32)
add_dependencies(main assimp glm spdlog glew)


if (WIN32)
	add_custom_command(TARGET main POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different
		"${EXTERNAL}/assimp/src/assimp-dl-build/code/Debug/assimp${LIBRARY_SUFFIX}.dll"
		$<TARGET_FILE_DIR:main>
	)

	add_custom_command(TARGET main POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different
		"${EXTERNAL}/glew/src/glew-dl/bin/Release/x64/glew32.dll"
		$<TARGET_FILE_DIR:main>
	)
endif (WIN32)