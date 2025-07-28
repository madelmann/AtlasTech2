# Projects/Arena/CMakeLists.txt - Arena Game Application

# Include base configuration
include(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

message(STATUS "Configuring Arena application...")

# Define source files based on the directory structure
set(SOURCES
    Main.cpp
    Arena.cpp
    Bullet/Bullet.cpp
    Bullet/BulletManager.cpp
    Camera/BasicCamera.cpp
    Camera/EditorCamera.cpp
    ClientSidePrediction/ClientSidePrediction.cpp
    Collision/Manager.cpp
    Editor/Editor.cpp
    Entity/Entity.cpp
    Entity/EntityManager.cpp
    Menu/MainMenu.cpp
    Network/Client/TCPClient.cpp
    Network/Client/Telegrams.cpp
    Network/Server/TCPServer.cpp
    Network/Server/Telegrams.cpp
    Player/Player.cpp
    Terrain/Manager.cpp
    Weapon/Manager.cpp
    Weapon/Weapon.cpp
)

# Define header files
set(HEADERS
    Arena.h
    Types.h
    Bullet/Bullet.h
    Bullet/BulletManager.h
    Camera/BasicCamera.h
    Camera/EditorCamera.h
    ClientSidePrediction/ClientSidePrediction.h
    ClientSidePrediction/IPredictionCorrector.h
    ClientSidePrediction/Types.h
    Collision/Manager.h
    Console/Plugins/ClientPlugin.h
    Console/Plugins/ServerPlugin.h
    Editor/Editor.h
    Entity/Entity.h
    Entity/EntityManager.h
    Entity/IEntityManager.h
    Menu/MainMenu.h
    Network/ATelegram.h
    Network/Client/IClient.h
    Network/Client/TCPClient.h
    Network/Client/Telegrams.h
    Network/ILocalPlayerManager.h
    Network/ISender.h
    Network/Server/IServer.h
    Network/Server/TCPServer.h
    Network/Server/Telegrams.h
    Network/Settings.h
    Player/Player.h
    Terrain/Manager.h
    Weapon/Manager.h
    Weapon/Weapon.h
)

# Define dependencies
set(DEPENDENCIES
    AtlasEngine
)

# Build the executable application with networking support
build_app(Arena "opengl;networking")

# Set application-specific properties
set_target_properties(Arena PROPERTIES
    OUTPUT_NAME "Arena"
    DEBUG_POSTFIX "_d"
)

# Copy configuration files to output directory
set(CONFIG_FILES
    engine.conf
    tcpclient.conf
    tcpserver.conf
)

foreach(CONFIG_FILE ${CONFIG_FILES})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_FILE})
        configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_FILE}
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG_FILE}
            COPYONLY
        )
    endif()
endforeach()

# Copy batch files for Windows
if(WIN32)
    set(BATCH_FILES
        RunAsClient.cmd
        RunAsServer.cmd
    )

    foreach(BATCH_FILE ${BATCH_FILES})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${BATCH_FILE})
            configure_file(
                ${CMAKE_CURRENT_SOURCE_DIR}/${BATCH_FILE}
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BATCH_FILE}
                COPYONLY
            )
        endif()
    endforeach()

    # Copy DLL files
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/alut.dll)
        configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/alut.dll
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/alut.dll
            COPYONLY
        )
    endif()
endif()

message(STATUS "Arena application configured successfully")
