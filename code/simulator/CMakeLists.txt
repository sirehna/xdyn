cmake_minimum_required(VERSION 2.8.8)
project(simulator)

set(SRC src/DataSourceBuilder.cpp
        src/KinematicsModule.cpp
        src/GravityModule.cpp
        src/HydrostaticModule.cpp
        src/WaveModule.cpp
        src/SimulatorModuleInterface.cpp
        src/StateDerivativesModule.cpp
        src/coriolis_and_centripetal.cpp
        )


include_directories(inc)
include_directories(${exception_handling_INCLUDE_DIRS})
include_directories(${simulator_external_data_structures_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(${data_source_INCLUDE_DIRS})
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})
include_directories(${kinematics_INCLUDE_DIRS})
include_directories(${simulator_yaml_parser_INCLUDE_DIRS})
include_directories(${mesh_INCLUDE_DIRS})
include_directories(${text_file_reader_INCLUDE_DIRS})
include_directories(${stl_reader_INCLUDE_DIRS})
include_directories(${hydro_models_INCLUDE_DIRS})
include_directories(${numeric_INCLUDE_DIRS})

add_library (${PROJECT_NAME}_static STATIC ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
