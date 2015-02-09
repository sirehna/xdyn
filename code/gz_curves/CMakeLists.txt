cmake_minimum_required(VERSION 2.8.8)
project(gz_curves)

set(SRC src/GZTypes.cpp
        )


include_directories(inc)
include_directories(${ssc_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
