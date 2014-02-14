# - Creates a special coverage build type and target on GCC.
#
# Defines a function ENABLE_COVERAGE_REPORT which generates the coverage target
# for selected targets. Optional arguments to this function are used to filter
# unwanted results using globbing expressions. Moreover targets with tests for
# the source code can be specified to trigger regenerating the report if the
# test has changed
#
# ENABLE_COVERAGE_REPORT(TARGETS target... [FILTER filter...] [TESTS test targets...])
#
# To generate a coverage report first build the project with
# CMAKE_BUILD_TYPE=coverage, then call make test and afterwards make coverage.
#
# The coverage report is based on gcov. Depending on the availability of lcov
# a HTML report will be generated and/or an XML report of gcovr is found.
# The generated coverage target executes all found solutions. Special targets
# exist to create e.g. only the xml report: coverage-xml.
#
# Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
#
# This program is free software; you can redistribute it
# and/or modify it under the terms of the GNU General
# Public License as published by the Free Software Foundation;
# either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#

INCLUDE(ParseArguments)

FIND_PACKAGE(Lcov  REQUIRED)
FIND_PACKAGE(Gcovr REQUIRED)

FUNCTION(ENABLE_COVERAGE_REPORT)
    # argument parsing
    PARSE_ARGUMENTS(ARG "FILTER;TARGETS;TESTS" "" ${ARGN})

    STRING(REGEX REPLACE "^([A-Z]):(.*)" "/\\1\\2" CMAKE_BINARY_DIR_UNIX ${CMAKE_BINARY_DIR})
    STRING(REGEX REPLACE "^([A-Z]):(.*)" "/\\1\\2" CMAKE_SOURCE_DIR_UNIX ${CMAKE_SOURCE_DIR})
    IF(WIN32)
        STRING(SUBSTRING ${CMAKE_SOURCE_DIR_UNIX} 1 1 SECOND_CHARACTER)
        STRING(TOLOWER ${SECOND_CHARACTER} SECOND_CHARACTER_LOWER)
        STRING(TOUPPER ${SECOND_CHARACTER} SECOND_CHARACTER_UPPER)
        STRING(REGEX REPLACE "^...(.*)" "/${SECOND_CHARACTER_LOWER}\\1" CMAKE_SOURCE_DIR_UNIX_FL " ${CMAKE_SOURCE_DIR_UNIX}")
        STRING(REGEX REPLACE "^...(.*)" "/${SECOND_CHARACTER_UPPER}\\1" CMAKE_SOURCE_DIR_UNIX_FU " ${CMAKE_SOURCE_DIR_UNIX}")
        MESSAGE(STATUS "ENABLE_COVERAGE_REPORT CMAKE_BINARY_DIR         : " ${CMAKE_BINARY_DIR})
        MESSAGE(STATUS "ENABLE_COVERAGE_REPORT CMAKE_SOURCE_DIR_UNIX_FL : " ${CMAKE_SOURCE_DIR_UNIX_FL})
        MESSAGE(STATUS "ENABLE_COVERAGE_REPORT CMAKE_SOURCE_DIR_UNIX_FU : " ${CMAKE_SOURCE_DIR_UNIX_FU})
    ELSE()
        SET(CMAKE_SOURCE_DIR_UNIX_FL ${CMAKE_SOURCE_DIR_UNIX})
        SET(CMAKE_SOURCE_DIR_UNIX_FU ${CMAKE_SOURCE_DIR_UNIX})
    ENDIF()
    SET(COVERAGE_RAW_FILE "${CMAKE_BINARY_DIR_UNIX}/coverage.raw.info")
    SET(COVERAGE_FILTERED_FILE "${CMAKE_BINARY_DIR_UNIX}/coverage.info")
    SET(COVERAGE_REPORT_DIR "${CMAKE_BINARY_DIR_UNIX}/coveragereport")
    SET(COVERAGE_XML_FILE "${CMAKE_BINARY_DIR}/coverage.xml")
    SET(COVERAGE_XML_COMMAND_FILE "${CMAKE_BINARY_DIR}/coverage-xml.cmake")

    # decide if there is any tool to create coverage data
    SET(TOOL_FOUND FALSE)
    IF(LCOV_FOUND OR GCOVR_FOUND)
        SET(TOOL_FOUND TRUE)
    ENDIF()
    IF(NOT TOOL_FOUND)
        MESSAGE(STATUS "Cannot enable coverage targets because neither lcov nor gcovr are found.")
    ENDIF()

    STRING(TOLOWER "${CMAKE_BUILD_TYPE}" COVERAGE_BUILD_TYPE)
    IF(CMAKE_COMPILER_IS_GNUCXX AND TOOL_FOUND AND "${COVERAGE_BUILD_TYPE}" MATCHES "coverage")

        MESSAGE(STATUS "Coverage support enabled for targets: ${ARG_TARGETS}")

        # create coverage build type
        SET(CMAKE_CXX_FLAGS_COVERAGE ${CMAKE_CXX_FLAGS_DEBUG} PARENT_SCOPE)
        SET(CMAKE_C_FLAGS_COVERAGE ${CMAKE_C_FLAGS_DEBUG} PARENT_SCOPE)
        SET(CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES} coverage PARENT_SCOPE)

        # instrument targets
        #SET_TARGET_PROPERTIES(${ARG_TARGETS} PROPERTIES COMPILE_FLAGS --coverage
        #                                                LINK_FLAGS --coverage)

        # html report
        IF (LCOV_FOUND)
            MESSAGE(STATUS "Enabling HTML coverage report")
            # set up coverage target
            ADD_CUSTOM_COMMAND(OUTPUT ${COVERAGE_RAW_FILE}
                               COMMAND ${LCOV_EXECUTABLE} -c -d ${CMAKE_BINARY_DIR_UNIX} -o ${COVERAGE_RAW_FILE}
                               WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                               COMMENT "Collecting coverage data in ${CMAKE_BINARY_DIR_UNIX} : Output expected ${COVERAGE_RAW_FILE}"
                               DEPENDS ${ARG_TARGETS} ${ARG_TESTS}
                               VERBATIM)

            # filter unwanted stuff
            LIST(LENGTH ARG_FILTER FILTER_LENGTH)
            IF(${FILTER_LENGTH} GREATER 0)
                SET(FILTER COMMAND ${LCOV_EXECUTABLE})
                FOREACH(F ${ARG_FILTER})
                    STRING(SUBSTRING ${F} 0 1 FIRST_CHARACTER)
                    IF(${FIRST_CHARACTER} MATCHES "/")
                        SET(FILTER ${FILTER} -r ${COVERAGE_FILTERED_FILE} ${F})
                    ELSE()
                        SET(FILTER ${FILTER} -r ${COVERAGE_FILTERED_FILE} "${CMAKE_SOURCE_DIR_UNIX_FL}/${F}/*")
                    ENDIF()
                ENDFOREACH()
                SET(FILTER ${FILTER} -o ${COVERAGE_FILTERED_FILE})
            ELSE()
                SET(FILTER "")
            ENDIF()

            IF("${FILTER}" STREQUAL "")
                ADD_CUSTOM_COMMAND(OUTPUT ${COVERAGE_FILTERED_FILE}
                                   COMMAND ${CMAKE_COMMAND} -E copy ${COVERAGE_RAW_FILE} ${COVERAGE_FILTERED_FILE}
                                   DEPENDS ${COVERAGE_RAW_FILE}
                                   COMMENT "Filtering recorded coverage data for project-relevant entries -> Just a copy of raw file"
                                   VERBATIM)
            ELSE()
                ADD_CUSTOM_COMMAND(OUTPUT ${COVERAGE_FILTERED_FILE}
                                   COMMAND ${LCOV_EXECUTABLE} -e ${COVERAGE_RAW_FILE} "${CMAKE_SOURCE_DIR_UNIX_FL}*" -o ${COVERAGE_FILTERED_FILE} ${FILTER}
                                   DEPENDS ${COVERAGE_RAW_FILE}
                                   COMMENT "Filtering recorded coverage data for project-relevant entries"
                                   VERBATIM)
            ENDIF()
            ADD_CUSTOM_COMMAND(OUTPUT ${COVERAGE_REPORT_DIR}
                               COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_REPORT_DIR}
                               COMMAND ${GENHTML_EXECUTABLE} --legend --show-details -t "${PROJECT_NAME} test coverage" -o ${COVERAGE_REPORT_DIR} ${COVERAGE_FILTERED_FILE}
                               DEPENDS ${COVERAGE_FILTERED_FILE}
                               COMMENT "Generating HTML coverage report in ${COVERAGE_REPORT_DIR}"
                               VERBATIM)

            ADD_CUSTOM_TARGET(coverage-html
                              DEPENDS ${COVERAGE_REPORT_DIR})

        ENDIF()

        # xml coverage report
        IF(GCOVR_FOUND)
            MESSAGE(STATUS "Enabling XML coverage report")
            # gcovr cannot write directly to a file so the execution needs to
            # be wrapped in a cmake file that generates the file output
            FILE(WRITE ${COVERAGE_XML_COMMAND_FILE}
                 "SET(ENV{LANG} en)\n")
            FILE(APPEND ${COVERAGE_XML_COMMAND_FILE} # GJ Remove option -r
                 "EXECUTE_PROCESS(COMMAND python \"${GCOVR_EXECUTABLE}\" -x \"${CMAKE_SOURCE_DIR}\" OUTPUT_FILE \"${COVERAGE_XML_FILE}\" WORKING_DIRECTORY \"${CMAKE_BINARY_DIR}\")\n")
            ADD_CUSTOM_COMMAND(OUTPUT ${COVERAGE_XML_FILE}
                               COMMAND ${CMAKE_COMMAND} ARGS -P ${COVERAGE_XML_COMMAND_FILE}
                               COMMENT "Generating coverage XML report"
                               VERBATIM)
            ADD_CUSTOM_TARGET(coverage-xml
                              DEPENDS ${COVERAGE_XML_FILE})
        ENDIF()

        # provide a global coverage target executing both steps if available
        SET(GLOBAL_DEPENDS "")
        IF(LCOV_FOUND)
            LIST(APPEND GLOBAL_DEPENDS ${COVERAGE_REPORT_DIR})
        ENDIF()
        IF(GCOVR_FOUND)
            LIST(APPEND GLOBAL_DEPENDS ${COVERAGE_XML_FILE})
        ENDIF()
        IF(LCOV_FOUND OR GCOVR_FOUND)
            ADD_CUSTOM_TARGET(coverage
                              DEPENDS ${GLOBAL_DEPENDS})
        ENDIF()
    ENDIF()

ENDFUNCTION()
