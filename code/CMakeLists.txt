CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(simulatorIrtJv)

INCLUDE(CTest)

SET(CMAKE_VERBOSE_MAKEFILE OFF)
#############################################################################
# User configuration
IF(NOT DEFINED CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
   SET(ARCH64_BITS TRUE)
ELSE()
   SET(ARCH64_BITS FALSE)
ENDIF()

IF(DEFINED INSTALL_PREFIX)
    SET(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})
ELSE()
    SET(CMAKE_INSTALL_PREFIX ../install${CMAKE_BUILD_TYPE})
ENDIF()
MESSAGE(STATUS "CMAKE_INSTALL_PREFIX :${CMAKE_INSTALL_PREFIX}")

#############################################################################
# Compilation configuration
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#############################################################################
# GIT SHA
INCLUDE(GetGitRevisionDescription)
GET_GIT_HEAD_REVISION(GIT_REFSPEC GIT_SHA1)
MESSAGE(STATUS "GIT_SHA1 : ${GIT_SHA1}")
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt"
               "${CMAKE_CURRENT_BINARY_DIR}/Copyright.txt" @ONLY)
#############################################################################
INCLUDE(CMakeGMock)

INCLUDE(CMakeOutputDirectories)

#############################################################################
# Third party libraries
# Boost
# -> Require at least version 1.53 of boost to use Odeint
#SET(Boost_DEBUG 1)
IF (WIN32)
    IF(NOT DEFINED BOOST_ROOT)
        SET(BOOST_ROOT "C:\\Program Files\\boost")
        IF(NOT "$ENV{BOOST_ROOT}" STREQUAL "")
            MESSAGE(STATUS "Use of environnement variable for BOOST")
            SET(BOOST_ROOT $ENV{BOOST_ROOT})
        ENDIF()
    ENDIF()
ENDIF()
MESSAGE(STATUS "BOOST_ROOT : ${BOOST_ROOT}")
SET(Boost_USE_STATIC_LIBS        ON)
IF (NOT(WIN32))
   SET(Boost_USE_MULTITHREADED     OFF)
ENDIF()
#SET(Boost_USE_STATIC_RUNTIME    OFF)
FIND_PACKAGE(Boost 1.53 COMPONENTS program_options REQUIRED)
IF(Boost_FOUND)
    MESSAGE(STATUS "Boost_INCLUDE_DIRS  : ${Boost_INCLUDE_DIRS}")
    MESSAGE(STATUS "Boost_LIBRARIES     : ${Boost_LIBRARIES}")
ENDIF()

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
ADD_SUBDIRECTORY(macros)
ADD_SUBDIRECTORY(random_data_generator)
ADD_SUBDIRECTORY(numeric)
ADD_SUBDIRECTORY(simulator_external_data_structures)
ADD_SUBDIRECTORY(test_data_generator)
ADD_SUBDIRECTORY(exception_handling)
ADD_SUBDIRECTORY(text_file_reader)
ADD_SUBDIRECTORY(decode_unit)
ADD_SUBDIRECTORY(data_source)
ADD_SUBDIRECTORY(data_source_serializers)
ADD_SUBDIRECTORY(interpolation)
ADD_SUBDIRECTORY(yaml_parser)
ADD_SUBDIRECTORY(simulator_yaml_parser)
ADD_SUBDIRECTORY(solver)
ADD_SUBDIRECTORY(ds_integration)
ADD_SUBDIRECTORY(kinematics)
ADD_SUBDIRECTORY(mesh)
ADD_SUBDIRECTORY(hydro_models)
ADD_SUBDIRECTORY(stl_reader)
ADD_SUBDIRECTORY(boost_program_options_descriptions)
ADD_SUBDIRECTORY(simulator)
ADD_SUBDIRECTORY(executables)

INCLUDE(CMakeLapackDouble)

ADD_LIBRARY(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:exception_handling_object>
                                   $<TARGET_OBJECTS:interpolation_object>
                                   $<TARGET_OBJECTS:text_file_reader_static>)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        data_source_serializers_static
        ds_integration_static
        simulator_static
        mesh_static
        stl_reader_static
        hydro_models_static
        kinematics_static
        simulator_yaml_parser_static
        yaml_parser_static
        simulator_external_data_structures_static
        yaml-cpp
        decode_unit_static
        data_source_static
        lapack                      # static
        gfortran                    # static
        )

INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}
        LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})

#############################################################################
# Testing part
ENABLE_TESTING()
IF(BUILD_TESTING)
    SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
    MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)

SET(TEST_EXE run_all_tests)

ADD_EXECUTABLE(${TEST_EXE}
        $<TARGET_OBJECTS:data_source_tests>
        $<TARGET_OBJECTS:solver_tests>
        $<TARGET_OBJECTS:data_source_serializers_tests>
        $<TARGET_OBJECTS:interpolation_tests>
        $<TARGET_OBJECTS:random_data_generator_tests>
        $<TARGET_OBJECTS:text_file_reader_tests>
        $<TARGET_OBJECTS:interpolation_object>
        $<TARGET_OBJECTS:hydro_models_tests>
        $<TARGET_OBJECTS:ds_integration_tests>
        $<TARGET_OBJECTS:kinematics_tests>
        $<TARGET_OBJECTS:mesh_tests>
        $<TARGET_OBJECTS:simulator_yaml_parser_tests>
        $<TARGET_OBJECTS:stl_reader_tests>
        $<TARGET_OBJECTS:simulator_tests>
        $<TARGET_OBJECTS:numeric_tests>
        )

SET(LINK_FLAGS ${LINK_FLAGS} "-Wl,-whole-archive")

TARGET_LINK_LIBRARIES(${TEST_EXE}
        gtest gtest_main                 # static
        gmock gmock_main                 # static
        random_data_generator_static     # static
        test_data_generator_static       # static
        ${PROJECT_NAME}
        )

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

