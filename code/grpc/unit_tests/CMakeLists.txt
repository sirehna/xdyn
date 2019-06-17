# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
SET(MODULE_UNDER_TEST grpc)
PROJECT(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/GRPCForceModelTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GMOCK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
include_directories(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS}) # For MeshIntersector
include_directories(${hdb_interpolators_INCLUDE_DIRS}) # For History
#INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
#INCLUDE_DIRECTORIES(${external_file_formats_INCLUDE_DIRS})
#INCLUDE_DIRECTORIES(${simulator_INCLUDE_DIRS}/../unit_tests/inc)
#include_directories(${external_file_formats_INCLUDE_DIRS})
#include_directories(${hdb_interpolators_INCLUDE_DIRS}/../unit_tests/inc)
#include_directories(${core_INCLUDE_DIRS}/../unit_tests/inc)

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
