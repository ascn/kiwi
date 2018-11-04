include(ExternalProject)
ExternalProject_Add(spdlog
    PREFIX            ${EXTERNAL}/spdlog
    GIT_REPOSITORY    https://github.com/gabime/spdlog.git
    GIT_TAG           v1.1.0
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
    TEST_COMMAND      ""
    LOG_DOWNLOAD      ON
)
ExternalProject_Get_Property(spdlog SOURCE_DIR)
set(SpdlogIncludeDir ${SOURCE_DIR}/include)