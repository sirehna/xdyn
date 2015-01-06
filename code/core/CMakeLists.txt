cmake_minimum_required(VERSION 2.8.8)
project(core)

set(SRC src/update_kinematics.cpp
        src/Sim.cpp
        src/SimulatorBuilder.cpp
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
include_directories(${ssc_INCLUDE_DIRS})
include_directories(${external_data_structures_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})
include_directories(${external_file_formats_INCLUDE_DIRS})
include_directories(${ds_integration_INCLUDE_DIRS})
include_directories(${yaml_parser_INCLUDE_DIRS})
include_directories(${wave_models_INCLUDE_DIRS})
include_directories(${models_INCLUDE_DIRS})
include_directories(${parser_extensions_INCLUDE_DIRS})
include_directories(${hydro_models_INCLUDE_DIRS})
include_directories(${mesh_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SRC})

set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
