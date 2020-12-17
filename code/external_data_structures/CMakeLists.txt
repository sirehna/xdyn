CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(external_data_structures)

INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})

SET(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

SET(SRC src/YamlAngle.cpp
        src/YamlBody.cpp
        src/YamlCommands.cpp
        src/YamlCoordinates.cpp
        src/YamlDynamics.cpp
        src/YamlDynamics6x6Matrix.cpp
        src/YamlEnvironmentalConstants.cpp
        src/YamlModel.cpp
        src/YamlOutput.cpp
        src/YamlPoint.cpp
        src/YamlPosition.cpp
        src/YamlRadiationDamping.cpp
        src/YamlRotation.cpp
        src/YamlSimulatorInput.cpp
        src/YamlSpeed.cpp
        src/YamlWaveModelInput.cpp
        src/YamlDiffraction.cpp
        src/YamlState.cpp
        src/YamlSimServerInputs.cpp
        src/YamlGRPC.cpp
        src/YamlWaveOutput.cpp
        )

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
if(NOT(MSVC))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif()

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})

