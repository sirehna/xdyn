cmake_minimum_required(VERSION 2.8.8)
project(model_wrappers)

set(SRC src/Body.cpp
        )


include_directories(inc)
include_directories(${exception_handling_INCLUDE_DIRS})
include_directories(${test_macros_INCLUDE_DIRS})
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})
include_directories(${kinematics_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${simulator_external_data_structures_INCLUDE_DIRS})

add_library (${PROJECT_NAME}_static STATIC ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

#add_subdirectory(unit_tests)
