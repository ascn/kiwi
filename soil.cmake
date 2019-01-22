include(ExternalProject)
ExternalProject_Add(soil-dl
    PREFIX            ${EXTERNAL}/soil
    URL    			  http://www.lonesock.net/files/soil.zip
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
    TEST_COMMAND      ""
    LOG_DOWNLOAD      ON
)

include_directories("${EXTERNAL}/soil/src/soil-dl/src")

add_library(soil UNKNOWN IMPORTED)
set_target_properties(soil PROPERTIES
	IMPORTED_LOCATION
	${EXTERNAL}/soil/src/soil-dl/lib/libSOIL.a)