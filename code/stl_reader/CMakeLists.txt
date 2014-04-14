cmake_minimum_required (VERSION 2.8.8)
project (stl_reader)

<<<<<<< HEAD
set(SRC src/StlReader.cpp)

include_directories(inc)
include_directories(${simulator_external_data_structures_INCLUDE_DIRS})
include_directories(${exception_handling_INCLUDE_DIRS})
include_directories(${test_macros_INCLUDE_DIRS})

add_library(${PROJECT_NAME}_static STATIC ${SRC})
=======
set(SRC src/.cpp
        )


include_directories(inc)
include_directories(${exception_handling_INCLUDE_DIRS})
include_directories(${test_macros_INCLUDE_DIRS})

add_library (${PROJECT_NAME}_static STATIC ${SRC})
>>>>>>> Add the basic CMakeLists.txt files
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
