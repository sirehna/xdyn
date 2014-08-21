# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
SET(MODULE_UNDER_TEST mesh)
PROJECT(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC
        src/MeshBuilderTest.cpp
        src/mesh_manipulationsTest.cpp)
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
include_directories(${random_data_generator_INCLUDE_DIRS})
include_directories(${test_data_generator_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(${base91x_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
