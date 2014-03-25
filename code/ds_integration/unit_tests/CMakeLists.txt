# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST ds_integration)
project (${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/DsSystemTests.cpp)
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(${random_data_generator_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})
include_directories(${data_source_INCLUDE_DIRS})

add_library (${PROJECT_NAME} OBJECT ${SRC})
