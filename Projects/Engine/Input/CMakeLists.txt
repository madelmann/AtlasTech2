INCLUDE(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

SET(SOURCES
    Keyboard.cpp
    KeyGrabber.cpp
    Mouse.cpp
)

SET(HEADERS
    Keyboard.h
    KeyGrabber.h
    Mouse.h
)

SET(DEPENDENCIES
)

add_include(Projects/Engine)

build_static_lib(EngineInput "")

