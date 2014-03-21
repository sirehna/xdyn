# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST simulator)
project (${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/ODEintTests.cpp
              src/constant_step_stepper_architecture_tests.cpp
              src/adaptive_step_stepper_architecture_tests.cpp)
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(SYSTEM inc)
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(SYSTEM ${GMOCK_INCLUDE_DIRS})
include_directories(${${MODULE_UNDER_TEST}_SOURCE_DIR}/inc)
include_directories(${random_data_generator_INCLUDE_DIRS})
include_directories(${test_macros_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(${test_utilities_INCLUDE_DIRS})

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
 #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")

# Linking libraries
add_library(${PROJECT_NAME} OBJECT ${SRC})
