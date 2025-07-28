# Projects/Tools/CMakeLists.txt - Tools directory for AtlasTech2

# Include base configuration
include(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

message(STATUS "Configuring AtlasTech2 Tools...")

# Add ShaderCompiler tool
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/ShaderCompiler/CMakeLists.txt)
    add_subdirectory(ShaderCompiler)
endif()

# Add AEMeshViewer tool if it exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/AEMeshViewer/CMakeLists.txt)
    add_subdirectory(AEMeshViewer)
endif()

message(STATUS "Tools configured successfully")
