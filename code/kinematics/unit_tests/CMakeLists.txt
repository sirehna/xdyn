# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST kinematics)
project (${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/KinematicsTests.cpp
              src/rotation_matrix_builders_Test.cpp
              src/TransformTest.cpp
              src/VelocityTest.cpp
              src/random_kinematics.cpp
              src/KinematicTreeTest.cpp
              src/WrenchTest.cpp
              src/transform_double_equal.cpp
              src/coriolis_and_centripetalTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
include_directories(${random_data_generator_INCLUDE_DIRS})
include_directories(${test_macros_SOURCE_DIR})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(${data_source_INCLUDE_DIRS})

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF (NOT(MSVC))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

add_library (${PROJECT_NAME} OBJECT ${SRC})
