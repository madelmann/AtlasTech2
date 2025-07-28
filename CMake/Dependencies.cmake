# Dependencies.cmake - Optional dependency configuration for AtlasTech2

# This file handles optional external dependencies that may be configured
# through environment variables or system package managers

message(STATUS "Loading optional dependencies...")

# Check for user environment file (optional)
set(USER_ENV_FILE "")

# Try different possible locations for user environment file
if(WIN32)
    if(DEFINED ENV{USERPROFILE})
        set(USER_ENV_FILE "$ENV{USERPROFILE}/.dev/atlastech2.cmake")
    endif()
else()
    if(DEFINED ENV{HOME})
        set(USER_ENV_FILE "$ENV{HOME}/.dev/atlastech2.cmake")
    endif()
endif()

# Include user environment file if it exists
if(USER_ENV_FILE AND EXISTS ${USER_ENV_FILE})
    include(${USER_ENV_FILE})
    message(STATUS "Loaded user environment from: ${USER_ENV_FILE}")
else()
    message(STATUS "No user environment file found (this is optional)")
    message(STATUS "  Looked for: ${USER_ENV_FILE}")
    message(STATUS "  You can create this file to set custom library paths")
endif()

# Example content for user environment file:
# set(BUILD_BOOST_INC "/path/to/boost/include")
# set(BUILD_BOOST_LIB "/path/to/boost/lib")
# set(BUILD_CURL_INC "/path/to/curl/include")
# set(BUILD_CURL_LIB "/path/to/curl/lib")
# set(BUILD_JSON_INC "/path/to/json/include")
# set(BUILD_JSON_LIB "/path/to/json/lib")
# set(BUILD_MYSQL_INC "/path/to/mysql/include")
# set(BUILD_MYSQL_LIB "/path/to/mysql/lib")

# Platform-specific default paths
if(WIN32)
    # Windows typical locations
    if(NOT DEFINED BUILD_BOOST_INC AND EXISTS "C:/boost/include")
        set(BUILD_BOOST_INC "C:/boost/include")
        set(BUILD_BOOST_LIB "C:/boost/lib")
    endif()

    if(NOT DEFINED BUILD_CURL_INC AND EXISTS "C:/curl/include")
        set(BUILD_CURL_INC "C:/curl/include")
        set(BUILD_CURL_LIB "C:/curl/lib")
    endif()

    # Try vcpkg locations if available
    if(DEFINED ENV{VCPKG_ROOT})
        set(VCPKG_ROOT $ENV{VCPKG_ROOT})
        if(EXISTS "${VCPKG_ROOT}/installed/x86-windows/include")
            message(STATUS "Found vcpkg installation at: ${VCPKG_ROOT}")
            # vcpkg paths will be handled automatically by vcpkg toolchain
        endif()
    endif()

else()
    # Unix-like systems - use pkg-config when available
    find_package(PkgConfig QUIET)

    # Check for common system locations
    if(NOT DEFINED BUILD_BOOST_INC)
        find_path(BOOST_INCLUDE_DIR boost/version.hpp
            PATHS /usr/include /usr/local/include /opt/local/include
        )
        if(BOOST_INCLUDE_DIR)
            set(BUILD_BOOST_INC ${BOOST_INCLUDE_DIR})
        endif()
    endif()

    if(PKG_CONFIG_FOUND)
        # Use pkg-config to find libraries
        pkg_check_modules(PC_CURL QUIET libcurl)
        if(PC_CURL_FOUND AND NOT DEFINED BUILD_CURL_INC)
            set(BUILD_CURL_INC ${PC_CURL_INCLUDE_DIRS})
            set(BUILD_CURL_LIB ${PC_CURL_LIBRARY_DIRS})
        endif()

        pkg_check_modules(PC_MYSQL QUIET mysqlclient)
        if(PC_MYSQL_FOUND AND NOT DEFINED BUILD_MYSQL_INC)
            set(BUILD_MYSQL_INC ${PC_MYSQL_INCLUDE_DIRS})
            set(BUILD_MYSQL_LIB ${PC_MYSQL_LIBRARY_DIRS})
        endif()
    endif()
endif()

# Report what was found/configured
message(STATUS "Dependency configuration:")
if(DEFINED BUILD_BOOST_INC)
    message(STATUS "  Boost include: ${BUILD_BOOST_INC}")
endif()
if(DEFINED BUILD_BOOST_LIB)
    message(STATUS "  Boost lib: ${BUILD_BOOST_LIB}")
endif()
if(DEFINED BUILD_CURL_INC)
    message(STATUS "  CURL include: ${BUILD_CURL_INC}")
endif()
if(DEFINED BUILD_CURL_LIB)
    message(STATUS "  CURL lib: ${BUILD_CURL_LIB}")
endif()
if(DEFINED BUILD_JSON_INC)
    message(STATUS "  JSON include: ${BUILD_JSON_INC}")
endif()
if(DEFINED BUILD_JSON_LIB)
    message(STATUS "  JSON lib: ${BUILD_JSON_LIB}")
endif()
if(DEFINED BUILD_MYSQL_INC)
    message(STATUS "  MySQL include: ${BUILD_MYSQL_INC}")
endif()
if(DEFINED BUILD_MYSQL_LIB)
    message(STATUS "  MySQL lib: ${BUILD_MYSQL_LIB}")
endif()

# Function to check if a dependency is available
function(check_dependency name inc_var lib_var)
    set(${name}_AVAILABLE FALSE PARENT_SCOPE)

    if(DEFINED ${inc_var})
        if(EXISTS ${${inc_var}})
            set(${name}_AVAILABLE TRUE PARENT_SCOPE)
            message(STATUS "${name} dependency: Available")
        else()
            message(WARNING "${name} include path does not exist: ${${inc_var}}")
        endif()
    else()
        message(STATUS "${name} dependency: Not configured")
    endif()
endfunction()

# Check availability of configured dependencies
check_dependency(BOOST BUILD_BOOST_INC BUILD_BOOST_LIB)
check_dependency(CURL BUILD_CURL_INC BUILD_CURL_LIB)
check_dependency(JSON BUILD_JSON_INC BUILD_JSON_LIB)
check_dependency(MYSQL BUILD_MYSQL_INC BUILD_MYSQL_LIB)

message(STATUS "Dependencies loading complete")
