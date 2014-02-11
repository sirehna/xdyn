# Add a target to generate API documentation with Doxygen
FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/DoxygenLayout.in ${CMAKE_CURRENT_BINARY_DIR}/DoxygenLayout.xml @ONLY)

    ADD_CUSTOM_TARGET(test_results
        COMMAND ./${TEST_EXE} --gtest_also_run_disabled_tests --gtest_output=xml:test_output.xml
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running all tests" VERBATIM
        DEPENDS ${TEST_EXE}
    )

    FIND_PROGRAM(SAXON_EXE Transform HINTS "c:/Program\ Files/Saxonica/SaxonHE9.4N/bin"
                                           "c:/Program\ Files/Saxonica/SaxonHE9.5N/bin")
    IF(SAXON_EXE-NOTFOUND)
        MESSAGE(STATUS "Program SAXON not found")
    ELSE()
        MESSAGE(STATUS "Program SAXON found")
    ENDIF()

    ADD_CUSTOM_TARGET(functionalities
        ${SAXON_EXE} -s:test_output.xml -xsl:${CMAKE_CURRENT_SOURCE_DIR}/get_specifications.xml -o:list_of_functionalities.html
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating list of functionalities from GTest's output XML" VERBATIM
        DEPENDS test_results
    )
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/list_of_functionalities.html
            DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../documentation/html)

    ADD_CUSTOM_TARGET(sdk_doc
        ${DOXYGEN_EXECUTABLE} Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        DEPENDS functionalities
    )

    FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/../images_for_documentation/*.svg")
    FOREACH(f ${files})
        FILE(COPY ${f} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../documentation/html)
    ENDFOREACH()

    FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/../images_for_documentation/*.png")
    FOREACH(f ${files})
        FILE(COPY ${f} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../documentation/latex)
    ENDFOREACH()

    FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.js")
    FOREACH(f ${files})
        FILE(COPY ${f} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../documentation/html)
    ENDFOREACH()

    FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.css")
    FOREACH(f ${files})
        FILE(COPY ${f} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../documentation/html)
    ENDFOREACH()

    INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../documentation/html
            DESTINATION doc)
    IF(WIN32)
        FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/doc.bat "\"html/index.html\"\n")
        INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/doc.bat
                DESTINATION doc)
    ENDIF()
ELSE()
    MESSAGE("Doxygen not found.")
ENDIF(DOXYGEN_FOUND)

ADD_CUSTOM_TARGET(
    sloccount
    sloccount `find . -maxdepth 1 -type d ! \\\( -name \".\" -o -name \"tools_ThirdParty\" -o -name \"doc\" -o -name \"data\" -o -name \"*bin*\" -o -name \".svn\" -o -name \".settings\" \\\)`> "${PROJECT_BINARY_DIR}/sloccount.txt"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Evaluates the amount of work of the project"
)

ADD_CUSTOM_TARGET(
    md5sum
    COMMAND find . -type f \\\( -name \"*.c\" -o -name \"*.h\" \\\) -print0 | xargs -0 md5sum > "${PROJECT_BINARY_DIR}/md5sum.txt"
    COMMAND find . -type f \\\( -name \"*.cpp\" -o -name \"*.hpp\" \\\) -print0 | xargs -0 md5sum >> "${PROJECT_BINARY_DIR}/md5sum.txt"
    COMMAND find . -type f \\\( -name \"*.cxx\" -o -name \"*.hxx\" \\\) -print0 | xargs -0 md5sum >> "${PROJECT_BINARY_DIR}/md5sum.txt"
    COMMAND find . -type f -name \"*.sh\" -print0 | xargs -0 md5sum >> "${PROJECT_BINARY_DIR}/md5sum.txt"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Create a file named md5sum.txt that contains the md5 hash of all source files"
)

ADD_CUSTOM_TARGET(
    archive
    COMMAND export dateSTR=`date +'%Y_%m_%d__%H_%M'` &&
            export pkg0=`echo $$PWD` &&
            export pkg=`echo \$\${PWD\#\#*/}` &&
            export tarName=`echo \$\${pkg}__\$\${dateSTR}` &&
            export dirToSave=`find . -maxdepth 1 ! \\\( -name \".\" -o -name \$\${pkg}__*.tar.gz -o -name \$\${pkg}__*.tgz -o -name \"Results*\" -o -name \"data\" -o -name \"bin\" -o -regex \"bin*\" -o -name \"build\" \\\) | cut -f2 -d"/"` &&
            tar -czf \$\${tarName}.tgz \$\${dirToSave}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Create an archive of the project"
)
