SET(CURRENT_MODULE "Arena")
MESSAGE("[ ${CURRENT_MODULE} ]")

# Find required packages
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(SDL2 REQUIRED sdl2)

# Find OpenGL
FIND_PACKAGE(OpenGL REQUIRED)

# Include directories
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/Projects/Engine
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)

# For minimal Arena, we only need one source file
SET(ARENA_SOURCES
    ArenaMinimal.cpp
)

# Create executable
ADD_EXECUTABLE(${CURRENT_MODULE} ${ARENA_SOURCES})

# Link libraries - minimal dependencies only
TARGET_LINK_LIBRARIES(${CURRENT_MODULE}
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    -lm
    -lpthread
)

# Compiler flags
TARGET_COMPILE_OPTIONS(${CURRENT_MODULE} PRIVATE ${SDL2_CFLAGS_OTHER})

# Set output properties
SET_TARGET_PROPERTIES(${CURRENT_MODULE} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
)

# Print build information
MESSAGE(STATUS "Building minimal Arena with SDL2 ${SDL2_VERSION}")
