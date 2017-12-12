INCLUDE(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

SET(SOURCES
    Application.cpp
)

SET(HEADERS
    Application.h
)

SET(DEPENDENCIES
)

add_include(Projects/Engine)

build_static_lib(EngineApplication "")

