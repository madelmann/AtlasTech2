# Projects/CMakeLists.txt - Main projects directory for AtlasTech2

# Include base configuration
include(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

message(STATUS "Building AtlasTech2 projects...")

# Add the main engine library first (other projects depend on it)
add_subdirectory(Engine)

# Add external libraries and dependencies
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Externals/CMakeLists.txt)
    add_subdirectory(Externals)
endif()

# Add other projects
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/WorldEditor/CMakeLists.txt)
    add_subdirectory(WorldEditor)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/TestDrive/CMakeLists.txt)
    add_subdirectory(TestDrive)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/SpaceInvaders/CMakeLists.txt)
    add_subdirectory(SpaceInvaders)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ISCViewer/CMakeLists.txt)
    add_subdirectory(ISCViewer)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Arena/CMakeLists.txt)
    add_subdirectory(Arena)
endif()

# Add tools
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMakeLists.txt)
    add_subdirectory(Tools)
endif()

# Add test suite
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/TestSuite/CMakeLists.txt)
    add_subdirectory(TestSuite)
endif()

# Add ObjectiveScript if it exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ObjectiveScript/CMakeLists.txt)
    add_subdirectory(ObjectiveScript)
endif()

# Add documentation if it exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Docs/CMakeLists.txt)
    add_subdirectory(Docs)
endif()

message(STATUS "All available projects configured")
