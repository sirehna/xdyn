cmake_minimum_required(VERSION 2.8.8)
project(grpc)

set(SRC src/SurfaceElevationFromGRPC.cpp
        )

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    UNSET(CMAKE_CXX_FLAGS)
    SET(CMAKE_CXX_FLAGS "-std=gnu++0x")
    IF(MINGW)
        # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pedantic -pedantic-errors -Wextra -Wall -Wfatal-errors ")
        # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable ")
        # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-common -Wformat=2 -Winit-self -Wpacked -Wp,-D_FORTIFY_SOURCE=2 ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpointer-arith -Wmissing-format-attribute -Wsign-compare -Wundef ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual -Wwrite-strings -Wno-deprecated -Wvariadic-macros")
    ELSE()
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pedantic -pedantic-errors -Wextra -Wall -Wunused-function")
        # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-common -Wformat=2 -Winit-self -Wpacked -Wp,-D_FORTIFY_SOURCE=2 ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpointer-arith -Wmissing-format-attribute -Wsign-compare -Wundef ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual -Wwrite-strings -Wfatal-errors -Wno-deprecated -Wvariadic-macros")
        IF(CMAKE_SIZEOF_VOID_P EQUAL 8) # If on a 64 bit machine
            IF(UNIX AND NOT WIN32) # If on Linux
                SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
                # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-declarations")
                # SET(REQUIRED_GCC_VERSION_FOR_FLAG_STRICT_ALIASING 8.0.0)
                # IF(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL ${REQUIRED_GCC_VERSION_FOR_FLAG_STRICT_ALIASING})
                #     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-aliasing=2")
                #     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-aliasing=2")
                # ENDIF()
            ENDIF()
        ENDIF()
    ENDIF()
ELSE()
    SET(CMAKE_CXX_FLAGS "")
ENDIF()

include_directories(inc)
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${environment_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${PROTOBUF_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${GRPC_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})

# gRPC
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

add_library (${PROJECT_NAME} OBJECT ${SRC} ${waves_proto_srcs} ${waves_grpc_srcs})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")
