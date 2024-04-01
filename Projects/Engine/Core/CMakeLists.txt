# INCLUDE(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

ADD_SUBDIRECTORY( Common )
ADD_SUBDIRECTORY( Configuration )

SET(SOURCES
	Common/Viewport.cpp
	Configuration/Configurator.cpp
	Engine.cpp
	EngineSettings.cpp
	EngineStates.cpp
	GeneralManager.cpp
	IdGenerator.cpp
)

SET(HEADERS
	Common/Viewport.h
	Configuration/Configurator.h
	Plugins/ConsolePlugin.h
	Engine.h
	EngineSettings.h
	EngineStates.h
	GeneralManager.h
	IdGenerator.h
	Settings.h
)

SET(DEPENDENCIES
)

add_include(Projects/Engine)
add_include(Projects/Externals)

build_static_lib(EngineCore "")

