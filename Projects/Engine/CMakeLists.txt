# Projects/Engine/CMakeLists.txt - AtlasTech2 Engine Library

# Include base configuration
include(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

message(STATUS "Configuring AtlasTech2 Engine...")

# Automatically find all source files
file(GLOB_RECURSE SOURCES
    "*.cpp"
)

# Automatically find all header files
file(GLOB_RECURSE HEADERS
    "*.h"
)

# Filter out any files that might not exist or cause issues
list(FILTER SOURCES EXCLUDE REGEX ".*unused.*")
list(FILTER HEADERS EXCLUDE REGEX ".*unused.*")

# Remove any empty entries
list(REMOVE_ITEM SOURCES "")
list(REMOVE_ITEM HEADERS "")

# Print number of files found for debugging
list(LENGTH SOURCES SOURCE_COUNT)
list(LENGTH HEADERS HEADER_COUNT)
message(STATUS "Found ${SOURCE_COUNT} source files and ${HEADER_COUNT} header files")

# Define dependencies (will be populated based on what's needed)
set(DEPENDENCIES)

# Create the static library
add_library(AtlasEngine STATIC ${SOURCES} ${HEADERS})

# Set library properties
set_target_properties(AtlasEngine PROPERTIES
    OUTPUT_NAME "AtlasEngine"
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Configure include directories for the target
target_include_directories(AtlasEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Interfaces>
    $<INSTALL_INTERFACE:include>
)

# Platform-specific configurations
if(WIN32)
    # Windows-specific libraries and settings
    target_link_libraries(AtlasEngine PRIVATE
        opengl32
        glu32
        gdi32
        user32
        kernel32
        winmm
    )

    # Add Windows-specific compile definitions
    target_compile_definitions(AtlasEngine PRIVATE
        WIN32
        _WINDOWS
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
else()
    # Unix-like systems
    find_package(OpenGL REQUIRED)
    if(OpenGL_FOUND)
        target_link_libraries(AtlasEngine PRIVATE ${OPENGL_LIBRARIES})
        target_include_directories(AtlasEngine PRIVATE ${OPENGL_INCLUDE_DIRS})
    endif()

    # Additional Unix libraries
    target_link_libraries(AtlasEngine PRIVATE
        pthread
        dl
        m
    )
endif()

# Optional dependencies (only link if available)
# These will be handled by the module system in Builder.cmake

# Debug/Release specific settings
target_compile_definitions(AtlasEngine PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# Export the target for use by other projects
set_property(TARGET AtlasEngine PROPERTY EXPORT_NAME Engine)

message(STATUS "AtlasTech2 Engine configured successfully")
message(STATUS "  Sources: ${SOURCE_COUNT} files")
message(STATUS "  Headers: ${HEADER_COUNT} files")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
