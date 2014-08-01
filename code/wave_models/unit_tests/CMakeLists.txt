# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST wave_models)
project(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/JonswapSpectrumTest.cpp
              src/PiersonMoskowitzSpectrumTest.cpp
              src/BretschneiderSpectrumTest.cpp
              src/DiracDirectionalSpreadingTest.cpp
              src/Cos2sDirectionalSpreadingTest.cpp
              src/DirectionalWaveSpectrumTest.cpp
              src/DiracSpectralDensityTest.cpp
              src/WaveNumberFunctorTest.cpp
              src/AiryTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
include_directories(${random_data_generator_INCLUDE_DIRS})
include_directories(${test_macros_SOURCE_DIR})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(SYSTEM ${kissfft_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SRC})
