CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(simulatorIrtJv)

#############################################################################
# User configuration
IF(NOT DEFINED CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

SET(CMAKE_INSTALL_PREFIX ../install${CMAKE_BUILD_TYPE})

#############################################################################
# Compilation configuration
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(CMakeGMock)

INCLUDE(CMakeOutputDirectories)

#############################################################################
# Third party libraries
SET(BOOST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
FIND_PACKAGE(Boost REQUIRED)
ADD_SUBDIRECTORY(yaml-cpp)
SET(YAML_CPP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp/src
                          ${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp/include
                          ${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp/include/yaml-cpp)
SET(eigen_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/eigen)
ADD_DEFINITIONS(-DEIGEN_DONT_VECTORIZE) # Cf. http://eigen.tuxfamily.org/dox-devel/group__TopicUnalignedArrayAssert.html
ADD_DEFINITIONS(-DEIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT)
#############################################################################
# Compilation part
INCLUDE(CMakeCompilationOptions)
ADD_SUBDIRECTORY(simulator_test_data_generator)
ADD_SUBDIRECTORY(macros)
ADD_SUBDIRECTORY(random_data_generator)
ADD_SUBDIRECTORY(exception_handling)
ADD_SUBDIRECTORY(text_file_reader)
ADD_SUBDIRECTORY(decode_unit)
ADD_SUBDIRECTORY(data_source)
ADD_SUBDIRECTORY(data_source_serializers)
ADD_SUBDIRECTORY(interpolation)
ADD_SUBDIRECTORY(simulation)
ADD_SUBDIRECTORY(yaml_parser)
ADD_SUBDIRECTORY(external_data_structures)
ADD_SUBDIRECTORY(test_utilities)
ADD_SUBDIRECTORY(parser)
ADD_SUBDIRECTORY(dynamic_model)
ADD_SUBDIRECTORY(dynamho_interface)
ADD_SUBDIRECTORY(physical_models)
ADD_SUBDIRECTORY(simulator)
ADD_SUBDIRECTORY(ds_integration)
ADD_SUBDIRECTORY(integration_tests)
ADD_SUBDIRECTORY(DyNaMHo)

INCLUDE(CMakeLapackDouble)

ADD_LIBRARY(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:exception_handling_object>
                                   $<TARGET_OBJECTS:interpolation_object>
                                   $<TARGET_OBJECTS:text_file_reader_static>)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        data_source_static          # static
        simulation_static           # static
        lapack                      # static
        gfortran                    # static
        yaml_parser_static
        decode_unit_static
        data_source_serializers_static
        ds_integration_static
        yaml-cpp
        )

INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}
        LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})

#############################################################################
# Testing part
ENABLE_TESTING()
SET(TEST_EXE run_all_tests)

ADD_EXECUTABLE(${TEST_EXE}
        $<TARGET_OBJECTS:data_source_tests>
        $<TARGET_OBJECTS:simulator_tests>
        $<TARGET_OBJECTS:data_source_serializers_tests>
        $<TARGET_OBJECTS:interpolation_tests>
        $<TARGET_OBJECTS:physical_models_object>
        $<TARGET_OBJECTS:random_data_generator_tests>
        $<TARGET_OBJECTS:text_file_reader_tests>
        $<TARGET_OBJECTS:interpolation_object>
        $<TARGET_OBJECTS:external_data_structures_object>
        $<TARGET_OBJECTS:dynamho_object>
        $<TARGET_OBJECTS:dynamic_model_object>
        $<TARGET_OBJECTS:integration_tests>
        $<TARGET_OBJECTS:dynamho_yaml_data_generator>
        $<TARGET_OBJECTS:parser_object>
        $<TARGET_OBJECTS:dynamho_interface_object>
        $<TARGET_OBJECTS:ds_integration_tests>
        )

SET(LINK_FLAGS ${LINK_FLAGS} "-Wl,-whole-archive")

TARGET_LINK_LIBRARIES(${TEST_EXE}
        gtest gtest_main                 # static
        gmock gmock_main                 # static
        random_data_generator_static     # static
        simulator_test_data_generator_static
        ${PROJECT_NAME}
        )

INSTALL(TARGETS ${TEST_EXE}
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})

ADD_TEST(NAME ${PROJECT_NAME}_TEST_001
         WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
         COMMAND ${TEST_EXE} --gtest_also_run_disabled_tests --gtest_output=xml:test_output.xml)

IF(CMAKE_BUILD_TYPE_UPPER MATCHES COVERAGE)
    ENABLE_COVERAGE_REPORT(TARGETS ${TEST_EXE}
                           FILTER boost eigen gmock lapack
                                  /c/mingw/lib/gcc /usr/include)
ENDIF()

#############################################################################
INCLUDE(CMakeCustomTargets)
INCLUDE(CMakePack)

SET_TARGET_PROPERTIES(parse PROPERTIES EXCLUDE_FROM_ALL TRUE)
SET_TARGET_PROPERTIES(simulation_tests PROPERTIES EXCLUDE_FROM_ALL TRUE)
SET_TARGET_PROPERTIES(make_test_data PROPERTIES EXCLUDE_FROM_ALL TRUE)
SET_TARGET_PROPERTIES(test_utilities_object PROPERTIES EXCLUDE_FROM_ALL TRUE)