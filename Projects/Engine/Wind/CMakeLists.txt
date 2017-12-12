
SET(SOURCES
    Wind.cpp
)

SET(HEADERS
    ConsolePlugin.h
    Settings.h
    Wind.h
)

SET(DEPENDENCIES
)

add_include(Projects/Engine)

build_static_lib(EngineWind "")

