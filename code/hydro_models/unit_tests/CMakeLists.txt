# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST hydro_models)
project (${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/DefaultWaveModelTest.cpp
              src/hydrostaticTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
include_directories(${random_data_generator_INCLUDE_DIRS})
include_directories(${test_data_generator_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(${kinematics_INCLUDE_DIRS})
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})
include_directories(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_macros_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${stl_reader_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

add_library (${PROJECT_NAME} OBJECT ${SRC})
