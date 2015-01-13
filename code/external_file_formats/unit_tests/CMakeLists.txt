SET(MODULE_UNDER_TEST external_file_formats)
PROJECT(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/stl_readerTest.cpp
              src/stl_writerTest.cpp
              src/low_level_hdb_parserTest.cpp
        )
INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(${base91x_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})

