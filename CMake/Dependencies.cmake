
# Cross-platform dependency management
# Handle Linux/Unix and Windows dependencies

# Find required packages for Linux/Unix
if(UNIX)
    FIND_PACKAGE(PkgConfig REQUIRED)

    # Find SDL2 (required)
    PKG_CHECK_MODULES(SDL2 REQUIRED sdl2)

    # Find optional packages
    PKG_CHECK_MODULES(SDL2_IMAGE SDL2_image)
    if(NOT SDL2_IMAGE_FOUND)
        MESSAGE(WARNING "SDL2_image not found - image loading may not work")
    endif()

    # Find OpenGL (required)
    FIND_PACKAGE(OpenGL REQUIRED)

    # Find OpenAL (optional)
    PKG_CHECK_MODULES(OPENAL openal)
    if(NOT OPENAL_FOUND)
        MESSAGE(WARNING "OpenAL not found - audio may not work")
    endif()

    # Find threading library
    FIND_PACKAGE(Threads REQUIRED)

    # Set include directories
    INCLUDE_DIRECTORIES(
        ${SDL2_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
    )

    # Add optional include directories
    if(SDL2_IMAGE_FOUND)
        INCLUDE_DIRECTORIES(${SDL2_IMAGE_INCLUDE_DIRS})
    endif()

    if(OPENAL_FOUND)
        INCLUDE_DIRECTORIES(${OPENAL_INCLUDE_DIRS})
    endif()

    # Set library directories
    LINK_DIRECTORIES(
        ${SDL2_LIBRARY_DIRS}
    )

    if(OPENAL_FOUND)
        LINK_DIRECTORIES(${OPENAL_LIBRARY_DIRS})
    endif()

    # Set compiler flags
    ADD_DEFINITIONS(${SDL2_CFLAGS_OTHER})

else()
    # Windows dependencies - keep original user-env approach
    SET(USER_ENV "$ENV{HOME}/.dev/atlastech2")
    INCLUDE(${USER_ENV} OPTIONAL RESULT_VARIABLE found)
    if ( "${found}" STREQUAL "NOTFOUND" )
        MESSAGE(FATAL_ERROR "Your user-env file [${USER_ENV}] does not exist")
    endif()
endif()
