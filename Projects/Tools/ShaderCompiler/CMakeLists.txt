# Projects/Tools/ShaderCompiler/CMakeLists.txt - Shader Compiler Tool

# Include base configuration
include(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

message(STATUS "Configuring ShaderCompiler tool...")

# Define source files
set(SOURCES
    main.cpp
    Compiler.cpp
)

# Define header files
set(HEADERS
    Compiler.h
)

# Define dependencies
set(DEPENDENCIES
    AtlasEngine
)

# Build the executable application
build_app(ShaderCompiler "opengl")

# Set application-specific properties
set_target_properties(ShaderCompiler PROPERTIES
    OUTPUT_NAME "ShaderCompiler"
    DEBUG_POSTFIX "_d"
)

# Copy test shader files to output directory for testing
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../test.frag)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/../test.frag
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.frag
        COPYONLY
    )
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../test.vert)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/../test.vert
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.vert
        COPYONLY
    )
endif()

message(STATUS "ShaderCompiler tool configured successfully")
