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
GET_GIT_HEAD_REVISION(GIT_REFSPEC GIT_SHA1)

GIT_DESCRIBE(GIT_VERSION --tags --dirty=-dirty)
IF(${GIT_VERSION} STREQUAL "GIT_UNTAGGED_REVISION")
    MESSAGE(STATUS "GIT : Untagged working revision")
    SET(GIT_VERSION_MAJOR 0)
    SET(GIT_VERSION_MINOR 0)
    SET(GIT_VERSION_PATCH 0)
    SET(GIT_VERSION_SHORT ${GIT_SHA1})
    GIT_IS_DIRTY(GIT_VERSION_DIRTY)
ELSE()
    #parse the version information into pieces.
    MESSAGE(STATUS "GIT_VERSION :  ${GIT_VERSION}")
    STRING(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" GIT_VERSION_MAJOR "${GIT_VERSION}")
    STRING(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" GIT_VERSION_MINOR "${GIT_VERSION}")
    STRING(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" GIT_VERSION_PATCH "${GIT_VERSION}")
    STRING(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+-(.*)" "\\1" GIT_VERSION_SHA1 "${GIT_VERSION}")
    STRING(REGEX MATCH   "^v[0-9]+\\.[0-9]+.*(-dirty)" "\\1" GIT_VERSION_DIRTY "${GIT_VERSION}")
    SET(GIT_VERSION_SHORT "${GIT_VERSION_MAJOR}.${GIT_VERSION_MINOR}.${GIT_VERSION_PATCH}")
    MESSAGE(STATUS "GIT_VERSION_DIRTY ${GIT_VERSION_DIRTY}")
    MESSAGE(STATUS "GIT_VERSION_SHA1 ${GIT_VERSION_SHA1}")
    IF(GIT_VERSION_SHA1)
        SET(GIT_VERSION_SHORT "${GIT_VERSION_SHORT}-${GIT_VERSION_SHA1}")
        IF(NOT ${GIT_VERSION} STREQUAL ${GIT_VERSION_SHA1})
            SET(GIT_VERSION_PATCH "${GIT_VERSION_PATCH}_${GIT_VERSION_SHA1}")
        ENDIF()
    ENDIF()
ENDIF()
IF(${GIT_VERSION_DIRTY})
    SET(GIT_VERSION_SHORT "${GIT_VERSION_SHORT}_dirty")
ENDIF()
SET(${PROJECT_NAME}_MAJOR_VERSION ${GIT_VERSION_MAJOR})
SET(${PROJECT_NAME}_MINOR_VERSION ${GIT_VERSION_MINOR})
SET(${PROJECT_NAME}_PATCH_VERSION ${GIT_VERSION_PATCH})



CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt"
               "${CMAKE_CURRENT_BINARY_DIR}/Copyright.txt" @ONLY)

MESSAGE(STATUS "VERSION_MAJOR : ${${PROJECT_NAME}_MAJOR_VERSION}")
MESSAGE(STATUS "VERSION_MINOR : ${${PROJECT_NAME}_MINOR_VERSION}")
MESSAGE(STATUS "VERSION_PATCH : ${${PROJECT_NAME}_PATCH_VERSION}")
SET(${PROJECT_NAME}_VERSION
    ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_VERSION})
SET(${PROJECT_NAME}_VERSION_STR
    ${${PROJECT_NAME}_MAJOR_VERSION}-${${PROJECT_NAME}_MINOR_VERSION}-${${PROJECT_NAME}_PATCH_VERSION})


#############################################################################
INCLUDE(CMakeGMock)

INCLUDE(CMakeOutputDirectories)

IF((DEFINED SSC_ROOT) AND (NOT ("(${SSC_ROOT}" STREQUAL "")))
    MESSAGE(STATUS "Using manually specified SSC_ROOT : ${SSC_ROOT}")
    FIND_PACKAGE(ssc REQUIRED COMPONENTS random_data_generator numeric NO_CMAKE_ENVIRONMENT_PATH HINTS "${SSC_ROOT}")
ELSE()
    FIND_PACKAGE(ssc REQUIRED COMPONENTS random_data_generator numeric)
ENDIF()
IF(ssc_FOUND)
    MESSAGE(STATUS "SSC : ssc_INCLUDE_DIRS = ${ssc_INCLUDE_DIRS}")
    MESSAGE(STATUS "SSC : ssc_VERSION = ${ssc_VERSION}")
ENDIF()
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
SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED OFF)
#SET(Boost_USE_STATIC_RUNTIME    OFF)
FIND_PACKAGE(Boost 1.53 COMPONENTS program_options REQUIRED)
IF(Boost_FOUND)
    MESSAGE(STATUS "Boost_INCLUDE_DIRS  : ${Boost_INCLUDE_DIRS}")
    MESSAGE(STATUS "Boost_LIBRARIES     : ${Boost_LIBRARIES}")
ENDIF()

SET(YAML_SKIP_INSTALL ON)
ADD_SUBDIRECTORY(yaml-cpp)
SET(YAML_CPP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp/src
                          ${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp/include
                          ${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp/include/yaml-cpp)

ADD_SUBDIRECTORY(kiss_fft130)

SET(eigen_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/eigen)
ADD_DEFINITIONS(-DEIGEN_DONT_VECTORIZE) # Cf. http://eigen.tuxfamily.org/dox-devel/group__TopicUnalignedArrayAssert.html
ADD_DEFINITIONS(-DEIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT)
#ADD_DEFINITIONS(-DEIGEN_NO_DEBUG)

#############################################################################
# Compilation part
INCLUDE(CMakeCompilationOptions)
ADD_SUBDIRECTORY(base91x)
ADD_SUBDIRECTORY(simulator_external_data_structures)
ADD_SUBDIRECTORY(test_data_generator)
ADD_SUBDIRECTORY(external_file_formats)
ADD_SUBDIRECTORY(binary_stl_data)
ADD_SUBDIRECTORY(mesh)
ADD_SUBDIRECTORY(wave_models)
ADD_SUBDIRECTORY(simulator_yaml_parser)
ADD_SUBDIRECTORY(hydro_models)
ADD_SUBDIRECTORY(boost_program_options_descriptions)
ADD_SUBDIRECTORY(hdb_interpolators)
ADD_SUBDIRECTORY(model_wrappers)
ADD_SUBDIRECTORY(simulator)
ADD_SUBDIRECTORY(parser_extensions)
ADD_SUBDIRECTORY(executables)

ADD_LIBRARY(${PROJECT_NAME} SHARED
        $<TARGET_OBJECTS:simulator>
        $<TARGET_OBJECTS:parser_extensions>
        $<TARGET_OBJECTS:model_wrappers>
        $<TARGET_OBJECTS:wave_models>
        $<TARGET_OBJECTS:external_file_formats>
        $<TARGET_OBJECTS:hydro_models>
        $<TARGET_OBJECTS:mesh>
        $<TARGET_OBJECTS:simulator_yaml_parser>
        $<TARGET_OBJECTS:simulator_external_data_structures>
        $<TARGET_OBJECTS:hdb_interpolators>
        )

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        yaml-cpp
        gfortran
)

INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}
        LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})

