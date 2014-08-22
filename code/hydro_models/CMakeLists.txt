CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(hydro_models)

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF (NOT(MSVC))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

SET(SRC 
        src/SurfaceElevationInterface.cpp
        src/hydrostatic.cpp
        src/SurfaceElevationFromWaves.cpp
        )

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${exception_handling_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${kinematics_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${simulator_external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${numeric_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_macros_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${wave_models_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME}_static STATIC ${SRC})
SET(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

ADD_SUBDIRECTORY(unit_tests)
