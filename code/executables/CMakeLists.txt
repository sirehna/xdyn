cmake_minimum_required(VERSION 2.8.8)
project(executables)

include_directories(${test_data_generator_INCLUDE_DIRS})

ADD_EXECUTABLE(generate_yaml_example src/generate_yaml_examples.cpp)

TARGET_LINK_LIBRARIES(generate_yaml_example test_data_generator_static)
