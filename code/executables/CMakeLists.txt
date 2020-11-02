CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(executables)

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

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${base91x_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_hdf5_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${HDF5_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${PROTOBUF_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${boost_program_options_descriptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${yaml_parser_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_file_formats_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${environment_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${binary_stl_data_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${force_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${parser_extensions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${observers_and_api_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${hdb_interpolators_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${gz_curves_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${interface_hdf5_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


# For gRPC
INCLUDE_DIRECTORIES(SYSTEM ${PROTOBUF_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${GRPC_INCLUDE_DIR})

SET(cosimulation_proto ${CMAKE_CURRENT_SOURCE_DIR}/../grpc/cosimulation.proto)
SET(cosimulation_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/cosimulation.pb.cc")
SET(cosimulation_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/cosimulation.pb.h")
SET(cosimulation_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/cosimulation.grpc.pb.cc")
SET(cosimulation_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/cosimulation.grpc.pb.h")
GET_FILENAME_COMPONENT(proto_path "${cosimulation_proto}" PATH)
ADD_CUSTOM_COMMAND(
      OUTPUT "${cosimulation_proto_srcs}" "${cosimulation_proto_hdrs}" "${cosimulation_grpc_srcs}" "${cosimulation_grpc_hdrs}"
      COMMAND ${PROTOBUF_PROTOC}
      ARGS --grpc_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
           --cpp_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
           -I "${PROTOC_PREFIX}${proto_path}"
           --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
           "${PROTOC_PREFIX}${cosimulation_proto}"
      DEPENDS "${cosimulation_proto}")

SET(model_exchange_proto ${CMAKE_CURRENT_SOURCE_DIR}/../grpc/model_exchange.proto)
SET(model_exchange_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/model_exchange.pb.cc")
SET(model_exchange_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/model_exchange.pb.h")
SET(model_exchange_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/model_exchange.grpc.pb.cc")
SET(model_exchange_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/model_exchange.grpc.pb.h")
ADD_CUSTOM_COMMAND(
      OUTPUT "${model_exchange_proto_srcs}" "${model_exchange_proto_hdrs}" "${model_exchange_grpc_srcs}" "${model_exchange_grpc_hdrs}"
      COMMAND ${PROTOBUF_PROTOC}
      ARGS --grpc_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
           --cpp_out "${PROTOC_PREFIX}${CMAKE_CURRENT_BINARY_DIR}"
           -I "${PROTOC_PREFIX}${proto_path}"
           --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
           "${PROTOC_PREFIX}${model_exchange_proto}"
      DEPENDS "${model_exchange_proto}")

CONFIGURE_FILE(
        src/display_command_line_arguments.cpp
        "${CMAKE_CURRENT_BINARY_DIR}/display_command_line_arguments.cpp" @ONLY)

CONFIGURE_FILE(
        src/convert_stl_files_to_code.cpp
        "${CMAKE_CURRENT_BINARY_DIR}/convert_stl_files_to_code.cpp" @ONLY)

ADD_EXECUTABLE(xdyn
        ${CMAKE_CURRENT_BINARY_DIR}/display_command_line_arguments.cpp
        src/parse_XdynCommandLineArguments.cpp
        src/build_observers_description.cpp
        src/XdynCommandLineArguments.cpp
        src/report_xdyn_exceptions_to_user.cpp
        src/xdyn.cpp
        )

TARGET_LINK_LIBRARIES(xdyn
        x-dyn
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        boost_program_options_descriptions_static
        ${GRPC_GRPCPP_UNSECURE}
        ${PROTOBUF_LIBPROTOBUF}
        )

ADD_EXECUTABLE(test_orbital_velocities_and_dynamic_pressures
        src/test_orbital_velocities_and_dynamic_pressures.cpp
        )

TARGET_LINK_LIBRARIES(test_orbital_velocities_and_dynamic_pressures
        x-dyn
        ${GRPC_GRPCPP_UNSECURE}
        ${PROTOBUF_LIBPROTOBUF}
        )

ADD_EXECUTABLE(test_hs
        src/test_hs.cpp
        $<TARGET_OBJECTS:test_data_generator>
        $<TARGET_OBJECTS:core>
        $<TARGET_OBJECTS:parser_extensions>
        $<TARGET_OBJECTS:force_models>
        $<TARGET_OBJECTS:environment_models>
        $<TARGET_OBJECTS:external_file_formats>
        $<TARGET_OBJECTS:mesh>
        $<TARGET_OBJECTS:yaml_parser>
        $<TARGET_OBJECTS:external_data_structures>
        )

TARGET_LINK_LIBRARIES(test_hs
        x-dyn
        binary_stl_data_static
        ${GRPC_GRPCPP_UNSECURE}
        ${PROTOBUF_LIBPROTOBUF}
        )

TARGET_LINK_LIBRARIES(test_hs
        yaml-cpp
        gfortran
        )

ADD_EXECUTABLE(yml2test src/yml2test.cpp)

ADD_EXECUTABLE(quat2eul src/convert_quaternion_to_euler.cpp)

TARGET_LINK_LIBRARIES(quat2eul
        ${ssc_STATIC_LIB}
        )

ADD_EXECUTABLE(convert_stl_files_to_code
        ${CMAKE_CURRENT_BINARY_DIR}/convert_stl_files_to_code.cpp
        )

TARGET_LINK_LIBRARIES(convert_stl_files_to_code
        x-dyn
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        boost_program_options_descriptions_static
        ${GRPC_GRPCPP_UNSECURE}
        ${PROTOBUF_LIBPROTOBUF}
        )

ADD_EXECUTABLE(gz
        src/gz.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/display_command_line_arguments.cpp
        src/parse_XdynCommandLineArguments.cpp
        src/XdynCommandLineArguments.cpp
        src/report_xdyn_exceptions_to_user.cpp
        src/build_observers_description.cpp
        )

TARGET_LINK_LIBRARIES(gz
        x-dyn
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        boost_program_options_descriptions_static
        ${GRPC_GRPCPP_UNSECURE}
        ${PROTOBUF_LIBPROTOBUF}
        )

ADD_EXECUTABLE(generate_yaml_example
        src/generate_yaml_examples.cpp src/file_writer.cpp
        $<TARGET_OBJECTS:test_data_generator>
        )

ADD_EXECUTABLE(generate_stl_examples
        src/generate_stl_examples.cpp
        src/file_writer.cpp
        $<TARGET_OBJECTS:test_data_generator>
        )

TARGET_LINK_LIBRARIES(generate_stl_examples
        binary_stl_data_static
        x-dyn
        ${GRPC_GRPCPP_UNSECURE}
        ${PROTOBUF_LIBPROTOBUF}
        )

ADD_EXECUTABLE(xdyn-for-cs
        ${CMAKE_CURRENT_BINARY_DIR}/display_command_line_arguments.cpp
        src/parse_XdynForCSCommandLineArguments.cpp
        src/build_observers_description.cpp
        src/report_xdyn_exceptions_to_user.cpp
        src/XdynForCSCommandLineArguments.cpp
        src/XdynCommandLineArguments.cpp
        src/xdyn_for_cs.cpp
        src/CosimulationServiceImpl.cpp
        "${cosimulation_proto_srcs}"
        "${cosimulation_grpc_srcs}"
        )

TARGET_LINK_LIBRARIES(xdyn-for-cs
        x-dyn
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        boost_program_options_descriptions_static
        ${GRPC_GRPCPP_UNSECURE}
        ${PROTOBUF_LIBPROTOBUF}
        )

ADD_EXECUTABLE(xdyn-for-me
        ${CMAKE_CURRENT_BINARY_DIR}/display_command_line_arguments.cpp
        src/parse_XdynForMECommandLineArguments.cpp
        src/build_observers_description.cpp
        src/report_xdyn_exceptions_to_user.cpp
        src/XdynForMECommandLineArguments.cpp
        src/xdyn_for_me.cpp
        src/ModelExchangeServiceImpl.cpp
        "${model_exchange_proto_srcs}"
        "${model_exchange_grpc_srcs}"
        )

TARGET_LINK_LIBRARIES(xdyn-for-me
        x-dyn
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        boost_program_options_descriptions_static
        ${GRPC_GRPCPP_UNSECURE}
        ${PROTOBUF_LIBPROTOBUF}
        )

################################################################################
INSTALL(TARGETS xdyn
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
INSTALL(TARGETS xdyn-for-cs
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
INSTALL(TARGETS gz
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
INSTALL(TARGETS xdyn-for-me
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/demos")

IF(WIN32)
    ADD_CUSTOM_COMMAND(
            TARGET generate_yaml_example
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:x-dyn> .
            COMMAND generate_yaml_example demos
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS x-dyn
            COMMENT "Generate YAML simulation input data files")
    ADD_CUSTOM_COMMAND(
            TARGET generate_stl_examples
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:x-dyn> .
            COMMAND generate_stl_examples demos
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS x-dyn
            COMMENT "Generate STL mesh input data files")
ELSE()
    ADD_CUSTOM_COMMAND(
            TARGET generate_yaml_example
            POST_BUILD
            COMMAND generate_yaml_example demos
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS x-dyn
            COMMENT "Generate YAML simulation input data files")
    ADD_CUSTOM_COMMAND(
            TARGET generate_stl_examples
            POST_BUILD
            COMMAND generate_stl_examples demos
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS x-dyn
            COMMENT "Generate STL mesh input data files")
ENDIF()

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../postprocessing/Python/plot.py
              ${CMAKE_CURRENT_SOURCE_DIR}/../../postprocessing/Python/animate.py
              ${CMAKE_CURRENT_SOURCE_DIR}/../../postprocessing/Python/waveYamlToCsv.py
        DESTINATION demos)

INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/demos
        DESTINATION ".")
