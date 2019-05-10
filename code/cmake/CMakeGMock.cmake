ADD_DEFINITIONS("-DGTEST_HAS_POSIX_RE=0")
IF(UNIX AND NOT WIN32) # If on Linux
    SET(gtest_disable_pthreads OFF CACHE BOOL "")
ELSE()
    SET(gtest_disable_pthreads ON CACHE BOOL "")
ENDIF()

ADD_SUBDIRECTORY(${THIRD_PARTY_DIRECTORY}/googletest ${CMAKE_CURRENT_BINARY_DIR}/googletest EXCLUDE_FROM_ALL)
SET(GMOCK_INCLUDE_DIRS ${THIRD_PARTY_DIRECTORY}/googletest/googlemock/include
                       ${THIRD_PARTY_DIRECTORY}/googletest/googlemock/include/gmock/)
SET(GTEST_INCLUDE_DIRS ${THIRD_PARTY_DIRECTORY}/googletest/googletest/include
                       ${THIRD_PARTY_DIRECTORY}/googletest/googletest/include/gtest)

