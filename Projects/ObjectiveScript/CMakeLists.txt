SET(CURRENT_MODULE "ObjectiveScript")
MESSAGE("[ ${CURRENT_MODULE} ]")

# Include directories
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ObjectiveScript
    ${PROJECT_SOURCE_DIR}/Projects/Engine
)

# Collect source files
FILE(GLOB_RECURSE OBJECTIVESCRIPT_SOURCES
    "ObjectiveScript/*.cpp"
)

FILE(GLOB_RECURSE OBJECTIVESCRIPT_HEADERS
    "ObjectiveScript/*.h"
)

# Create static library
ADD_LIBRARY(${CURRENT_MODULE} STATIC ${OBJECTIVESCRIPT_SOURCES} ${OBJECTIVESCRIPT_HEADERS})

# Set library properties
SET_TARGET_PROPERTIES(${CURRENT_MODULE} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
)

# Link dependencies (if any)
TARGET_LINK_LIBRARIES(${CURRENT_MODULE}
    # Add any dependencies here if needed
)

# Compiler definitions
TARGET_COMPILE_DEFINITIONS(${CURRENT_MODULE} PRIVATE
    # Add any compile definitions here if needed
)
