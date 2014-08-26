CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(executables)

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF (NOT(MSVC))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${boost_program_options_descriptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exception_handling_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${text_file_reader_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${data_source_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ds_integration_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${simulator_external_data_structures_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${simulator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_macros_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${kinematics_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${solver_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${csv_file_reader_INCLUDE_DIRS})
include_directories(${yaml_parser_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${simulator_yaml_parser_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${stl_reader_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${mesh_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${base91x_INCLUDE_DIRS})

CONFIGURE_FILE(src/utilities_for_InputData.cpp "${CMAKE_CURRENT_BINARY_DIR}/utilities_for_InputData.cpp" @ONLY)
CONFIGURE_FILE(src/postprocessing.cpp "${CMAKE_CURRENT_BINARY_DIR}/postprocessing.cpp" @ONLY)
CONFIGURE_FILE(src/convert_stl_files_to_code.cpp "${CMAKE_CURRENT_BINARY_DIR}/convert_stl_files_to_code.cpp" @ONLY)
ADD_EXECUTABLE(sim ${CMAKE_CURRENT_BINARY_DIR}/utilities_for_InputData.cpp
                   src/InputData.cpp
                   src/simulator.cpp)

ADD_EXECUTABLE(postprocessing ${CMAKE_CURRENT_BINARY_DIR}/postprocessing.cpp)
TARGET_LINK_LIBRARIES(postprocessing simulatorIrtJv)

TARGET_LINK_LIBRARIES(sim simulatorIrtJv)
TARGET_LINK_LIBRARIES(sim ${Boost_PROGRAM_OPTIONS_LIBRARY})
TARGET_LINK_LIBRARIES(sim boost_program_options_descriptions_static)

ADD_EXECUTABLE(convert_stl_files_to_code ${CMAKE_CURRENT_BINARY_DIR}/convert_stl_files_to_code.cpp)
TARGET_LINK_LIBRARIES(convert_stl_files_to_code simulatorIrtJv)
TARGET_LINK_LIBRARIES(convert_stl_files_to_code ${Boost_PROGRAM_OPTIONS_LIBRARY})
TARGET_LINK_LIBRARIES(convert_stl_files_to_code boost_program_options_descriptions_static base91x_static)


ADD_EXECUTABLE(generate_yaml_example src/generate_yaml_examples.cpp)
ADD_EXECUTABLE(generate_stl_examples src/generate_stl_examples.cpp)

TARGET_LINK_LIBRARIES(generate_yaml_example test_data_generator_static)
TARGET_LINK_LIBRARIES(generate_stl_examples test_data_generator_static)

################################################################################
INSTALL(TARGETS sim
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
INSTALL(TARGETS postprocessing
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
INSTALL(TARGETS convert_stl_files_to_code
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/demos")
ADD_CUSTOM_COMMAND(TARGET generate_yaml_example
                   POST_BUILD
                   COMMAND generate_yaml_example demos
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Generate YAML simulation input data files")

ADD_CUSTOM_COMMAND(TARGET generate_stl_examples
                   POST_BUILD
                   COMMAND generate_stl_examples demos
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Generate STL mesh input data files")

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../postprocessing/plot.py
        DESTINATION demos)

INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/demos
        DESTINATION ".")
