cmake_minimum_required(VERSION 2.8.8)
project(executables)

include_directories(${test_data_generator_INCLUDE_DIRS})
<<<<<<< HEAD
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(${boost_program_options_descriptions_INCLUDE_DIRS})

ADD_EXECUTABLE(sim src/simulator.cpp)
TARGET_LINK_LIBRARIES(sim simulatorIrtJv ${Boost_PROGRAM_OPTIONS_LIBRARY}
                                          boost_program_options_descriptions_static)

ADD_EXECUTABLE(generate_yaml_example src/generate_yaml_examples.cpp)
=======

ADD_EXECUTABLE(generate_yaml_example src/generate_yaml_examples.cpp)

>>>>>>> 8fd3d32ea7c44deff973a64b640b9b5378dabe87
TARGET_LINK_LIBRARIES(generate_yaml_example test_data_generator_static)