cmake_minimum_required(VERSION 2.8.8)
project(grpc)

set(SRC src/SurfaceElevationFromGRPC.cpp
        )

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.


include_directories(inc)
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
include_directories(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${environment_models_INCLUDE_DIRS})

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

# gRPC
INCLUDE(CMakeProtoBuf)
INCLUDE(CMakeGRPC)
SET(CMAKE_C_FLAGS "-fPIC")
SET(CMAKE_CXX_FLAGS "-fPIC")
if(NOT(MSVC))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif()
SET(waves_proto ${CMAKE_CURRENT_SOURCE_DIR}/../waves_grpc/waves.proto)
GET_FILENAME_COMPONENT(waves_proto_path "${waves_proto}" PATH)

SET(waves_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/waves.pb.cc")
SET(waves_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/waves.pb.h")
SET(waves_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/waves.grpc.pb.cc")
SET(waves_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/waves.grpc.pb.h")
#SET_SOURCE_FILES_PROPERTIES("${CMAKE_CURRENT_BINARY_DIR}/waves.grpc.pb.cc" PROPERTIES COMPILE_FLAGS "")
#set_property(SOURCE "${CMAKE_CURRENT_BINARY_DIR}/waves.grpc.pb.cc" PROPERTY COMPILE_FLAGS -Werror)
ADD_CUSTOM_COMMAND(
      OUTPUT "${waves_proto_srcs}" "${waves_proto_hdrs}" "${waves_grpc_srcs}" "${waves_grpc_hdrs}"
      COMMAND ${PROTOBUF_PROTOC}
      ARGS --grpc_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
           --cpp_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
           -I "${PROTOC_PREFIX}${waves_proto_path}"
           --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
           "${PROTOC_PREFIX}${waves_proto}"
      DEPENDS "${waves_proto}")


include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})


add_library (${PROJECT_NAME} OBJECT ${SRC} ${waves_proto_srcs} ${waves_grpc_srcs})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")
