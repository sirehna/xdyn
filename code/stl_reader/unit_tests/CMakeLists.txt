<<<<<<< HEAD
SET(MODULE_UNDER_TEST stl_reader)
PROJECT(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC
        src/StlReaderTest.cpp)
INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${simulator_external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${random_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_macros_SOURCE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
=======
# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST stl_reader)
project (${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC
        src/StlReaderTest.cpp)
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
include_directories(${random_data_generator_INCLUDE_DIRS})
include_directories(${test_macros_SOURCE_DIR})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SRC})
>>>>>>> 76267ed94b4d397077d5e77b61863f3dde24d7fd
