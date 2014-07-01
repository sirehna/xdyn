# Add a target to generate API documentation with Doxygen
FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/DoxygenLayout.in ${CMAKE_CURRENT_BINARY_DIR}/DoxygenLayout.xml @ONLY)
    FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../doc_dev")
    FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../doc_dev/html")
    ADD_CUSTOM_TARGET(test_results
        COMMAND ./${TEST_EXE} --gtest_also_run_disabled_tests --gtest_output=xml:test_output.xml
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running all tests" VERBATIM
        DEPENDS ${TEST_EXE}
    )

    IF (WIN32)
    FIND_PACKAGE(XmlTransform)
    ADD_CUSTOM_TARGET(functionalities
        COMMAND ${XMLTRANSFORM_EXECUTABLE} -s:test_output.xml -xsl:${CMAKE_CURRENT_SOURCE_DIR}/get_specifications.xml -o:list_of_functionalities.html
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/list_of_functionalities.html ${CMAKE_CURRENT_SOURCE_DIR}/../doc_dev/html
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating list of functionalities from GTest's output XML" VERBATIM
        DEPENDS test_results
    )
    ENDIF()

    FIND_PACKAGE(Pandoc)
    IF(PANDOC_EXECUTABLE-NOTFOUND)
        MESSAGE(STATUS "Program PANDOC not found -> Developer documentation will be uncomplete")
    ELSE()
        ADD_CUSTOM_TARGET(doc_dev_guide
            ${PANDOC_EXECUTABLE} dev_guide.md -o html/dev_guide.html
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../doc_dev
            COMMENT "Generating the developper guide" VERBATIM
        )
    ENDIF()

    IF (WIN32)
        ADD_CUSTOM_TARGET(doc_dev
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            DEPENDS functionalities
            )
    ELSE()
        ADD_CUSTOM_TARGET(doc_dev
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            )
    ENDIF()
    IF(NOT PANDOC-NOTFOUND)
        ADD_DEPENDENCIES(doc_dev doc_dev_guide)
    ENDIF()

    FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/../images_for_documentation/*.svg")
    FOREACH(f ${files})
        FILE(COPY ${f} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../doc_dev/html)
    ENDFOREACH()

    FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/../images_for_documentation/*.png")
    FOREACH(f ${files})
        FILE(COPY ${f} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../doc_dev/latex)
    ENDFOREACH()

    FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.js")
    FOREACH(f ${files})
        FILE(COPY ${f} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../doc_dev/html)
    ENDFOREACH()

    FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.css")
    FOREACH(f ${files})
        FILE(COPY ${f} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../doc_dev/html)
    ENDFOREACH()

    IF(WIN32)
        FILE(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/../doc_dev/doc_developer.bat "\"html/index.html\"\n")
    ENDIF()
ELSE()
    MESSAGE("Doxygen not found.")
ENDIF(DOXYGEN_FOUND)

FIND_PACKAGE(Pandoc)
IF(PANDOC_EXECUTABLE-NOTFOUND)
    MESSAGE(STATUS "Program PANDOC not found -> No user documentation will be generated")
ELSE()
    FOREACH(f fr)
        ADD_CUSTOM_COMMAND(
            OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/user_guide_${f}.html
            COMMAND ${PANDOC_EXECUTABLE} -s user_guide_${f}.md -o user_guide_${f}.html
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user)
        LIST(APPEND DOC_USER_INSTALL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/user_guide_${f}.html)
    ENDFOREACH()
    ADD_CUSTOM_TARGET(doc_user ALL DEPENDS ${DOC_USER_INSTALL_FILES})
    INSTALL(FILES ${DOC_USER_INSTALL_FILES} DESTINATION doc)

    FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/images/*.svg")
    FOREACH(f ${files})
        INSTALL(FILES ${f} DESTINATION doc/images)
    ENDFOREACH()
ENDIF()

ADD_CUSTOM_TARGET(
    sloccount
    sloccount `find . -maxdepth 1 -type d ! \\\( -name \".\" -o -name \"tools_ThirdParty\" -o -name \"doc\" -o -name \"data\" -o -name \"*bin*\" -o -name \".svn\" -o -name \".git\" -o -name \".settings\" \\\)`> "${PROJECT_BINARY_DIR}/sloccount.txt"
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
