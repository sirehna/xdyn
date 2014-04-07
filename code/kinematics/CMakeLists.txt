cmake_minimum_required (VERSION 2.8.8)
project (kinematics)

set(SRC src/AbstractFrame.cpp
        src/FixedFrame.cpp
        src/Frame.cpp
        src/Vector3DInFrame.cpp
        src/Velocity.cpp
        src/rotation_matrix_builders.cpp
        src/EulerAngles.cpp
        src/Kinematics.cpp
        src/Point.cpp
        )

include_directories(inc)
include_directories(${exception_handling_INCLUDE_DIRS})
include_directories(${test_macros_INCLUDE_DIRS})
include_directories(SYSTEM ${robwork_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})

add_library (${PROJECT_NAME}_static STATIC ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
