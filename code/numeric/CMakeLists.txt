cmake_minimum_required(VERSION 2.8.8)
project(numeric)

include_directories(inc)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
