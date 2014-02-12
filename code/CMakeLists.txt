CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(simulatorIrtJv)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(CMakeGMock)

SET(CMAKE_INSTALL_PREFIX ../install)

FOREACH(f RUNTIME_OUTPUT_DIRECTORY
          LIBRARY_OUTPUT_DIRECTORY
          ARCHIVE_OUTPUT_DIRECTORY)
    IF(NOT DEFINED ${f})
        SET(${f} "bin")
    ENDIF()
ENDFOREACH()

IF(CMAKE_COMPILER_IS_GNUCC)
    EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                    OUTPUT_VARIABLE GCC_VERSION)
    STRING(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${GCC_VERSION})
    LIST(GET GCC_VERSION_COMPONENTS 0 GCC_MAJOR)
    LIST(GET GCC_VERSION_COMPONENTS 1 GCC_MINOR)
ENDIF()

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wfatal-errors -pedantic -pedantic-errors -Wextra -Wall -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -Wvariadic-macros -fno-common -Wformat=2 -Winit-self -Wpacked -Wp,-D_FORTIFY_SOURCE=2 -Wpointer-arith -Wlarger-than-65500 -Wmissing-declarations -Wmissing-format-attribute -Wno-deprecated -Wsign-compare -Wstrict-aliasing=2 -Wundef -ffast-math -fomit-frame-pointer")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -Woverloaded-virtual -Weffc++ -Wwrite-strings")
SET(C_FLAGS "${CMAKE_C_FLAGS}")
SET(CXX_FLAGS "${CMAKE_CXX_FLAGS}")

SET(boost_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Path to Boost's include directory")
ADD_SUBDIRECTORY(macros)
ADD_SUBDIRECTORY(random_data_generator)
ADD_SUBDIRECTORY(exception_handling)
ADD_SUBDIRECTORY(data_source)
ADD_SUBDIRECTORY(interpolation)
ADD_SUBDIRECTORY(simulation)

SET(BUILD_TESTING FALSE)
ADD_SUBDIRECTORY(lapack)

ADD_LIBRARY(${PROJECT_NAME} SHARED
        $<TARGET_OBJECTS:exception_handling_object>
        $<TARGET_OBJECTS:interpolation_object>)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        data_source_static          # static
        simulation_static           # static
        lapack                      # static
        gfortran                    # static
        )

INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}
        LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})

#############################################################################
ENABLE_TESTING()
SET(TEST_EXE run_all_tests)

ADD_EXECUTABLE(${TEST_EXE}
        $<TARGET_OBJECTS:data_source_tests>
        $<TARGET_OBJECTS:simulation_tests>
        $<TARGET_OBJECTS:interpolation_object>)

SET(LINK_FLAGS ${LINK_FLAGS} "-Wl,-whole-archive")

TARGET_LINK_LIBRARIES(${TEST_EXE}
        gtest gtest_main                 # static
        gmock gmock_main                 # static
        random_data_generator_static     # static
        ${PROJECT_NAME}
        )

INSTALL(TARGETS ${TEST_EXE}
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}
        LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})

ADD_TEST(${CTEST_EXE} ${TEST_EXE} WORKING_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
#############################################################################


FOREACH(f "doc/manual.pdf")
    IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../${f}")
        INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../${f}"
                DESTINATION doc)
    ENDIF()
ENDFOREACH()

INCLUDE(CMakeCustomTargets)
INCLUDE(CMakePack)

