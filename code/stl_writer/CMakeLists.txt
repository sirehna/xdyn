cmake_minimum_required(VERSION 2.8.8)
project(stl_writer)

set(SRC src/stl_writer.cpp
        )


include_directories(inc)
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${simulator_external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})

add_library (${PROJECT_NAME} OBJECT ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
