cmake_minimum_required(VERSION 2.8.8)
project(observers_and_api)

include(CMakeListsGenerateDemoMatLab.txt)
include(CMakeListsGenerateDemoPython.txt)

set(SRC src/simulator_api.cpp
        src/ListOfObservers.cpp
        src/Hdf5Observer.cpp
        src/Hdf5WaveObserver.cpp
        src/Hdf5WaveObserverBuilder.cpp
        src/Hdf5WaveSpectrumObserver.cpp
        src/SimObserver.cpp
        src/CsvObserver.cpp
        src/DictObserver.cpp
        src/TsvObserver.cpp
        src/JsonObserver.cpp
        src/WebSocketObserver.cpp
        ${CMAKE_BINARY_DIR}/demoMatLab.cpp
        ${CMAKE_BINARY_DIR}/demoPython.cpp
        src/MapObserver.cpp
        src/SimStepper.cpp
        src/ConfBuilder.cpp
        src/HistoryParser.cpp
        src/XdynForCS.cpp
        src/XdynForME.cpp
        src/SimServerInputs.cpp
        )

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
if(NOT(MSVC))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif()

include_directories(inc)
include_directories(SYSTEM ${HDF5_INCLUDE_DIR})
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})
include_directories(SYSTEM ${eigen_hdf5_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${base91x_INCLUDE_DIRS})
include_directories(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
include_directories(${exceptions_INCLUDE_DIRS})
include_directories(${ssc_INCLUDE_DIRS})
include_directories(${CMAKE_BINARY_DIR})
include_directories(${interface_hdf5_INCLUDE_DIRS})
include_directories(${external_data_structures_INCLUDE_DIRS})
include_directories(${core_INCLUDE_DIRS})
include_directories(${hdb_interpolators_INCLUDE_DIRS})
include_directories(${force_models_INCLUDE_DIRS})
include_directories(${mesh_INCLUDE_DIRS})
include_directories(${yaml_parser_INCLUDE_DIRS})
include_directories(${environment_models_INCLUDE_DIRS})
include_directories(${external_file_formats_INCLUDE_DIRS})
include_directories(${parser_extensions_INCLUDE_DIRS})
include_directories(${grpc_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
