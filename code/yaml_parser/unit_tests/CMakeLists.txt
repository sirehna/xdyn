# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST yaml_parser)
project(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/SimulatorYamlParserTest.cpp
              src/environment_parsersTest.cpp
              src/parse_commandsTest.cpp
              src/parse_outputTest.cpp
              src/parse_addressTest.cpp
              )
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${ssc_INCLUDE_DIRS})
include_directories(${exceptions_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GMOCK_INCLUDE_DIRS})
include_directories(${external_data_structures_INCLUDE_DIRS})
include_directories(${test_data_generator_INCLUDE_DIRS})

add_library(${PROJECT_NAME} OBJECT ${SRC})
