# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST model_wrappers)
project(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/generate_body_for_tests.cpp
              src/BodyBuilderTest.cpp
              src/DefaultSurfaceElevationTest.cpp
              src/GravityForceModelTest.cpp
              src/HydrostaticForceModelTest.cpp
              src/SurfaceElevationFromWavesTest.cpp
              src/FroudeKrylovForceModelTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(${test_data_generator_INCLUDE_DIRS})
include_directories(${mesh_INCLUDE_DIRS})
include_directories(${simulator_INCLUDE_DIRS}/../unit_tests/inc)

add_library(${PROJECT_NAME} OBJECT ${SRC})
