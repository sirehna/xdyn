cmake_minimum_required(VERSION 2.8.8)
project(executables)

<<<<<<< HEAD
include_directories(${test_data_generator_INCLUDE_DIRS})

ADD_EXECUTABLE(generate_yaml_example src/generate_yaml_examples.cpp)

TARGET_LINK_LIBRARIES(generate_yaml_example test_data_generator_static)
=======
set(SRC src/.cpp
        )


include_directories(inc)
include_directories(${exception_handling_INCLUDE_DIRS})
include_directories(${test_macros_INCLUDE_DIRS})

add_library (${PROJECT_NAME}_static STATIC ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

add_subdirectory(unit_tests)
>>>>>>> Stubbed executables module
