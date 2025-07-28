# Projects/SpaceInvaders/CMakeLists.txt - SpaceInvaders Game Application

# Include base configuration
include(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

message(STATUS "Configuring SpaceInvaders application...")

# Define source files based on the Visual Studio project
set(SOURCES
    Main.cpp
    SpaceInvaders.cpp
    Bullet/Bullet.cpp
    Bullet/BulletManager.cpp
    Camera/BasicCamera.cpp
    Camera/EditorCamera.cpp
    Collision/Manager.cpp
    Game/Game.cpp
    NPC/Manager.cpp
    NPC/NPC.cpp
    Screens/Credits.cpp
    Screens/MainMenu.cpp
)

# Define header files
set(HEADERS
    SpaceInvaders.h
    Types.h
    Bullet/Bullet.h
    Bullet/BulletManager.h
    Camera/BasicCamera.h
    Camera/EditorCamera.h
    Collision/Manager.h
    Game/Game.h
    NPC/Manager.h
    NPC/NPC.h
    Screens/Credits.h
    Screens/MainMenu.h
)

# Define dependencies
set(DEPENDENCIES
    AtlasEngine
)

# Build the executable application
build_app(SpaceInvaders "opengl")

# Set application-specific properties
set_target_properties(SpaceInvaders PROPERTIES
    OUTPUT_NAME "SpaceInvaders"
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

message(STATUS "SpaceInvaders application configured successfully")
