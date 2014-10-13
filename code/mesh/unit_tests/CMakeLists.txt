# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
SET(MODULE_UNDER_TEST mesh)
PROJECT(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC
        src/MeshBuilderTest.cpp
        src/MeshIntersectorTest.cpp
        src/mesh_manipulationsTest.cpp
        src/RandomEPointGenerator.cpp
        )
# ------8<---------------------------------------------->8-----

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF (NOT(MSVC))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
include_directories(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(${base91x_INCLUDE_DIRS})
include_directories(${binary_stl_data_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
