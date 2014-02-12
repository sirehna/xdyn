# This file contains CMake macros, that can be called in CMakeLists.txt file

# This macro produces a string containing the time of the call to this macro
MACRO(MACRO_TODAY TODAY_RESULT)
    SET(${TODAY_RESULT} YYYYY mm DD -- HH MM)
    IF(UNIX OR MSYS)
        EXECUTE_PROCESS(COMMAND "date" "+%Y_%m_%d__%H_%M" OUTPUT_VARIABLE ${TODAY_RESULT} OUTPUT_STRIP_TRAILING_WHITESPACE)
        # Trick to get rid of eol characters
        STRING(REGEX REPLACE "(....)_(..)_(..)__(..)_(..).*" "\\1 \\2 \\3 -- \\4 \\5" ${TODAY_RESULT} ${${TODAY_RESULT}})
    ELSEIF(WIN32)
        EXECUTE_PROCESS(COMMAND "cmd" " /C date /T && time /T" OUTPUT_VARIABLE ${TODAY_RESULT} OUTPUT_STRIP_TRAILING_WHITESPACE)
        # MESSAGE(STATUS "TODAY_RESULT = ${${TODAY_RESULT}}")
        # Trick to get rid of eol characters
        STRING(REGEX REPLACE "(..)/(..)/(....).*(..):(..).*" "\\3 \\2 \\1 -- \\4 \\5" ${TODAY_RESULT} ${${TODAY_RESULT}})
    ELSE()
        MESSAGE(SEND_ERROR "Date not implemented")
    ENDIF()
ENDMACRO(MACRO_TODAY)

MACRO(MACRO_SVNVERSION SVNVERSION_RESULT)
    FIND_PACKAGE(Subversion)
    SET(${SVNVERSION_RESULT} "Repertoire_non_versionne_avec_SVN")
    FIND_PROGRAM(SVNVERSION_EXECUTABLE svnversion)
    IF(SVNVERSION_EXECUTABLE)
        EXECUTE_PROCESS(COMMAND "${SVNVERSION_EXECUTABLE}" "${PROJECT_SOURCE_DIR}" OUTPUT_VARIABLE ${SVNVERSION_RESULT} OUTPUT_STRIP_TRAILING_WHITESPACE)
        STRING(REPLACE ":" "_" ${SVNVERSION_RESULT} ${${SVNVERSION_RESULT}})
    ENDIF()
ENDMACRO(MACRO_SVNVERSION)


#At the end of this script snippet, the CMake variable PROCESSOR_COUNT has a
#value appropriate for passing to make's -j for parallel builds.
#
#When used in a ctest -S script, you can call...
#
#IF(PROCESSOR_COUNT)
#  SET(CTEST_BUILD_FLAGS "-j${PROCESSOR_COUNT}")
#ENDIF()
#
#...to enable parallel builds with "Unix Makefiles" and the ctest_build
#command.
MACRO(MACRO_GET_NUMBER_OF_PROCESSORS PROCESSOR_COUNT)
    # Unknown:
    SET(PROCESSOR_COUNT 0)

    # Linux:
    SET(cpuinfo_file "/proc/cpuinfo")
    IF(EXISTS "${cpuinfo_file}")
        FILE(STRINGS "${cpuinfo_file}" procs REGEX "^processor.: [0-9]+$")
        LIST(LENGTH procs PROCESSOR_COUNT)
    ENDIF()

    # Mac:
    IF(APPLE)
        FIND_PROGRAM(cmd_sys_pro "system_profiler")
        IF(cmd_sys_pro)
            EXECUTE_PROCESS(COMMAND ${cmd_sys_pro} OUTPUT_VARIABLE info)
            STRING(REGEX REPLACE "^.*Total Number Of Cores: ([0-9]+).*$" "\\1"
                   PROCESSOR_COUNT "${info}")
        ENDIF()
    ENDIF()

    # Windows:
    IF(WIN32)
        SET(PROCESSOR_COUNT "$ENV{NUMBER_OF_PROCESSORS}")
    ENDIF()
ENDMACRO(MACRO_GET_NUMBER_OF_PROCESSORS)

