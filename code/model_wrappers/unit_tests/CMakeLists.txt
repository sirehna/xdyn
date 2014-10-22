# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
SET(MODULE_UNDER_TEST model_wrappers)
PROJECT(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/generate_body_for_tests.cpp
              src/BodyBuilderTest.cpp
              src/QuadraticDampingForceModelTest.cpp
              src/DefaultSurfaceElevationTest.cpp
              src/GravityForceModelTest.cpp
              src/HydrostaticForceModelTest.cpp
              src/SurfaceElevationFromWavesTest.cpp
              src/FroudeKrylovForceModelTest.cpp
              src/LinearDampingForceModelTest.cpp
              src/WageningenControlledForceModelTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${simulator_INCLUDE_DIRS}/../unit_tests/inc)

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
