CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(parser_extensions)

SET(SRC src/builders.cpp
        src/listeners.cpp
        src/InterpolationModule.cpp
        )

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF (NOT(MSVC))
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${yaml_parser_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${environment_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${hdb_interpolators_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${grpc_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
SET(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

ADD_SUBDIRECTORY(unit_tests)
