CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(x-dyn)
SET(DESCRIPTION "X-DYN : Simulateur 6ddl de navires")
SET(CMAKE_VERBOSE_MAKEFILE OFF)
#############################################################################
OPTION(BUILD_DOCUMENTATION "Boolean used to build xdyn documentation" ON)
OPTION(THIRD_PARTY_DIRECTORY "Where should CMake look for (former submodules) eigen, eigen3-hdf5, etc... ?" ${CMAKE_CURRENT_SOURCE_DIR})
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
# External libraries
INCLUDE(CMakeOutputDirectories)
INCLUDE(CMakeGMock)
INCLUDE(CMakeSSC)
INCLUDE(CMakeYamlCpp)
INCLUDE(CMakeEigen)
INCLUDE(CMakeHdf5)
INCLUDE(CMakeProtoBuf)
INCLUDE(CMakeGRPC)

INCLUDE(CMakeBoost)
SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED OFF)
#SET(Boost_USE_STATIC_RUNTIME    OFF)
FIND_PACKAGE(Boost 1.53 COMPONENTS program_options filesystem system regex REQUIRED) #random
IF(Boost_FOUND)
    MESSAGE(STATUS "Boost_INCLUDE_DIRS  : ${Boost_INCLUDE_DIRS}")
    MESSAGE(STATUS "Boost_LIBRARIES     : ${Boost_LIBRARIES}")
ENDIF()

#############################################################################
# Compilation part
INCLUDE(CMakeCompilationOptions)
ADD_SUBDIRECTORY(base91x)
ADD_SUBDIRECTORY(exceptions)
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

IF (WIN32)
    SET(STATICORSHARED STATIC)
ELSE()
    SET(STATICORSHARED SHARED)
ENDIF()

ADD_LIBRARY(${PROJECT_NAME} ${STATICORSHARED}
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

IF (WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${ssc_STATIC_LIB})
ELSE()
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC ${ssc_STATIC_LIB})
ENDIF()

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE yaml-cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE gfortran)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${HDF5_CXX_STATIC_LIBRARY} ${HDF5_C_STATIC_LIBRARY})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${Boost_FILESYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${Boost_SYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${Boost_REGEX_LIBRARY})

IF(WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC wsock32 ws2_32)
ELSE()
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC pthread)
ENDIF()

IF(UNIX AND NOT(WIN32))
    SET(LIBRARY_OUTPUT_DIRECTORY "lib")
    INSTALL(TARGETS ${PROJECT_NAME}
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib)
ELSE()
    INSTALL(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}
            ARCHIVE DESTINATION ${LIBRARY_OUTPUT_DIRECTORY}
            LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
ENDIF()
#############################################################################
# Testing part

INSTALL(FILES integration_tests.py
        DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})

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
        gtest      # static
        gmock_main # static
        binary_stl_data_static
        ${Boost_FILESYSTEM_LIBRARY}
        ${PROJECT_NAME}
        ${GRPC_GRPCPP_UNSECURE}
        ${PROTOBUF_LIBPROTOBUF}
        )

MESSAGE(STATUS "CMAKE_BUILD_TYPE_UPPER : ${CMAKE_BUILD_TYPE_UPPER}")
IF(CMAKE_BUILD_TYPE_UPPER MATCHES COVERAGE)
MESSAGE(STATUS "Adding coverage")
FIND_PACKAGE(codecov)
add_coverage(${TEST_EXE})
ENDIF()

INCLUDE(CMakeTesting)
IF(BUILD_DOCUMENTATION)
    MESSAGE(STATUS "Build documentation")
    INCLUDE(CMakeDocumentation)
ENDIF()
INCLUDE(CMakeCustomTargets)
INCLUDE(CMakePack)
