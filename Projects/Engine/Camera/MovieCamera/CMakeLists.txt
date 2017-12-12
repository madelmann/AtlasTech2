
SET(SOURCES
    MovieCamera.cpp
    TimeLine.h
)

SET(HEADERS
    Action.h
    MovieCamera.h
    Settings.h
    TimeLine.h
)

SET(DEPENDENCIES
    EngineMath
)

add_include(Projects/Engine)

build_static_lib(EngineCameraMovieCamera "")

