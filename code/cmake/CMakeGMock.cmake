ADD_DEFINITIONS("-DGTEST_HAS_POSIX_RE=0")
IF(UNIX AND NOT WIN32) # If on Linux
    SET(gtest_disable_pthreads OFF CACHE BOOL "")
ELSE()
    SET(gtest_disable_pthreads ON CACHE BOOL "")
ENDIF()

ADD_SUBDIRECTORY(google-test)
SET(GMOCK_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/google-test/googlemock/include
                       ${CMAKE_CURRENT_SOURCE_DIR}/google-test/googlemock/include/gmock/)
SET(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/google-test/googletest/include
                       ${CMAKE_CURRENT_SOURCE_DIR}/google-test/googletest/include/gtest)

