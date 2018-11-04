include(ExternalProject)

if(WIN32)
ExternalProject_Add(glew-dl
        PREFIX            ${EXTERNAL}/glew
        URL               https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download
        CONFIGURE_COMMAND ""
        BUILD_COMMAND     ""
        INSTALL_COMMAND   ""
        TEST_COMMAND      ""
        LOG_DOWNLOAD      ON
        )

include_directories("${EXTERNAL}/glew/src/glew-dl/include")

add_library(glew UNKNOWN IMPORTED)
set_target_properties(glew PROPERTIES
    IMPORTED_LOCATION
    ${EXTERNAL}/glew/src/glew-dl/lib/Release/x64/glew32.lib)
endif(WIN32)
