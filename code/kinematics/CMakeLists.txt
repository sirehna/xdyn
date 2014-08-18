CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(kinematics)

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF (NOT(MSVC))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

SET(SRC src/Velocity.cpp
        src/rotation_matrix_builders.cpp
        src/EulerAngles.cpp
        src/Kinematics.cpp
        src/Point.cpp
        src/PointMatrix.cpp
        src/Transform.cpp
        src/KinematicTree.cpp
        src/Wrench.cpp
        src/UnsafeWrench.cpp
        src/coriolis_and_centripetal.cpp
        src/EnvironmentAndFrames.cpp
        )

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${exception_handling_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_macros_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${data_source_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME}_static STATIC ${SRC})
SET(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

ADD_SUBDIRECTORY(unit_tests)
