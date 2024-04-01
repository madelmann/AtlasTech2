INCLUDE(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

SET(SOURCES
    Files.cpp
    SharedPtr.cpp
    Strings.cpp
    Tools.cpp
)

SET(HEADERS
    Files.h
    SharedPtr.h
    Strings.h
    Tools.h
)

SET(DEPENDENCIES
)

add_include(Projects/Engine)

build_static_lib(EngineTools "")

