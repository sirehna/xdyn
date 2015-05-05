cmake_minimum_required(VERSION 2.8.8)
project(websocket)

set(SRC src/ConnectionMetadata.cpp
        src/WebSocketClient.cpp
        src/WebSocketMessage.cpp
        src/WebSocketMessageImpl.cpp
        )

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF (NOT(MSVC))
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

include_directories(inc)
include_directories(${ssc_INCLUDE_DIRS})
include_directories(SYSTEM ${websocketpp_INCLUDE_DIR})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
ADD_DEFINITIONS(-D_WEBSOCKETPP_CPP11_STL_)
ADD_DEFINITIONS(-D_WEBSOCKETPP_CPP11_THREAD_)
ADD_DEFINITIONS(-D_WEBSOCKETPP_CPP11_SYSTEM_ERROR_)

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
