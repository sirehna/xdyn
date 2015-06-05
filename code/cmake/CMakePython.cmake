FUNCTION(FIND_PYTHON_MODULE module)
    STRING(TOUPPER ${module} module_upper)
    IF(NOT PY_${module_upper})
        IF(ARGC GREATER 1 AND ARGV1 STREQUAL "REQUIRED")
            SET(${module}_FIND_REQUIRED TRUE)
        ENDIF()
        EXECUTE_PROCESS(COMMAND "${PYTHON_EXECUTABLE}" "-c"
            "import re, ${module}; print(re.compile('/__init__.py.*').sub('',${module}.__file__))"
            RESULT_VARIABLE _${module}_status
            OUTPUT_VARIABLE _${module}_location
            ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
        IF(NOT _${module}_status)
            SET(PY_${module_upper} ${_${module}_location} CACHE STRING
                "Location of Python module ${module}")
        ENDIF()
    ENDIF()
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(PY_${module} DEFAULT_MSG PY_${module_upper})
ENDFUNCTION(FIND_PYTHON_MODULE)
################################################################################

FIND_PACKAGE(PythonInterp 3.4)
IF(PYTHONINTERP_FOUND)
    FIND_PYTHON_MODULE(cx_Freeze)
    FIND_PYTHON_MODULE(tornado)
ENDIF()
MESSAGE(STATUS "PY_CX_FREEZE ${PY_CX_FREEZE}")
MESSAGE(STATUS "PY_TORNADO ${PY_TORNADO}")
IF(PY_CX_FREEZE AND PY_TORNADO)
    MESSAGE(STATUS "Creation of a server")
ENDIF()
