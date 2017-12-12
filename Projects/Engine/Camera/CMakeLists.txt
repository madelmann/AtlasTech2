
ADD_SUBDIRECTORY(MovieCamera)

SET(SOURCES
    Viewer.cpp
)

SET(HEADERS
    Viewer.h
)

SET(DEPENDENCIES
    EngineMath
    EngineDriver
)

add_include(Projects/Engine)

build_static_lib(EngineCamera "")

