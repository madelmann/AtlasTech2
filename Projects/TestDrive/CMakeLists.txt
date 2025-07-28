# Projects/TestDrive/CMakeLists.txt - TestDrive Application

# Include base configuration
include(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

message(STATUS "Configuring TestDrive application...")

# Define source files based on the Visual Studio project
set(SOURCES
    Main.cpp
    TestDrive.cpp
    Camera/BasicCamera.cpp
    Camera/EditorCamera.cpp
    Camera/External.cpp
    Menu/MainMenu.cpp
    Player/Player.cpp
    Vehicle/Vehicle.cpp
    Vehicle/VehicleManager.cpp
)

# Define header files
set(HEADERS
    TestDrive.h
    Camera/BasicCamera.h
    Camera/EditorCamera.h
    Camera/External.h
    Menu/MainMenu.h
    Player/Player.h
    Vehicle/Vehicle.h
    Vehicle/VehicleManager.h
)

# Define dependencies
set(DEPENDENCIES
    AtlasEngine
)

# Build the executable application
build_app(TestDrive "opengl")

# Set application-specific properties
set_target_properties(TestDrive PROPERTIES
    OUTPUT_NAME "TestDrive"
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

message(STATUS "TestDrive application configured successfully")
