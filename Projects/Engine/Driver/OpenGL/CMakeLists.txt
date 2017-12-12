
ADD_SUBDIRECTORY(OpenGL)

SET(SOURCES
    Context.cpp
)

SET(HEADERS
    Context.h
)

SET(DEPENDENCIES
)

add_include(Projects/Engine)
add_include(Projects/Externals)

build_static_lib(EngineDriverOpenGL "")

