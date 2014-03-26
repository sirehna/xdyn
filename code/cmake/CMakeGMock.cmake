ADD_DEFINITIONS("-DGTEST_HAS_POSIX_RE=0")
IF(ARCH64_BITS AND UNIX AND NOT WIN32) # If on Linux
    MESSAGE(STATUS "1")
    #ADD_DEFINITIONS("-Dgtest_disable_pthreads=1")
    #SET(gtest_disable_pthreads ON CACHE BOOL "")
ELSE()
    MESSAGE(STATUS "2")
    ADD_DEFINITIONS("-Dgtest_disable_pthreads=1")
    SET(gtest_disable_pthreads ON CACHE BOOL "")
ENDIF()

ADD_SUBDIRECTORY(gmock)

SET(GMOCK_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/gmock/include
                       ${CMAKE_CURRENT_SOURCE_DIR}/gmock/include/gmock)
SET(GTEST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/gmock/gtest/include
                       ${CMAKE_CURRENT_SOURCE_DIR}/gmock/gtest/include/gmock)
