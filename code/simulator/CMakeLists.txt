cmake_minimum_required(VERSION 2.8.8)
project(simulator)

set(SRC src/simulator_api.cpp
        src/update_kinematics.cpp
        src/Sim.cpp
        src/HydrostaticForceModel.cpp
        src/SimulatorBuilder.cpp
        src/SimObserver.cpp
        src/ForceBuilder.cpp
        src/GravityForceBuilder.cpp
        src/GravityForceModel.cpp
        src/HydrostaticForceBuilder.cpp
        src/SimCsvObserver.cpp
        src/SimNoObserver.cpp
        src/OutputTransformer.cpp
        src/Res.cpp
        src/SurfaceElevationBuilder.cpp
        )

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
IF (NOT(MSVC))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

include_directories(inc)
include_directories(${exception_handling_INCLUDE_DIRS})
include_directories(${simulator_external_data_structures_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})
include_directories(${kinematics_INCLUDE_DIRS})
include_directories(${simulator_yaml_parser_INCLUDE_DIRS})
include_directories(${mesh_INCLUDE_DIRS})
include_directories(${text_file_reader_INCLUDE_DIRS})
include_directories(${stl_reader_INCLUDE_DIRS})
include_directories(${hydro_models_INCLUDE_DIRS})
include_directories(${test_macros_INCLUDE_DIRS})
include_directories(${yaml_parser_INCLUDE_DIRS})
include_directories(${ds_integration_INCLUDE_DIRS})
include_directories(${solver_INCLUDE_DIRS})
include_directories(${wave_models_INCLUDE_DIRS})
include_directories(${model_wrappers_INCLUDE_DIRS})
include_directories(${parser_extensions_INCLUDE_DIRS})

add_library(${PROJECT_NAME}_static STATIC ${SRC})

set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
