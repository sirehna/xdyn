cmake_minimum_required(VERSION 2.8.8)
project(hdb_interpolators)

set(SRC src/HDBData.cpp
        src/HDBBuilder.cpp
        )

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF (NOT(MSVC))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

include_directories(inc)
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_file_formats_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})

set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
