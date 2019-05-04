CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(hos)

FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(SYSTEM ${PROTOBUF_INCLUDE_DIR})

IF(Protobuf_PROTOC_EXECUTABLE)
    MESSAGE(STATUS "Protobuf_PROTOC_EXECUTABLE ${Protobuf_PROTOC_EXECUTABLE}")
    MESSAGE(STATUS "Create protobuf::protoc")
    ADD_EXECUTABLE(protobuf::protoc IMPORTED)
    IF(EXISTS "${Protobuf_PROTOC_EXECUTABLE}")
        MESSAGE(STATUS "Set target properties")
        SET_TARGET_PROPERTIES(protobuf::protoc PROPERTIES
                                IMPORTED_LOCATION "${Protobuf_PROTOC_EXECUTABLE}")
    ENDIF()
ENDIF()

PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER hos.proto)

SET(SRC src/HOS.cpp)

IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wextra -Wall -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -fno-common -Wformat=2 -Winit-self -Wpacked -Wp,-D_FORTIFY_SOURCE=2 -Wpointer-arith -Wmissing-declarations -Wmissing-format-attribute -Wsign-compare -Wstrict-aliasing=2 -Wundef -Wconversion")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -Woverloaded-virtual -Weffc++ -Wwrite-strings -Wno-deprecated -Wvariadic-macros")
ENDIF()

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF(NOT(MSVC))
    SET(CMAKE_CXX_FLAGS "-Werror -pedantic -pedantic-errors -Wextra -Wall -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -fno-common -Wformat=2 -Winit-self -Wpacked -Wp,-D_FORTIFY_SOURCE=2 -Wpointer-arith -Wmissing-format-attribute -Wsign-compare -Wundef -Woverloaded-virtual -Wwrite-strings -Wfatal-errors -Wno-deprecated -Wvariadic-macros -std=gnu++0x")
    IF(CMAKE_SIZEOF_VOID_P EQUAL 8) # If on a 64 bit machine
        IF(UNIX AND NOT WIN32) # If on Linux
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-declarations")
            IF(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
                SET(REQUIRED_GCC_VERSION_FOR_FLAG_STRICT_ALIASING 8.0.0)
                IF(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL ${REQUIRED_GCC_VERSION_FOR_FLAG_STRICT_ALIASING})
                    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-aliasing=2")
                ENDIF()
            ENDIF()
        ENDIF()
    ENDIF()
ENDIF()

ADD_DEFINITIONS(-DPROTOBUF_INLINE_NOT_IN_HEADERS=0)

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/${PROJECT_NAME})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${environment_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${PROTOBUF_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${cppzmq_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${yaml_parser_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${ZeroMQ_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})


ADD_LIBRARY(${PROJECT_NAME} OBJECT ${PROTO_SRC} ${SRC})
SET(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

ADD_SUBDIRECTORY(unit_tests)
