# - Find XML transformation program
#
# Will define:
#
# XMLTRANSFORM_FOUND      - Boolean used to indicate whether the program was found
# XMLTRANSFORM_EXECUTABLE - Full path to a XML transformation program
#
# Uses:
#
# XMLTRANSFORM_ROOT - root to search for the program

MESSAGE(STATUS "Looking for a XML transformation program")
SET(XMLTRANSFORM_FOUND FALSE)
IF(WIN32)
    FIND_PROGRAM(XMLTRANSFORM_EXECUTABLE Transform HINTS "C:/Program\ Files/Saxonica/SaxonHE9.4N/bin"
                                                         "C:/Program\ Files/Saxonica/SaxonHE9.5N/bin")
    IF(${XMLTRANSFORM_EXECUTABLE} STREQUAL "XMLTRANSFORM_EXECUTABLE-NOTFOUND")
        MESSAGE(STATUS "Program SAXON not found")
    ELSE()
        SET(XMLTRANSFORM_FOUND TRUE)
    ENDIF()
ELSE()
    FIND_PROGRAM(XMLTRANSFORM_EXECUTABLE NAMES xsltproc)
    IF(${XMLTRANSFORM_EXECUTABLE} STREQUAL "XMLTRANSFORM_EXECUTABLE-NOTFOUND")
        MESSAGE(STATUS "Program XSLTPROC not found")
    ELSE()
	SET(XMLTRANSFORM_FOUND TRUE)
    ENDIF()
ENDIF()

# only visible in advanced view
MARK_AS_ADVANCED(XMLTRANSFORM_FOUND)
MARK_AS_ADVANCED(XMLTRANSFORM_EXECUTABLE)
