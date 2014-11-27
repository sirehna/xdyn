CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(simulator_external_data_structures)

INCLUDE_DIRECTORIES(SYSTEM ${eigen_INCLUDE_DIRS})

SET(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

SET(SRC src/YamlAngle.cpp
        src/YamlAnglesOutput.cpp
        src/YamlBlockedDegreesOfFreedom.cpp
        src/YamlBody.cpp
        src/YamlCommands.cpp
        src/YamlCoordinates.cpp
        src/YamlDynamics.cpp
        src/YamlDynamics6x6Matrix.cpp
        src/YamlEnvironmentalConstants.cpp
        src/YamlGravity.cpp
        src/YamlForcesAndTorquesOutput.cpp
        src/YamlModel.cpp
        src/YamlOutput.cpp
        src/YamlPoint.cpp
        src/YamlPosition.cpp
        src/YamlPositionOutput.cpp
        src/YamlResistanceCurve.cpp
        src/YamlRotation.cpp
        src/YamlSimulatorInput.cpp
        src/YamlSpeed.cpp
        src/YamlWaveModelInput.cpp
        src/YamlWageningen.cpp
        )

INCLUDE_DIRECTORIES(inc)
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})

ADD_LIBRARY(${PROJECT_NAME} OBJECT ${SRC})

