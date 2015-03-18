# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST gz_curves)
project(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/ResultantForceComputerTest.cpp
              src/gz_newton_raphsonTest.cpp
              src/GZCurveTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
include_directories(${ssc_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(${binary_stl_data_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_file_formats_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SRC})
