include(ExternalProject)
ExternalProject_Add(glm
    PREFIX            ${EXTERNAL}/glm
    GIT_REPOSITORY    https://github.com/g-truc/glm.git
    GIT_TAG           0.9.9.2
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
    TEST_COMMAND      ""
    LOG_DOWNLOAD      ON
)
ExternalProject_Get_Property(glm SOURCE_DIR)
set(GlmIncludeDir ${SOURCE_DIR}/glm)