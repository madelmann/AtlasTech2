
SET(SOURCES
    Manager.cpp
    Water.cpp
)

SET(HEADERS
    Plugins/ConsolePlugin.h
    Manager.h
    Settings.h
    Water.h
)

SET(DEPENDENCIES
)

add_include(Projects/Engine)

build_static_lib(EngineWater "")

