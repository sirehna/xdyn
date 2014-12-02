# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST hdb_interpolators)
project(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/HDBParserTest.cpp
              src/HistoryTest.cpp
              src/DampingMatrixInterpolatorTest.cpp
              src/DiffractionInterpolatorTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
include_directories(${ssc_INCLUDE_DIRS})
include_directories(${test_data_generator_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SRC})
