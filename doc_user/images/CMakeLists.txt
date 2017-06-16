CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(XDYN-IMAGES)

SET(CMAKE_VERBOSE_MAKEFILE OFF)

# List of figures generated with Python script spectrum.py
SET(SPECTRA spectrumBretschneider spectrumGaussian spectrumITTC spectrumJonswap
            spectrumOchi spectrumOneraTILV10 spectrumPiersonMoskowitz
            spectrumVignatBovis spectrumComparison1 spectrumComparison2
            waveMonochromatique waveBichromatique)

FOREACH(s ${SPECTRA})
    LIST(APPEND SPECTRA_FULL_FILENAME ${CMAKE_CURRENT_SOURCE_DIR}/${s}.svg)
ENDFOREACH()

# MESSAGE(STATUS "SPECTRA_FULL_FILENAME ${SPECTRA_FULL_FILENAME}")

FILE(GLOB SVG_IMAGES "${CMAKE_CURRENT_SOURCE_DIR}/*.svg")
# MESSAGE(STATUS "SVG_IMAGES : ${SVG_IMAGES}")

FOREACH(s ${SPECTRA_FULL_FILENAME})
    LIST(REMOVE_ITEM SVG_IMAGES ${s})
ENDFOREACH()
# MESSAGE(STATUS "SVG_IMAGES : ${SVG_IMAGES}")

#FOREACH(s ${SVG_IMAGES})
#    STRING(REGEX REPLACE ".svg$" ".png" PNG_IMAGE ${s})
#    LIST(APPEND PNG_IMAGES ${PNG_IMAGE})
#    # MESSAGE(STATUS "Creating target ${PNG_IMAGE}")
#    ADD_CUSTOM_COMMAND(OUTPUT ${PNG_IMAGE}
#                       COMMAND inkscape -z -e ${PNG_IMAGE} ${s} -d 300
#                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                       COMMENT "Generating ${PNG_IMAGE}"
#                       )
#ENDFOREACH()

# One checks if command FIND_PACKAGE(PythonInterp 3.4) has been run from an
# upper CMakeLists.txt project
#
# If not, we choose the default python executable
IF(NOT PYTHONINTERP_FOUND)
    SET(PYTHON_EXECUTABLE python)
ENDIF()

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/spectrumBretschneider.svg
                   COMMAND ${PYTHON_EXECUTABLE} spectrum.py
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/spectrum.py
                   COMMENT "Generating spectra images"
                   )

SET(IMAGE_SVG_SPECTRA_DEPENDENCIES ${CMAKE_CURRENT_SOURCE_DIR}/spectrumBretschneider.svg
    CACHE PATH "Path SVG spectra image dependency")

ADD_CUSTOM_TARGET(
    doc_images_svg
    DEPENDS ${SVG_IMAGES}
            ${CMAKE_CURRENT_SOURCE_DIR}/spectrumBretschneider.svg
    )

ADD_CUSTOM_TARGET(
    doc_images_png
    DEPENDS ${PNG_IMAGES}
    )

IF(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    ADD_CUSTOM_TARGET(
        doc ALL
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/spectrumBretschneider.svg ${PNG_IMAGES}
        )
ENDIF()
