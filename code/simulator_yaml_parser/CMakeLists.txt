cmake_minimum_required(VERSION 2.8.8)
project(simulator_yaml_parser)

set(SRC src/SimulatorYamlParser.cpp
        src/force_parsers.cpp
        src/parse_unit_value.cpp
        src/environment_parsers.cpp
        src/check_input_yaml.cpp
        src/external_data_structures_parsers.cpp
        )

include_directories(inc)
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
include_directories(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
include_directories(${simulator_external_data_structures_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(${wave_models_INCLUDE_DIRS})
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
