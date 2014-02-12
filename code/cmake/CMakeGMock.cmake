IF(WIN32)
    ADD_DEFINITIONS("-DGTEST_HAS_POSIX_RE=0")
    ADD_DEFINITIONS("-Dgtest_disable_pthreads=0")
    SET(gtest_disable_pthreads ON  CACHE BOOL "")
ELSE()
    ADD_DEFINITIONS("-DGTEST_HAS_POSIX_RE=1")
    ADD_DEFINITIONS("-Dgtest_disable_pthreads=1")
    SET(gtest_disable_pthreads OFF CACHE BOOL "")
ENDIF()
ADD_SUBDIRECTORY(gmock)

SET(GMOCK_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/gmock/include
                       ${CMAKE_CURRENT_SOURCE_DIR}/gmock/include/gmock)
SET(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/gmock/gtest/include
                       ${CMAKE_CURRENT_SOURCE_DIR}/gmock/gtest/include/gmock)