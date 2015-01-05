CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(test_data_generator)

SET(SRC src/hdb_data.cpp
        src/STL_data.cpp
        src/yaml_data.cpp
        src/TriMeshTestData.cpp
    )

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
SET(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")
