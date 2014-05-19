CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(mesh)

SET(SRC src/Mesh.cpp
        src/MeshBuilder.cpp
        src/mesh_manipulations.cpp)

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${simulator_external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${stl_reader_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exception_handling_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_macros_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${numeric_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Woverloaded-virtual -Wwrite-strings -Wfatal-errors -Wno-deprecated -Wvariadic-macros")

ADD_LIBRARY(${PROJECT_NAME}_static STATIC ${SRC})
SET(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

ADD_SUBDIRECTORY(unit_tests)