MACRO(add_imported_lib libname)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${libname})
    get_target_property(MYLIB_LOCATION ${libname} LOCATION)
    install(FILES ${MYLIB_LOCATION} DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
ENDMACRO()
add_imported_lib(numeric_shared)
add_imported_lib(kinematics_shared)
add_imported_lib(data_source_shared)
add_imported_lib(yaml_parser_shared)
add_imported_lib(text_file_reader_shared)
add_imported_lib(decode_unit_shared)
add_imported_lib(csv_file_reader_shared)
add_imported_lib(exception_handling_shared)
add_imported_lib(data_source_shared)
add_imported_lib(interpolation_shared)

#############################################################################
# Testing part
ENABLE_TESTING()
IF(BUILD_TESTING)
    SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
    MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)

SET(TEST_EXE run_all_tests)

ADD_EXECUTABLE(${TEST_EXE}
        $<TARGET_OBJECTS:hydro_models_tests>
        $<TARGET_OBJECTS:mesh_tests>
        $<TARGET_OBJECTS:simulator_yaml_parser_tests>
        $<TARGET_OBJECTS:external_file_formats_tests>
        $<TARGET_OBJECTS:simulator_tests>
        $<TARGET_OBJECTS:wave_models_tests>
        $<TARGET_OBJECTS:kissfft_static>
        $<TARGET_OBJECTS:model_wrappers_tests>
        $<TARGET_OBJECTS:parser_extensions_tests>
        $<TARGET_OBJECTS:test_data_generator>
        $<TARGET_OBJECTS:hdb_interpolators_tests>
        )

SET(LINK_FLAGS ${LINK_FLAGS} "-Wl,-whole-archive")

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

