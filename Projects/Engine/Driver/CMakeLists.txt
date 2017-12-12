
#ADD_SUBDIRECTORY(OpenGL)

SET(SOURCES
    Driver.cpp
    DriverInformation.cpp
    DriverUtils.cpp
)

SET(HEADERS
    Driver.h
    DriverExtensions.h
    DriverInformation.h
    DriverUtils.h
)

SET(DEPENDENCIES
    EngineDriverOpenGL
)

add_include(Projects/Engine)
add_include(Projects/Externals)

build_static_lib(EngineDriver "")

