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
INCLUDE(CMakeCompilationOptions)

#############################################################################
# Compilation part
SET(boost_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Path to Boost's include directory")
ADD_SUBDIRECTORY(macros)
ADD_SUBDIRECTORY(random_data_generator)
ADD_SUBDIRECTORY(exception_handling)
ADD_SUBDIRECTORY(data_source)
ADD_SUBDIRECTORY(interpolation)
ADD_SUBDIRECTORY(simulation)
ADD_SUBDIRECTORY(test_data_generator)
INCLUDE(CMakeLapackDouble)

ADD_LIBRARY(${PROJECT_NAME} SHARED
        $<TARGET_OBJECTS:exception_handling_object>
        $<TARGET_OBJECTS:interpolation_object>)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        data_source_static          # static
        simulation_static           # static
        test_data_generator_static
        lapack                      # static
        gfortran                    # static
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
        $<TARGET_OBJECTS:simulation_tests>
        $<TARGET_OBJECTS:interpolation_object>)

SET(LINK_FLAGS ${LINK_FLAGS} "-Wl,-whole-archive")

TARGET_LINK_LIBRARIES(${TEST_EXE}
        gtest gtest_main                 # static
        gmock gmock_main                 # static
        random_data_generator_static     # static
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

