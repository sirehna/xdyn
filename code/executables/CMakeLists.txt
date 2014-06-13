CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(executables)

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
include_directories(${test_macros_INCLUDE_DIRS})

CONFIGURE_FILE(src/simulator.cpp "${CMAKE_CURRENT_BINARY_DIR}/simulator.cpp" @ONLY)
ADD_EXECUTABLE(sim ${CMAKE_CURRENT_BINARY_DIR}/simulator.cpp)

TARGET_LINK_LIBRARIES(sim simulatorIrtJv)
TARGET_LINK_LIBRARIES(sim ${Boost_PROGRAM_OPTIONS_LIBRARY})
TARGET_LINK_LIBRARIES(sim boost_program_options_descriptions_static)

ADD_EXECUTABLE(generate_yaml_example src/generate_yaml_examples.cpp)
ADD_EXECUTABLE(generate_stl_examples src/generate_stl_examples.cpp)

TARGET_LINK_LIBRARIES(generate_yaml_example test_data_generator_static)
TARGET_LINK_LIBRARIES(generate_stl_examples test_data_generator_static)

################################################################################
INSTALL(TARGETS sim
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})

ADD_CUSTOM_COMMAND(TARGET generate_yaml_example
                   POST_BUILD
                   COMMAND generate_yaml_example
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Generate YAML simulation input data files")

ADD_CUSTOM_COMMAND(TARGET generate_stl_examples
                   POST_BUILD
                   COMMAND generate_stl_examples
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Generate STL mesh input data files")

INSTALL(
    CODE "FILE( GLOB _Yml \"${CMAKE_CURRENT_BINARY_DIR}/*.yml\" )"
    CODE "FILE( INSTALL \${_Yml} DESTINATION ${CMAKE_INSTALL_PREFIX}/demos)"
)

INSTALL(
    CODE "FILE( GLOB _Stl \"${CMAKE_CURRENT_BINARY_DIR}/*.stl\" )"
    CODE "FILE( INSTALL \${_Stl} DESTINATION ${CMAKE_INSTALL_PREFIX}/demos)"
)