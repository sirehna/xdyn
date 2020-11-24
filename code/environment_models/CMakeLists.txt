cmake_minimum_required(VERSION 2.8.8)
project(environment_models)

set(SRC src/WaveSpectralDensity.cpp
        src/JonswapSpectrum.cpp
        src/PiersonMoskowitzSpectrum.cpp
        src/BretschneiderSpectrum.cpp
        src/WaveDirectionalSpreading.cpp
        src/DiracDirectionalSpreading.cpp
        src/Cos2sDirectionalSpreading.cpp
        src/DiscreteDirectionalWaveSpectrum.cpp
        src/DiracSpectralDensity.cpp
        src/WaveNumberFunctor.cpp
        src/Airy.cpp
        src/WaveModel.cpp
        src/discretize.cpp
        src/YamlSpectraInput.cpp
        src/SumOfWaveSpectralDensities.cpp
        src/SumOfWaveDirectionalSpreadings.cpp
        src/WaveDirectionalSpreading.cpp
        src/Stretching.cpp
        src/WindModel.cpp
        src/DefaultWindModel.cpp
        )

# Using C++ 2011
# The -std=c++0x option causes g++ to go into 'strict ANSI' mode so it doesn't declare non-standard functions
# (and _stricmp() is non-standard - it's just a version of strcmp() that's case-insensitive).
# Use -std=gnu++0x instead.
if(NOT(MSVC))
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif()

include_directories(inc)
INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${exceptions_INCLUDE_DIRS})
include_directories(${external_data_structures_INCLUDE_DIRS})

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${YAML_CPP_INCLUDE_DIRS})

add_library (${PROJECT_NAME} OBJECT ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
