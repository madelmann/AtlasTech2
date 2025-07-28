# Extensions.cmake - Optional feature extensions for AtlasTech2

# This file handles optional feature extensions that can be enabled/disabled
# based on build configuration or available dependencies

message(STATUS "Configuring AtlasTech2 extensions...")

# Define available extensions
option(USE_JSON_EXTENSION "Enable JSON extension support" ON)
option(USE_MYSQL_EXTENSION "Enable MySQL database extension" OFF)
option(USE_SYSTEM_EXTENSION "Enable system-specific extensions" ON)
option(USE_AUDIO_EXTENSION "Enable audio/sound extensions" ON)
option(USE_PHYSICS_EXTENSION "Enable physics extensions" ON)
option(USE_SCRIPTING_EXTENSION "Enable scripting extensions" ON)
option(USE_NETWORKING_EXTENSION "Enable networking extensions" OFF)

# JSON Extension
if(USE_JSON_EXTENSION)
    add_compile_definitions(USE_JSON_EXTENSION)
    message(STATUS "  JSON extension: ENABLED")

    # Try to find a modern JSON library
    find_package(nlohmann_json QUIET)
    if(nlohmann_json_FOUND)
        message(STATUS "    Using nlohmann_json library")
        add_compile_definitions(USE_NLOHMANN_JSON)
    else()
        message(STATUS "    Using legacy JSON implementation")
    endif()
else()
    message(STATUS "  JSON extension: DISABLED")
endif()

# MySQL Extension
if(USE_MYSQL_EXTENSION)
    # Check if MySQL development libraries are available
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(MYSQL QUIET mysqlclient)
    endif()

    if(MYSQL_FOUND OR DEFINED ENV{BUILD_MYSQL_INC})
        add_compile_definitions(USE_MYSQL_EXTENSION)
        message(STATUS "  MySQL extension: ENABLED")
    else()
        message(WARNING "  MySQL extension requested but MySQL development libraries not found")
        message(STATUS "  MySQL extension: DISABLED (missing dependencies)")
        set(USE_MYSQL_EXTENSION OFF CACHE BOOL "MySQL not available" FORCE)
    endif()
else()
    message(STATUS "  MySQL extension: DISABLED")
endif()

# System Extension
if(USE_SYSTEM_EXTENSION)
    add_compile_definitions(USE_SYSTEM_EXTENSION)
    message(STATUS "  System extension: ENABLED")

    # Platform-specific system extensions
    if(WIN32)
        add_compile_definitions(USE_WINDOWS_SYSTEM_EXTENSION)
        message(STATUS "    Windows system extensions enabled")
    elseif(UNIX)
        add_compile_definitions(USE_UNIX_SYSTEM_EXTENSION)
        message(STATUS "    Unix system extensions enabled")

        if(APPLE)
            add_compile_definitions(USE_MACOS_SYSTEM_EXTENSION)
            message(STATUS "    macOS system extensions enabled")
        endif()
    endif()
else()
    message(STATUS "  System extension: DISABLED")
endif()

# Audio Extension
if(USE_AUDIO_EXTENSION)
    add_compile_definitions(USE_AUDIO_EXTENSION)
    message(STATUS "  Audio extension: ENABLED")

    # Try to find audio libraries
    if(WIN32)
        # Windows has built-in audio support
        add_compile_definitions(USE_WINDOWS_AUDIO)
    else()
        # Try to find OpenAL or other audio libraries
        find_package(PkgConfig QUIET)
        if(PKG_CONFIG_FOUND)
            pkg_check_modules(OPENAL QUIET openal)
            if(OPENAL_FOUND)
                add_compile_definitions(USE_OPENAL)
                message(STATUS "    OpenAL support enabled")
            endif()
        endif()
    endif()
else()
    message(STATUS "  Audio extension: DISABLED")
endif()

# Physics Extension
if(USE_PHYSICS_EXTENSION)
    add_compile_definitions(USE_PHYSICS_EXTENSION)
    message(STATUS "  Physics extension: ENABLED")

    # Could add detection for physics libraries like Bullet, Box2D, etc.
    message(STATUS "    Using built-in physics implementation")
