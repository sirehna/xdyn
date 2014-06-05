CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(executables)

INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${boost_program_options_descriptions_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exception_handling_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${test_data_generator_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${text_file_reader_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${data_source_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ds_integration_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${simulator_INCLUDE_DIRS})

CONFIGURE_FILE(src/simulator.cpp "${CMAKE_CURRENT_BINARY_DIR}/simulator.cpp" @ONLY)
ADD_EXECUTABLE(sim ${CMAKE_CURRENT_BINARY_DIR}/simulator.cpp)

TARGET_LINK_LIBRARIES(sim simulatorIrtJv)
TARGET_LINK_LIBRARIES(sim ${Boost_PROGRAM_OPTIONS_LIBRARY})
TARGET_LINK_LIBRARIES(sim boost_program_options_descriptions_static)

ADD_EXECUTABLE(generate_yaml_example src/generate_yaml_examples.cpp)

TARGET_LINK_LIBRARIES(generate_yaml_example test_data_generator_static)

INSTALL(TARGETS sim
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
