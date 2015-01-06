# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST observers_and_api)
project(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/SimCsvObserverTest.cpp
              src/SimHdf5ObserverTest.cpp
              src/OutputTransformerTest.cpp
              src/SimTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
include_directories(${ssc_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(${core_INCLUDE_DIRS})
include_directories(${models_INCLUDE_DIRS})
include_directories(${mesh_INCLUDE_DIRS})
include_directories(${wave_models_INCLUDE_DIRS})
include_directories(${test_data_generator_INCLUDE_DIRS})
include_directories(${binary_stl_data_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SRC})
