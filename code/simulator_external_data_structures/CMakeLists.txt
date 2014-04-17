CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(simulator_external_data_structures)

INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})

SET(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

set(SRC src/YamlRotation.cpp
        src/YamlModel.cpp
        src/YamlCoordinates.cpp
        src/YamlAngle.cpp
        src/YamlPosition.cpp
        src/YamlPoint.cpp
        src/YamlSpeed.cpp
        src/YamlInertiaMatrix.cpp
        src/YamlDynamics.cpp
        src/YamlBody.cpp
        src/YamlBlockedDegreesOfFreedom.cpp
        src/YamlOutput.cpp
        src/YamlPositionOutput.cpp
        src/YamlAnglesOutput.cpp
        src/YamlForcesAndTorquesOutput.cpp
        src/YamlSimulatorInput.cpp
        )


include_directories(inc)
include_directories(${exception_handling_INCLUDE_DIRS})
include_directories(${test_macros_INCLUDE_DIRS})

add_library(${PROJECT_NAME}_static STATIC ${SRC})

