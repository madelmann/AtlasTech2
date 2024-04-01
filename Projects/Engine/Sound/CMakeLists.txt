INCLUDE(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

SET(SOURCES
    AudioResource.cpp
    BaseSound.cpp
    Manager.cpp
    Sound.cpp
    Sound2D.cpp
    Sound3D.cpp
)

SET(HEADERS
    AudioResource.h
    BaseSound.h
    Manager.h
    Sound.h
    Sound2D.h
    Sound3D.h
)

SET(DEPENDENCIES
)

add_include(Projects/Engine)

build_static_lib(EngineSound "")

