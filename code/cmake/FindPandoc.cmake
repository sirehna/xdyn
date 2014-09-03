# - Find pandoc
#
# Will define:
#
# PANDOC - the pandoc binary
#
# Uses:
#
# PANDOC_ROOT - root to search for the program

IF(WIN32)
    FIND_PROGRAM(PANDOC NAMES pandoc HINTS ${PANDOC_ROOT} "C:/Program\ Files/Pandoc"
                                                            "C:/Programmes/Pandoc")
ELSE()
    FIND_PROGRAM(PANDOC NAMES pandoc)
ENDIF()

IF(PANDOC)
    EXECUTE_PROCESS(COMMAND ${PANDOC} "--version" OUTPUT_VARIABLE PANDOC_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
ENDIF()

# only visible in advanced view
MARK_AS_ADVANCED(PANDOC)

