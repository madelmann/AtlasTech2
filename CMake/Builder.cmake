# Builder.cmake - Modern cross-platform build functions for AtlasTech2

# Function to build a static library
function(build_static_lib target modules)
    if(NOT SOURCES)
        message(WARNING "No SOURCES defined for target ${target}")
        return()
    endif()

    # Handle modules before linking
    _handle_modules_pre_linker("${modules}")

    # Create the static library
    add_library(${target} STATIC ${SOURCES} ${HEADERS})

    # Set target properties
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    # Add include directories for this target
    target_include_directories(${target} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    )

    # Link dependencies if any
    if(DEPENDENCIES)
        target_link_libraries(${target} PUBLIC ${DEPENDENCIES})
    endif()

    # Link platform-specific libraries
    if(ATLAS_PLATFORM_LIBS)
        target_link_libraries(${target} PRIVATE ${ATLAS_PLATFORM_LIBS})
    endif()

    # Handle modules after linking
    _handle_modules_post_linker("${modules}" ${target})

    # Clear variables for next use
    unset(SOURCES PARENT_SCOPE)
    unset(HEADERS PARENT_SCOPE)
    unset(DEPENDENCIES PARENT_SCOPE)
endfunction()

# Function to build an executable application
function(build_app target modules)
    if(NOT SOURCES)
        message(WARNING "No SOURCES defined for target ${target}")
        return()
    endif()

    # Handle modules before linking
    _handle_modules_pre_linker("${modules}")

    # Create the executable
    add_executable(${target} ${SOURCES} ${HEADERS})

    # Set target properties
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )

    # Add include directories for this target
    target_include_directories(${target} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # Link dependencies
    if(DEPENDENCIES)
        target_link_libraries(${target} PRIVATE ${DEPENDENCIES})
    endif()

    # Link platform-specific libraries
    if(ATLAS_PLATFORM_LIBS)
        target_link_libraries(${target} PRIVATE ${ATLAS_PLATFORM_LIBS})
    endif()

    # Handle modules after linking
    _handle_modules_post_linker("${modules}" ${target})

    # Clear variables for next use
    unset(SOURCES PARENT_SCOPE)
    unset(HEADERS PARENT_SCOPE)
    unset(DEPENDENCIES PARENT_SCOPE)
endfunction()

# Function to build a static library for testing (includes test management utilities)
function(build_static_testlib target modules)
    # Append test management dependency
    list(APPEND DEPENDENCIES "UtilsTestManagement")

    # Use the regular static lib function
    build_static_lib(${target} "${modules}")
endfunction()

# Internal function to handle modules before linking
function(_handle_modules_pre_linker modules)
    foreach(module IN LISTS modules)
        if(module STREQUAL "boost")
            _handle_pre_boost()
        elseif(module STREQUAL "curl")
            _handle_pre_curl()
        elseif(module STREQUAL "json")
            _handle_pre_json()
        elseif(module STREQUAL "mysql")
            _handle_pre_mysql()
        elseif(module STREQUAL "opengl")
            _handle_pre_opengl()
        elseif(module STREQUAL "sdl")
            _handle_pre_sdl()
        endif()
    endforeach()
endfunction()

# Internal function to handle modules after linking
function(_handle_modules_post_linker modules target)
    foreach(module IN LISTS modules)
        if(module STREQUAL "boost")
            _handle_post_boost(${target})
        elseif(module STREQUAL "curl")
            _handle_post_curl(${target})
        elseif(module STREQUAL "json")
            _handle_post_json(${target})
        elseif(module STREQUAL "mysql")
            _handle_post_mysql(${target})
        elseif(module STREQUAL "opengl")
            _handle_post_opengl(${target})
        elseif(module STREQUAL "sdl")
            _handle_post_sdl(${target})
        endif()
    endforeach()
endfunction()

###############################
### BOOST MODULE

function(_handle_pre_boost)
    find_package(Boost QUIET COMPONENTS system filesystem thread)

    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
        message(STATUS "Found Boost: ${Boost_VERSION}")
    else()
        # Fallback to environment variables if available
        if(DEFINED ENV{BUILD_BOOST_INC})
            include_directories($ENV{BUILD_BOOST_INC})
            message(STATUS "Using Boost from BUILD_BOOST_INC: $ENV{BUILD_BOOST_INC}")
        else()
            message(WARNING "Boost not found and BUILD_BOOST_INC not set")
        endif()
    endif()
endfunction()

function(_handle_post_boost target)
    find_package(Boost QUIET COMPONENTS system filesystem thread)

    if(Boost_FOUND)
        target_link_libraries(${target} PRIVATE ${Boost_LIBRARIES})
    else()
        # Fallback to environment variables
        if(DEFINED ENV{BUILD_BOOST_LIB})
            target_link_directories(${target} PRIVATE $ENV{BUILD_BOOST_LIB})
        endif()
    endif()
endfunction()

###############################
### CURL MODULE

function(_handle_pre_curl)
    find_package(CURL QUIET)

    if(CURL_FOUND)
        include_directories(${CURL_INCLUDE_DIRS})
        message(STATUS "Found CURL: ${CURL_VERSION_STRING}")
    else()
        if(DEFINED ENV{BUILD_CURL_INC})
            include_directories($ENV{BUILD_CURL_INC})
            message(STATUS "Using CURL from BUILD_CURL_INC: $ENV{BUILD_CURL_INC}")
        else()
            message(WARNING "CURL not found and BUILD_CURL_INC not set")
        endif()
    endif()
endfunction()

function(_handle_post_curl target)
    find_package(CURL QUIET)

    if(CURL_FOUND)
        target_link_libraries(${target} PRIVATE ${CURL_LIBRARIES})
    else()
        if(DEFINED ENV{BUILD_CURL_LIB})
            target_link_directories(${target} PRIVATE $ENV{BUILD_CURL_LIB})
            target_link_libraries(${target} PRIVATE curl)
        endif()
    endif()
endfunction()

###############################
### JSON MODULE

function(_handle_pre_json)
    # Try to find a modern JSON library first
    find_package(nlohmann_json QUIET)

    if(nlohmann_json_FOUND)
        message(STATUS "Found nlohmann_json")
    else()
        # Fallback to environment variables
        if(DEFINED ENV{BUILD_JSON_INC})
            include_directories($ENV{BUILD_JSON_INC})
            message(STATUS "Using JSON from BUILD_JSON_INC: $ENV{BUILD_JSON_INC}")
        else()
            message(WARNING "JSON library not found and BUILD_JSON_INC not set")
        endif()
    endif()
endfunction()

function(_handle_post_json target)
    find_package(nlohmann_json QUIET)

    if(nlohmann_json_FOUND)
        target_link_libraries(${target} PRIVATE nlohmann_json::nlohmann_json)
    else()
        if(DEFINED ENV{BUILD_JSON_LIB})
            target_link_directories(${target} PRIVATE $ENV{BUILD_JSON_LIB})
            target_link_libraries(${target} PRIVATE Json)
        endif()
    endif()
endfunction()

###############################
### MYSQL MODULE

function(_handle_pre_mysql)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(MYSQL QUIET mysqlclient)
    endif()

    if(MYSQL_FOUND)
        include_directories(${MYSQL_INCLUDE_DIRS})
        message(STATUS "Found MySQL: ${MYSQL_VERSION}")
    else()
        if(DEFINED ENV{BUILD_MYSQL_INC})
            include_directories($ENV{BUILD_MYSQL_INC})
            message(STATUS "Using MySQL from BUILD_MYSQL_INC: $ENV{BUILD_MYSQL_INC}")
        else()
            message(WARNING "MySQL not found and BUILD_MYSQL_INC not set")
        endif()
    endif()
endfunction()

function(_handle_post_mysql target)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(MYSQL QUIET mysqlclient)
    endif()

    if(MYSQL_FOUND)
        target_link_libraries(${target} PRIVATE ${MYSQL_LIBRARIES})
        target_link_directories(${target} PRIVATE ${MYSQL_LIBRARY_DIRS})
    else()
        if(DEFINED ENV{BUILD_MYSQL_LIB})
            target_link_directories(${target} PRIVATE $ENV{BUILD_MYSQL_LIB})
            target_link_libraries(${target} PRIVATE mysqlclient)
        endif()
    endif()
endfunction()

###############################
### OPENGL MODULE

function(_handle_pre_opengl)
    find_package(OpenGL REQUIRED)
    if(OpenGL_FOUND)
        include_directories(${OPENGL_INCLUDE_DIR})
        message(STATUS "Found OpenGL")
    endif()
endfunction()

function(_handle_post_opengl target)
    find_package(OpenGL REQUIRED)
    if(OpenGL_FOUND)
        target_link_libraries(${target} PRIVATE ${OPENGL_LIBRARIES})
    endif()
endfunction()

###############################
### SDL MODULE

function(_handle_pre_sdl)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(SDL2 QUIET sdl2)
    endif()

    if(SDL2_FOUND)
        include_directories(${SDL2_INCLUDE_DIRS})
        message(STATUS "Found SDL2: ${SDL2_VERSION}")
    else()
        if(DEFINED ENV{BUILD_SDL_INC})
            include_directories($ENV{BUILD_SDL_INC})
            message(STATUS "Using SDL from BUILD_SDL_INC: $ENV{BUILD_SDL_INC}")
        else()
            message(WARNING "SDL2 not found and BUILD_SDL_INC not set")
        endif()
    endif()
endfunction()

function(_handle_post_sdl target)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(SDL2 QUIET sdl2)
    endif()

    if(SDL2_FOUND)
        target_link_libraries(${target} PRIVATE ${SDL2_LIBRARIES})
        target_link_directories(${target} PRIVATE ${SDL2_LIBRARY_DIRS})
    else()
        if(DEFINED ENV{BUILD_SDL_LIB})
            target_link_directories(${target} PRIVATE $ENV{BUILD_SDL_LIB})
            target_link_libraries(${target} PRIVATE SDL2 SDL2main)
        endif()
    endif()
endfunction()
