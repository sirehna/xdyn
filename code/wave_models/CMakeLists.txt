cmake_minimum_required(VERSION 2.8.8)
project(wave_models)

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
        src/ListOfWaveModels.cpp
        )


include_directories(inc)
include_directories(${exception_handling_INCLUDE_DIRS})
include_directories(${test_macros_INCLUDE_DIRS})

add_library (${PROJECT_NAME}_static STATIC ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
