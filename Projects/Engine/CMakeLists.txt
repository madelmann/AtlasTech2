INCLUDE(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

#ADD_SUBDIRECTORY(Appearence)
#ADD_SUBDIRECTORY(Application)
#ADD_SUBDIRECTORY(Camera)
#ADD_SUBDIRECTORY(Clouds)
#ADD_SUBDIRECTORY(Common)
#ADD_SUBDIRECTORY(Console)
ADD_SUBDIRECTORY(Core)
#ADD_SUBDIRECTORY(Database)
#ADD_SUBDIRECTORY(Driver)
#ADD_SUBDIRECTORY(Environment)
#ADD_SUBDIRECTORY(Fog)
#ADD_SUBDIRECTORY(Font)
#ADD_SUBDIRECTORY(Frustum)
#ADD_SUBDIRECTORY(Input)
#ADD_SUBDIRECTORY(Interfaces)
#ADD_SUBDIRECTORY(JSON)
#ADD_SUBDIRECTORY(JsonNG)
#ADD_SUBDIRECTORY(Light)
#ADD_SUBDIRECTORY(LOD)
#ADD_SUBDIRECTORY(Material)
#ADD_SUBDIRECTORY(Math)
#ADD_SUBDIRECTORY(Media)
#ADD_SUBDIRECTORY(Menu)
#ADD_SUBDIRECTORY(Mesh)
#ADD_SUBDIRECTORY(Misc)
#ADD_SUBDIRECTORY(Parsers)
#ADD_SUBDIRECTORY(Physic)
#ADD_SUBDIRECTORY(Renderer)
#ADD_SUBDIRECTORY(Scene)
#ADD_SUBDIRECTORY(ScriptNG)
#ADD_SUBDIRECTORY(Shader)
#ADD_SUBDIRECTORY(Sky)
#ADD_SUBDIRECTORY(Sound)
#ADD_SUBDIRECTORY(Template)
#ADD_SUBDIRECTORY(Terrain)
#ADD_SUBDIRECTORY(Texture)
#ADD_SUBDIRECTORY(Tools)
#ADD_SUBDIRECTORY(UI)
#ADD_SUBDIRECTORY(UIKit)
#ADD_SUBDIRECTORY(Water)
#ADD_SUBDIRECTORY(Wind)

SET(SOURCES
	Engine.cpp
)

SET(HEADERS
	Engine.h
)

SET(DEPENDENCIES
)

add_include(Projects/Engine)
add_include(Projects/ObjectiveScript)

build_static_lib(Engine "")

