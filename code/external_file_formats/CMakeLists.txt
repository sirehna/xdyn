CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(external_file_formats)

SET(SRC src/HDBFile.cpp
        src/HDBParser.cpp
        src/StlReader.cpp
        src/stl_writer.cpp
        src/hdb_to_ast.cpp
        )

SET(CMAKE_CXX_FLAGS " -Woverloaded-virtual -Wwrite-strings -Wfatal-errors -Wno-deprecated -Wvariadic-macros -std=gnu++0x")

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${simulator_external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
SET(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

ADD_SUBDIRECTORY(unit_tests)
