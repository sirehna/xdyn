cmake_minimum_required (VERSION 2.8.8)
project (test_data_generator)

set(SRC src/STL_data.cpp
        src/yaml_data.cpp
        src/TriMeshTestData.cpp
        )

include_directories(inc)
include_directories(${simulator_external_data_structures_INCLUDE_DIRS})
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})

add_library (${PROJECT_NAME} OBJECT ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

