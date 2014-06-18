cmake_minimum_required(VERSION 2.8.8)
project(simulator)

set(SRC src/DataSourceBuilder.cpp
        src/KinematicsModule.cpp
        src/GravityModule.cpp
        src/HydrostaticModule.cpp
        src/WaveModule.cpp
        src/SimulatorModuleInterface.cpp
        src/StateDerivativesModule.cpp
        src/simulator_api.cpp
        src/update_kinematics.cpp
        src/Sim.cpp
        src/HydrostaticForceModel.cpp
        src/SimulatorBuilder.cpp
        src/Body.cpp
        src/ForceBuilder.cpp
        src/DefaultWaveBuilder.cpp
        src/GravityForceBuilder.cpp
        src/GravityForceModel.cpp
        src/BodyBuilder.cpp
        src/HydrostaticForceBuilder.cpp
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
include_directories(${test_macros_INCLUDE_DIRS})
include_directories(${yaml_parser_INCLUDE_DIRS})
include_directories(${ds_integration_INCLUDE_DIRS})

add_library(${PROJECT_NAME}_static STATIC ${SRC})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
