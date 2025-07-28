# Base.cmake - Cross-platform base configuration for AtlasTech2

# Set output directories if not already set
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()

if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()

if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()

# Ensure C++14 standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform and compiler specific settings
if(WIN32)
    # Windows-specific settings
    if(MSVC)
        # Visual Studio settings
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /EHsc")

        # Debug configuration
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi /RTC1 /MDd")

        # Release configuration
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob2 /DNDEBUG /MD")

        # Disable common warnings for legacy code
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996 /wd4244 /wd4267 /wd4305 /wd4018 /wd4101")

        # Define Windows version
        add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)
        add_definitions(-D_WIN32_WINNT=0x0601)  # Windows 7

    elseif(MINGW)
        # MinGW settings
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
    endif()

else()
    # Unix-like systems (Linux, macOS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long -Wnon-virtual-dtor")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -Wno-unused-variable")

    # Debug configuration
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wno-unused-function")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-value -Wno-write-strings")
        add_definitions(-DDEBUG)
    endif()

    # Release configuration
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fno-strict-aliasing")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings -Wno-unused-function")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-result -Wno-unused-value")
        add_definitions(-DNDEBUG)
    endif()
endif()

# Common definitions
add_definitions(-DLOG_LEVEL=3)

# Set include current directory
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Include common functionality
include(${CMAKE_CURRENT_LIST_DIR}/Common.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Builder.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Extensions.cmake)

# Optional: Include dependencies if file exists
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/Dependencies.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/Dependencies.cmake)
endif()

# Helper function to add global include directories
function(add_global_include_directory dir)
    get_filename_component(abs_dir ${dir} ABSOLUTE)
    include_directories(${abs_dir})
endfunction()

# Set up common include directories
add_global_include_directory(${PROJECT_SOURCE_DIR}/Projects/Engine)
add_global_include_directory(${PROJECT_SOURCE_DIR}/Projects/Engine/Interfaces)

# Platform-specific libraries
if(WIN32)
    # Windows system libraries
    set(PLATFORM_LIBS
        kernel32
        user32
        gdi32
        winspool
        comdlg32
        advapi32
        shell32
        ole32
        oleaut32
        uuid
        odbc32
        odbccp32
        opengl32
        glu32
    )
else()
    # Unix-like system libraries
    find_package(OpenGL REQUIRED)
    set(PLATFORM_LIBS
        ${OPENGL_LIBRARIES}
        pthread
        dl
        m
    )

    # Try to find additional libraries that might be needed
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(SDL2 QUIET sdl2)
        if(SDL2_FOUND)
            list(APPEND PLATFORM_LIBS ${SDL2_LIBRARIES})
        endif()
    endif()
endif()

# Export platform libraries for use in other CMake files
set(ATLAS_PLATFORM_LIBS ${PLATFORM_LIBS} CACHE INTERNAL "Platform-specific libraries")

# Debugging help
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Uncomment the next line for detailed dependency debugging
    # set_property(GLOBAL PROPERTY GLOBAL_DEPENDS_DEBUG_MODE 1)
endif()
