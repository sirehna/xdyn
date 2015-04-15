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
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt"
               "${CMAKE_CURRENT_BINARY_DIR}/Copyright.txt" @ONLY)

#############################################################################
INCLUDE(CMakeGMock)
INCLUDE(CMakeOutputDirectories)
INCLUDE(CMakeSSC)
INCLUDE(CMakeBoost)
INCLUDE(CMakeYamlCpp)

#############################################################################
# Third party libraries
# Boost
# -> Require at least version 1.53 of boost to use Odeint
SET(eigen_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/eigen)
SET(eigen_hdf5_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/eigen3-hdf5)
ADD_DEFINITIONS(-DEIGEN_DONT_VECTORIZE) # Cf. http://eigen.tuxfamily.org/dox-devel/group__TopicUnalignedArrayAssert.html
ADD_DEFINITIONS(-DEIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT)
#ADD_DEFINITIONS(-DEIGEN_NO_DEBUG)

INCLUDE(CMakeHdf5)

#############################################################################
# Compilation part
INCLUDE(CMakeCompilationOptions)
ADD_SUBDIRECTORY(base91x)
ADD_SUBDIRECTORY(external_data_structures)
ADD_SUBDIRECTORY(test_data_generator)
ADD_SUBDIRECTORY(interface_hdf5)
ADD_SUBDIRECTORY(external_file_formats)
ADD_SUBDIRECTORY(binary_stl_data)
ADD_SUBDIRECTORY(mesh)
ADD_SUBDIRECTORY(environment_models)
ADD_SUBDIRECTORY(yaml_parser)
ADD_SUBDIRECTORY(boost_program_options_descriptions)
ADD_SUBDIRECTORY(hdb_interpolators)
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(force_models)
ADD_SUBDIRECTORY(parser_extensions)
ADD_SUBDIRECTORY(observers_and_api)
ADD_SUBDIRECTORY(gz_curves)
ADD_SUBDIRECTORY(executables)

ADD_LIBRARY(${PROJECT_NAME} SHARED
        $<TARGET_OBJECTS:core>
        $<TARGET_OBJECTS:observers_and_api>
        $<TARGET_OBJECTS:parser_extensions>
        $<TARGET_OBJECTS:force_models>
        $<TARGET_OBJECTS:environment_models>
        $<TARGET_OBJECTS:external_file_formats>
        $<TARGET_OBJECTS:mesh>
        $<TARGET_OBJECTS:yaml_parser>
        $<TARGET_OBJECTS:external_data_structures>
        $<TARGET_OBJECTS:hdb_interpolators>
        $<TARGET_OBJECTS:interface_hdf5>
        $<TARGET_OBJECTS:gz_curves>
        )

MACRO(ADD_IMPORTED_LIB libname)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC ${libname})
    GET_TARGET_PROPERTY(MYLIB_LOCATION ${libname} LOCATION)
    INSTALL(FILES ${MYLIB_LOCATION} DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
ENDMACRO()
ADD_IMPORTED_LIB(numeric_shared)
ADD_IMPORTED_LIB(kinematics_shared)
ADD_IMPORTED_LIB(data_source_shared)
ADD_IMPORTED_LIB(yaml_parser_shared)
ADD_IMPORTED_LIB(text_file_reader_shared)
ADD_IMPORTED_LIB(decode_unit_shared)
ADD_IMPORTED_LIB(csv_file_reader_shared)
ADD_IMPORTED_LIB(exception_handling_shared)
ADD_IMPORTED_LIB(data_source_shared)
ADD_IMPORTED_LIB(interpolation_shared)
ADD_IMPORTED_LIB(integrate_shared)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE yaml-cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE gfortran)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE hdf5_cpp hdf5)

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
        $<TARGET_OBJECTS:mesh_tests>
        $<TARGET_OBJECTS:yaml_parser_tests>
        $<TARGET_OBJECTS:external_file_formats_tests>
        $<TARGET_OBJECTS:core_tests>
        $<TARGET_OBJECTS:environment_models_tests>
        $<TARGET_OBJECTS:force_models_tests>
        $<TARGET_OBJECTS:parser_extensions_tests>
        $<TARGET_OBJECTS:test_data_generator>
        $<TARGET_OBJECTS:hdb_interpolators_tests>
        $<TARGET_OBJECTS:interface_hdf5_tests>
        $<TARGET_OBJECTS:observers_and_api_tests>
        $<TARGET_OBJECTS:gz_curves_tests>
        )

TARGET_LINK_LIBRARIES(${TEST_EXE}
        gtest gtest_main                 # static
        gmock gmock_main                 # static
        base91x_static
        binary_stl_data_static
        ${PROJECT_NAME}
        random_data_generator_shared
        )

ADD_TEST(NAME ${PROJECT_NAME}_TEST_001
         WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
         COMMAND ${TEST_EXE} --gtest_output=xml:test_output.xml)

IF(CMAKE_BUILD_TYPE_UPPER MATCHES COVERAGE)
    ENABLE_COVERAGE_REPORT(TARGETS ${TEST_EXE}
                           FILTER boost eigen gmock lapack
                                  /c/mingw/lib/gcc /usr/include)
ENDIF()

#############################################################################
INCLUDE(CMakeCustomTargets)
INCLUDE(CMakePack)

