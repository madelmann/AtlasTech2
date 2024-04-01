
SET(SOURCES
    Manager.cpp
)

SET(HEADERS
    ConsolePlugin.h
    Manager.h
    Settings.h
)

SET(DEPENDENCIES
    EngineMath
    EngineDriver
)

add_include(Projects/Engine)

build_static_lib(EngineClouds "")

