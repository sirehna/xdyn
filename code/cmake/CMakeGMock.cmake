ADD_DEFINITIONS("-DGTEST_HAS_POSIX_RE=0")
IF(UNIX AND NOT WIN32) # If on Linux
    SET(gtest_disable_pthreads OFF CACHE BOOL "")
ELSE()
    SET(gtest_disable_pthreads ON CACHE BOOL "")
ENDIF()

IF(EXISTS gmock)
    ADD_SUBDIRECTORY(gmock)
    SET(GMOCK_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/gmock/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/gmock/include/gmock)
    SET(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/gmock/gtest/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/gmock/gtest/include/gmock)
ELSE()
    ADD_SUBDIRECTORY(googletest)
    SET(GMOCK_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googlemock/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googlemock/include/gmock)
    SET(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include/gmock)
ENDIF()
 