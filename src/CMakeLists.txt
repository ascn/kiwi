file(GLOB KIWI_SRC
	"*.h"
	"*.cpp")

add_executable(
	main
	${KIWI_SRC})

target_link_libraries(main pthread)