# Projects/WorldEditor/CMakeLists.txt - WorldEditor Application

# Include base configuration
include(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

message(STATUS "Configuring WorldEditor application...")

# Define source files based on the Visual Studio project
set(SOURCES
    Main.cpp
    WorldEditor.cpp
    Camera/BasicCamera.cpp
    Camera/EditorCamera.cpp
    Menu/MainMenu.cpp
    Renderer/HUDRenderer.cpp
    Renderer/MenuRenderer.cpp
)

# Define header files
set(HEADERS
    WorldEditor.h
    Camera/BasicCamera.h
    Camera/EditorCamera.h
    Menu/MainMenu.h
    Renderer/HUDRenderer.h
    Renderer/MenuRenderer.h
)

# Define dependencies
set(DEPENDENCIES
    AtlasEngine
)

# Build the executable application
build_app(WorldEditor "opengl")

# Set application-specific properties
set_target_properties(WorldEditor PROPERTIES
    OUTPUT_NAME "WorldEditor"
    DEBUG_POSTFIX "_d"
)

# Copy configuration files to output directory
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/engine.conf)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/engine.conf
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine.conf
        COPYONLY
    )
endif()

# Copy DLL files for Windows
if(WIN32 AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/alut.dll)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/alut.dll
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/alut.dll
        COPYONLY
    )
endif()

message(STATUS "WorldEditor application configured successfully")
