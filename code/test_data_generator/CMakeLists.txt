CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(test_data_generator)

SET(SRC src/hdb_data.cpp
        src/stl_data.cpp
        src/yaml_data.cpp
        src/TriMeshTestData.cpp
        src/hdb_data_bug_3230.cpp
        src/yml_data_bug_3230.cpp
        src/bug_3235.cpp
        src/bug_3207.cpp
        src/bug_3238.cpp
        src/bug_3241.cpp
        src/issue_20.cpp
        src/bug_3187.cpp
        src/bug_3185.cpp
    )

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF (NOT(MSVC))
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
SET(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")
