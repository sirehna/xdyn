# - Find pandoc
#
# Will define:
#
# PANDOC_EXECUTABLE - the pandoc binary
#
# Uses:
#
# PANDOC_ROOT - root to search for the program

IF(WIN32)
    FIND_PROGRAM(PANDOC_EXECUTABLE pandoc HINTS ${PANDOC_ROOT} "C:/Program\ Files/Pandoc"
                                                               "C:/Programmes/Pandoc")
ELSE()
    FIND_PROGRAM(PANDOC_EXECUTABLE pandoc)
ENDIF()

# only visible in advanced view
MARK_AS_ADVANCED(PANDOC_EXECUTABLE)
