# - Find pandoc
#
# Will define:
#
# XMLTRANSFORM_EXECUTABLE - the pandoc binary
#
# Uses:
#
# XMLTRANSFORM_ROOT - root to search for the program

IF(WIN32)
    FIND_PROGRAM(XMLTRANSFORM_EXECUTABLE Transform HINTS "C:/Program\ Files/Saxonica/SaxonHE9.4N/bin"
                                                         "C:/Program\ Files/Saxonica/SaxonHE9.5N/bin")
    IF(XMLTRANSFORM_EXECUTABLE-NOTFOUND)
        MESSAGE(STATUS "Program SAXON not found")
    ENDIF()
ELSE()
    FIND_PROGRAM(XMLTRANSFORM_EXECUTABLE xsltproc)
    IF(XMLTRANSFORM_EXECUTABLE-NOTFOUND)
        MESSAGE(STATUS "Program XSLTPROC not found")
    ENDIF()
ENDIF()

# only visible in advanced view
MARK_AS_ADVANCED(XMLTRANSFORM_EXECUTABLE)
