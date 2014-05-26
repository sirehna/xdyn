# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST simulator)
project(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/DataSourceBuilderTest.cpp
              src/KinematicsModuleTest.cpp
              src/GravityModuleTest.cpp
              src/WaveModuleTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
include_directories(${random_data_generator_INCLUDE_DIRS})
include_directories(${simulator_yaml_parser_INCLUDE_DIRS})
include_directories(${yaml_parser_INCLUDE_DIRS})
include_directories(${simulator_external_data_structures_INCLUDE_DIRS})
include_directories(${test_data_generator_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(${kinematics_INCLUDE_DIRS})
include_directories(${kinematics_INCLUDE_DIRS}/../unit_tests/inc)
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})
include_directories(${mesh_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SRC})
