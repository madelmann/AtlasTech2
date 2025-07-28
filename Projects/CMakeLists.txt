INCLUDE(${PROJECT_SOURCE_DIR}/CMake/Base.cmake)

# this helps debugging dependencies - a lot :)
# SET_PROPERTY(GLOBAL PROPERTY GLOBAL_DEPENDS_DEBUG_MODE 1)

# Temporarily disable complex engine components for minimal build
#ADD_SUBDIRECTORY(Engine)
#ADD_SUBDIRECTORY(ObjectiveScript)
ADD_SUBDIRECTORY(Arena)
#ADD_SUBDIRECTORY(ISCViewer)
#ADD_SUBDIRECTORY(SpaceInvaders)
#ADD_SUBDIRECTORY(TestDrive)
#ADD_SUBDIRECTORY(TestSuite)
#ADD_SUBDIRECTORY(Tools)
#ADD_SUBDIRECTORY(WorldEditor)
