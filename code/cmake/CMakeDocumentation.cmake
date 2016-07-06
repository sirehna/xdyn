# Add a target to generate API documentation with Doxygen
FIND_PACKAGE(Doxygen)
FIND_PACKAGE(Pandoc)
FIND_PACKAGE(XmlTransform)

MESSAGE(STATUS "PANDOC : ${PANDOC}")
IF(PANDOC)
    MESSAGE(STATUS "Program PANDOC found -> Documentation will be generated")
ELSE()
    MESSAGE(STATUS "Program PANDOC NOT found -> Documentation will be uncomplete")
ENDIF()

IF(DOXYGEN_FOUND)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/DoxygenLayout.in ${CMAKE_CURRENT_BINARY_DIR}/DoxygenLayout.xml @ONLY)
    FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../doc_dev")
    FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../doc_dev/html")
    ADD_CUSTOM_TARGET(test_results
        COMMAND ./${TEST_EXE} --gtest_output=xml:test_output.xml
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running all tests" VERBATIM
        DEPENDS ${TEST_EXE}
    )

    IF(WIN32)
        IF(XMLTRANSFORM_FOUND)
            ADD_CUSTOM_TARGET(functionalities
                COMMAND ${XMLTRANSFORM_EXECUTABLE} -s:test_output.xml -xsl:${CMAKE_CURRENT_SOURCE_DIR}/get_specifications.xml -o:list_of_functionalities.html
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/list_of_functionalities.html ${CMAKE_CURRENT_SOURCE_DIR}/../doc_dev/html
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating list of functionalities from GTest's output XML" VERBATIM
                DEPENDS test_results
            )
        ENDIF()
    ENDIF()

    IF(PANDOC)
        ADD_CUSTOM_TARGET(doc_dev_guide
            ./doc_html.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../doc_dev
            COMMENT "Generating the developper guide" VERBATIM
        )
    ELSE()
        MESSAGE(STATUS "Program PANDOC not found -> Developer documentation will be uncomplete")
    ENDIF()

    IF(WIN32 AND XMLTRANSFORM_FOUND)
        MESSAGE(STATUS "Adding doc_dev target WITH dependency on target functionalities")
        ADD_CUSTOM_TARGET(doc_dev
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            DEPENDS functionalities
            )
    ELSE()
        MESSAGE(STATUS "Adding doc_dev target WITHOUT dependency on target functionalities (No XML transform was found)")
        ADD_CUSTOM_TARGET(doc_dev
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            )
    ENDIF()
    IF(PANDOC)
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
ELSE()
    MESSAGE("Doxygen not found.")
ENDIF()

IF(PANDOC)
    FILE(GLOB plot_py "${CMAKE_CURRENT_SOURCE_DIR}/../postprocessing/plot.py")
    FILE(COPY ${plot_py} DESTINATION executables)
    ADD_CUSTOM_TARGET(move_stl
        ${CMAKE_COMMAND} -E copy_directory executables/demos executables
        COMMENT "Move generated files so the tutorial_svg target can find them"
        DEPENDS generate_yaml_example generate_stl_examples
        )
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/images/tutorial_01.svg
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/./generate_images_for_tutorials.sh "${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/images"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/executables
        COMMENT "Generating tutorial SVG images" VERBATIM
        DEPENDS move_stl sim ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/generate_images_for_tutorials.sh
        )
    FILE(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/images/*.svg")
    FOREACH(f ${files})
        INSTALL(FILES ${f} DESTINATION doc/images)
    ENDFOREACH()

    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/doc.html
                       COMMAND pandoc -s --toc --mathml -f markdown introduction.md interfaces.md courbes_de_GZ.md calcul_de_GM.md solver.md reperes_et_conventions.md modeles_environnementaux.md diffraction_radiation.md modeles_efforts.md tutorial_*.md  -t html --highlight-style pygments -o doc.html  -c stylesheet.css
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user
                       DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/introduction.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/interfaces.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/calcul_de_GM.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/courbes_de_GZ.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/solver.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/reperes_et_conventions.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/modeles_environnementaux.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/diffraction_radiation.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/modeles_efforts.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/tutorial_01.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/tutorial_02.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/tutorial_03.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/tutorial_06.md
                       )
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/doc.docx
                       COMMAND pandoc -s --toc --mathml -f markdown introduction.md interfaces.md courbes_de_GZ.md solver.md reperes_et_conventions.md modeles_environnementaux.md diffraction_radiation.md modeles_efforts.md tutorial_*.md  -t html --highlight-style pygments -o doc.docx  -c stylesheet.css
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user
                       DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/introduction.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/interfaces.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/calcul_de_GM.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/courbes_de_GZ.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/solver.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/reperes_et_conventions.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/modeles_environnementaux.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/diffraction_radiation.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/modeles_efforts.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/tutorial_01.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/tutorial_02.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/tutorial_03.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/tutorial_06.md
                       )
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/doc.pdf
                       COMMAND ./doc_pdf.sh
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user
                       DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/introduction.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/interfaces.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/courbes_de_GZ.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/calcul_de_GM.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/solver.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/reperes_et_conventions.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/modeles_environnementaux.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/diffraction_radiation.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/modeles_efforts.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/tutorial_01.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/tutorial_02.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/tutorial_03.md
                               ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/tutorial_06.md
                       )

        LIST(APPEND DOC_USER_INSTALL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/doc.html)
        LIST(APPEND DOC_USER_INSTALL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/doc.docx)
        LIST(APPEND DOC_USER_INSTALL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/doc.pdf)
    ADD_CUSTOM_TARGET(doc_user ALL DEPENDS ${DOC_USER_INSTALL_FILES})
    INSTALL(FILES ${DOC_USER_INSTALL_FILES} DESTINATION doc)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../doc_user/stylesheet.css DESTINATION doc)
ELSE()
    MESSAGE(STATUS "Program PANDOC not found -> No user documentation will be generated")
ENDIF()
