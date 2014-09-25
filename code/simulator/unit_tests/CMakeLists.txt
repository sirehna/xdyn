# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST simulator)
project(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/SimTest.cpp
              src/SimulatorBuilderTest.cpp
              src/update_kinematicsTests.cpp
              src/SimulatorBuilderTest.cpp
              src/OutputTransformerTest.cpp
              src/SimCsvObserverTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
include_directories(${simulator_external_data_structures_INCLUDE_DIRS})
include_directories(${test_data_generator_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})
include_directories(${binary_stl_data_INCLUDE_DIRS})
include_directories(${model_wrappers_INCLUDE_DIRS}/../unit_tests/inc)
include_directories(${parser_extensions_INCLUDE_DIRS}/../unit_tests/inc)

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF (NOT(MSVC))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

add_library(${PROJECT_NAME} OBJECT ${SRC})
