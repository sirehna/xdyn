CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(executables)

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF(NOT(MSVC))
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${boost_program_options_descriptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${yaml_parser_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${external_file_formats_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${base91x_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${environment_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${binary_stl_data_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${force_models_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${parser_extensions_INCLUDE_DIRS})
include_directories(${observers_and_api_INCLUDE_DIRS})
include_directories(${hdb_interpolators_INCLUDE_DIRS})
include_directories(${gz_curves_INCLUDE_DIRS})

CONFIGURE_FILE(src/utilities_for_InputData.cpp "${CMAKE_CURRENT_BINARY_DIR}/utilities_for_InputData.cpp" @ONLY)
CONFIGURE_FILE(src/convert_stl_files_to_code.cpp "${CMAKE_CURRENT_BINARY_DIR}/convert_stl_files_to_code.cpp" @ONLY)
ADD_EXECUTABLE(sim ${CMAKE_CURRENT_BINARY_DIR}/utilities_for_InputData.cpp
                   src/InputData.cpp
                   src/simulator.cpp)

TARGET_LINK_LIBRARIES(sim simulatorIrtJv)
TARGET_LINK_LIBRARIES(sim ${Boost_PROGRAM_OPTIONS_LIBRARY})
TARGET_LINK_LIBRARIES(sim boost_program_options_descriptions_static)

ADD_EXECUTABLE(test_hs src/test_hs.cpp $<TARGET_OBJECTS:test_data_generator>
        $<TARGET_OBJECTS:core>
        $<TARGET_OBJECTS:parser_extensions>
        $<TARGET_OBJECTS:force_models>
        $<TARGET_OBJECTS:environment_models>
        $<TARGET_OBJECTS:external_file_formats>
        $<TARGET_OBJECTS:mesh>
        $<TARGET_OBJECTS:yaml_parser>
        $<TARGET_OBJECTS:external_data_structures>)
TARGET_LINK_LIBRARIES(test_hs simulatorIrtJv binary_stl_data_static base91x_static
)

TARGET_LINK_LIBRARIES(test_hs yaml-cpp
        gfortran)

TARGET_LINK_LIBRARIES(test_hs numeric_shared
kinematics_shared
data_source_shared
yaml_parser_shared
text_file_reader_shared
decode_unit_shared
csv_file_reader_shared)

ADD_EXECUTABLE(yml2test src/yml2test.cpp)

ADD_EXECUTABLE(quat2eul src/convert_quaternion_to_euler.cpp 
        $<TARGET_OBJECTS:yaml_parser>
        $<TARGET_OBJECTS:external_data_structures>)
TARGET_LINK_LIBRARIES(quat2eul yaml_parser_shared
                               decode_unit_shared
                               exception_handling_shared
                               kinematics_shared
                               )


ADD_EXECUTABLE(convert_stl_files_to_code ${CMAKE_CURRENT_BINARY_DIR}/convert_stl_files_to_code.cpp)
TARGET_LINK_LIBRARIES(convert_stl_files_to_code
                      simulatorIrtJv
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      boost_program_options_descriptions_static
                      base91x_static)

ADD_EXECUTABLE(gz src/gz.cpp
                  ${CMAKE_CURRENT_BINARY_DIR}/utilities_for_InputData.cpp
                  src/InputData.cpp)

TARGET_LINK_LIBRARIES(gz
                      simulatorIrtJv
                      ${Boost_PROGRAM_OPTIONS_LIBRARY}
                      boost_program_options_descriptions_static
                      base91x_static)

ADD_EXECUTABLE(generate_yaml_example
               src/generate_yaml_examples.cpp src/file_writer.cpp
               $<TARGET_OBJECTS:test_data_generator>)

ADD_EXECUTABLE(generate_stl_examples
               src/generate_stl_examples.cpp src/file_writer.cpp
               $<TARGET_OBJECTS:test_data_generator>)
TARGET_LINK_LIBRARIES(generate_stl_examples
                      binary_stl_data_static
                      base91x_static
                      simulatorIrtJv)

################################################################################
INSTALL(TARGETS sim
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
INSTALL(TARGETS convert_stl_files_to_code
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/demos")

IF(WIN32)
    ADD_CUSTOM_COMMAND(TARGET generate_yaml_example
                       OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/demos/cube_in_waves.yml
                       POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:simulatorIrtJv> .
                       COMMAND generate_yaml_example demos
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                       DEPENDS simulatorIrtJv
                       COMMENT "Generate YAML simulation input data files")
    ADD_CUSTOM_COMMAND(TARGET generate_stl_examples
                       OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/demos/cube.stl
                       POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:simulatorIrtJv> .
                       COMMAND generate_stl_examples demos
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                       DEPENDS simulatorIrtJv
                       COMMENT "Generate STL mesh input data files")
ELSE()
    ADD_CUSTOM_COMMAND(TARGET generate_yaml_example
                       POST_BUILD
                       COMMAND generate_yaml_example demos
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                       DEPENDS simulatorIrtJv
                       COMMENT "Generate YAML simulation input data files")
    ADD_CUSTOM_COMMAND(TARGET generate_stl_examples
                       POST_BUILD
                       COMMAND generate_stl_examples demos
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                       DEPENDS simulatorIrtJv
                       COMMENT "Generate STL mesh input data files")
ENDIF()

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../postprocessing/plot.py
              ${CMAKE_CURRENT_SOURCE_DIR}/../../postprocessing/animate.py
              ${CMAKE_CURRENT_SOURCE_DIR}/../../postprocessing/waveYamlToCsv.py
        DESTINATION demos)

INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/demos
        DESTINATION ".")
