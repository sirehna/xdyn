CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(executables)

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't
# declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's
# case-insensitive).
# Use -std=gnu++0x instead.
IF(NOT(MSVC))
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${base91x_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${eigen_hdf5_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${HDF5_INCLUDE_DIR})
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

CONFIGURE_FILE(
        src/utilities_for_InputData.cpp
        "${CMAKE_CURRENT_BINARY_DIR}/utilities_for_InputData.cpp" @ONLY)

CONFIGURE_FILE(
        src/convert_stl_files_to_code.cpp
        "${CMAKE_CURRENT_BINARY_DIR}/convert_stl_files_to_code.cpp" @ONLY)

ADD_EXECUTABLE(xdyn
        ${CMAKE_CURRENT_BINARY_DIR}/utilities_for_InputData.cpp
        src/utilities_for_simulator.cpp
        src/InputData.cpp
        src/InputDataForME.cpp
        src/simulator_run.cpp
        src/simulator.cpp
        )

TARGET_LINK_LIBRARIES(xdyn
        x-dyn
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        boost_program_options_descriptions_static
        ${SSC_LIB}
        )

ADD_EXECUTABLE(test_orbital_velocities_and_dynamic_pressures
src/test_orbital_velocities_and_dynamic_pressures.cpp)

TARGET_LINK_LIBRARIES(test_orbital_velocities_and_dynamic_pressures
        x-dyn
        ${SSC_LIB}
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
        $<TARGET_OBJECTS:external_data_structures>)

TARGET_LINK_LIBRARIES(test_hs
        x-dyn
        binary_stl_data_static
        ${SSC_LIB}
        )

TARGET_LINK_LIBRARIES(test_hs
        yaml-cpp
        gfortran
        )

ADD_EXECUTABLE(yml2test src/yml2test.cpp)

ADD_EXECUTABLE(quat2eul src/convert_quaternion_to_euler.cpp)

TARGET_LINK_LIBRARIES(quat2eul
        exception_handling_shared
        kinematics_shared
        )

ADD_EXECUTABLE(convert_stl_files_to_code
        ${CMAKE_CURRENT_BINARY_DIR}/convert_stl_files_to_code.cpp
        )

TARGET_LINK_LIBRARIES(convert_stl_files_to_code
        x-dyn
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        boost_program_options_descriptions_static
        ${SSC_LIB}
        )

ADD_EXECUTABLE(gz
        src/gz.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/utilities_for_InputData.cpp
        src/InputData.cpp
        src/InputDataForME.cpp
        src/simulator_run.cpp
        src/utilities_for_simulator.cpp
        )

TARGET_LINK_LIBRARIES(gz
        x-dyn
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        boost_program_options_descriptions_static
        ${SSC_LIB}
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
        ${SSC_LIB}
        )

ADD_EXECUTABLE(xdyn-server
        src/utilities_for_InputDataSimServer.cpp
        src/utilities_for_simulator.cpp
        src/InputData.cpp
        src/simulator_server.cpp

        )

TARGET_LINK_LIBRARIES(xdyn-server
        x-dyn
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        boost_program_options_descriptions_static
        ${SSC_LIB}
        )

ADD_EXECUTABLE(xdyn-for-me
        ${CMAKE_CURRENT_BINARY_DIR}/utilities_for_InputData.cpp
        src/utilities_for_simulator.cpp
        src/InputDataForME.cpp
        src/InputData.cpp
        src/xdyn_for_me.cpp

        )

TARGET_LINK_LIBRARIES(xdyn-for-me
        x-dyn
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        boost_program_options_descriptions_static
        ${SSC_LIB}
        )

################################################################################
INSTALL(TARGETS xdyn
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
INSTALL(TARGETS xdyn-server
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
