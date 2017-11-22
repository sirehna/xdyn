cmake_minimum_required(VERSION 2.8.8)
project(hos)

INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER hos.proto)
#ADD_LIBRARY(proto ${PROTO_HEADER} ${PROTO_SRC})

set(SRC src/HOS.cpp)

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
if(NOT(MSVC))
    set(CMAKE_CXX_FLAGS "-Werror -pedantic -pedantic-errors -Wextra -Wall -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -fno-common -Wformat=2 -Winit-self -Wpacked -Wp,-D_FORTIFY_SOURCE=2 -Wpointer-arith -Wmissing-declarations -Wmissing-format-attribute -Wsign-compare -Wstrict-aliasing=2 -Wundef -Woverloaded-virtual -Wwrite-strings -Wfatal-errors -Wno-deprecated -Wvariadic-macros -std=gnu++0x")
    IF(CMAKE_SIZEOF_VOID_P EQUAL 8) # If on a 64 bit machine
            IF(UNIX AND NOT WIN32) # If on Linux
                SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
            ENDIF()
    ENDIF()
endif()

add_definitions(-DPROTOBUF_INLINE_NOT_IN_HEADERS=0)

include_directories(inc)
include_directories(${CMAKE_BINARY_DIR}/${PROJECT_NAME})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
include_directories(${external_data_structures_INCLUDE_DIRS})
include_directories(${core_INCLUDE_DIRS})
include_directories(${environment_models_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIRS})

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})


add_library (${PROJECT_NAME} OBJECT ${PROTO_SRC} ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