else()
    message(STATUS "  Physics extension: DISABLED")
endif()

# Scripting Extension
if(USE_SCRIPTING_EXTENSION)
    add_compile_definitions(USE_SCRIPTING_EXTENSION)
    message(STATUS "  Scripting extension: ENABLED")

    # Could add detection for scripting languages like Lua, Python, etc.
    message(STATUS "    Using built-in scripting implementation")
else()
    message(STATUS "  Scripting extension: DISABLED")
endif()

# Networking Extension
if(USE_NETWORKING_EXTENSION)
    # Check for networking library dependencies
    find_package(CURL QUIET)

    if(CURL_FOUND OR DEFINED ENV{BUILD_CURL_INC})
        add_compile_definitions(USE_NETWORKING_EXTENSION)
        message(STATUS "  Networking extension: ENABLED")

        if(CURL_FOUND)
            add_compile_definitions(USE_CURL_NETWORKING)
            message(STATUS "    Using CURL for networking")
        endif()
    else()
        message(WARNING "  Networking extension requested but dependencies not found")
        message(STATUS "  Networking extension: DISABLED (missing dependencies)")
        set(USE_NETWORKING_EXTENSION OFF CACHE BOOL "Networking dependencies not available" FORCE)
    endif()
else()
    message(STATUS "  Networking extension: DISABLED")
endif()

# Debug/Development Extensions
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(USE_DEBUG_EXTENSIONS)
    message(STATUS "  Debug extensions: ENABLED")

    # Enable additional debugging features
    add_compile_definitions(ENABLE_MEMORY_DEBUGGING)
    add_compile_definitions(ENABLE_PERFORMANCE_PROFILING)
    add_compile_definitions(ENABLE_VERBOSE_LOGGING)
endif()

# Export extension status for use in other CMake files
set(ATLAS_EXTENSIONS_JSON ${USE_JSON_EXTENSION} CACHE INTERNAL "JSON extension status")
set(ATLAS_EXTENSIONS_MYSQL ${USE_MYSQL_EXTENSION} CACHE INTERNAL "MySQL extension status")
set(ATLAS_EXTENSIONS_SYSTEM ${USE_SYSTEM_EXTENSION} CACHE INTERNAL "System extension status")
set(ATLAS_EXTENSIONS_AUDIO ${USE_AUDIO_EXTENSION} CACHE INTERNAL "Audio extension status")
set(ATLAS_EXTENSIONS_PHYSICS ${USE_PHYSICS_EXTENSION} CACHE INTERNAL "Physics extension status")
set(ATLAS_EXTENSIONS_SCRIPTING ${USE_SCRIPTING_EXTENSION} CACHE INTERNAL "Scripting extension status")
set(ATLAS_EXTENSIONS_NETWORKING ${USE_NETWORKING_EXTENSION} CACHE INTERNAL "Networking extension status")

message(STATUS "Extensions configuration complete")

# Helper function to check if an extension is enabled
function(atlas_extension_enabled extension_name result_var)
    if(extension_name STREQUAL "JSON")
        set(${result_var} ${USE_JSON_EXTENSION} PARENT_SCOPE)
    elseif(extension_name STREQUAL "MYSQL")
        set(${result_var} ${USE_MYSQL_EXTENSION} PARENT_SCOPE)
    elseif(extension_name STREQUAL "SYSTEM")
        set(${result_var} ${USE_SYSTEM_EXTENSION} PARENT_SCOPE)
    elseif(extension_name STREQUAL "AUDIO")
        set(${result_var} ${USE_AUDIO_EXTENSION} PARENT_SCOPE)
    elseif(extension_name STREQUAL "PHYSICS")
        set(${result_var} ${USE_PHYSICS_EXTENSION} PARENT_SCOPE)
    elseif(extension_name STREQUAL "SCRIPTING")
        set(${result_var} ${USE_SCRIPTING_EXTENSION} PARENT_SCOPE)
    elseif(extension_name STREQUAL "NETWORKING")
        set(${result_var} ${USE_NETWORKING_EXTENSION} PARENT_SCOPE)
    else()
        set(${result_var} FALSE PARENT_SCOPE)
    endif()
endfunction()
