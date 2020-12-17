# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
SET(MODULE_UNDER_TEST core)
PROJECT(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/generate_body_for_tests.cpp
              src/SimulatorBuilderTest.cpp
              src/update_kinematicsTests.cpp
              src/BodyBuilderTest.cpp
              src/SurfaceElevationFromWavesTest.cpp
              src/BodyTest.cpp
              src/ControllableForceModelTest.cpp
              src/random_kinematics.cpp
              src/BlockedDOFTest.cpp
              src/EnvironmentAndFramesTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GMOCK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${binary_stl_data_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${parser_extensions_INCLUDE_DIRS}/../unit_tests/inc)

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF (NOT(MSVC))
    SET(CMAKE_CXX_FLAGS "--std=gnu++0x")
ENDIF()

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
