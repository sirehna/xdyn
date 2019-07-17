# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST observers_and_api)
project(${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC
        src/ListOfObserversTest.cpp
        src/Hdf5ObserverTest.cpp
        src/Hdf5WaveObserverTest.cpp
        src/Hdf5WaveObserverBuilderTest.cpp
        src/JsonObserverTest.cpp
        src/SimTest.cpp
        src/ForceTester.cpp
        src/ForceTests.cpp
        src/MapObserverTest.cpp
        src/ObserverTests.cpp
        src/SimStepperTest.cpp
        src/ConfBuilderTest.cpp
        src/HistoryParserTest.cpp
        src/XdynForCSTest.cpp
        src/XdynForMETest.cpp
        src/EverythingObserverTest.cpp
        )
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(inc)
include_directories(${${MODULE_UNDER_TEST}_INCLUDE_DIRS})
include_directories(${ssc_INCLUDE_DIRS})
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
include_directories(SYSTEM ${YAML_CPP_INCLUDE_DIRS})
include_directories(${core_INCLUDE_DIRS})
include_directories(${mesh_INCLUDE_DIRS})
include_directories(${environment_models_INCLUDE_DIRS})
include_directories(${test_data_generator_INCLUDE_DIRS})
include_directories(${binary_stl_data_INCLUDE_DIRS})
include_directories(${hdb_interpolators_INCLUDE_DIRS})

IF(WIN32)
    IF(${CMAKE_SYSTEM_VERSION} STRLESS 6.0)
        MESSAGE(STATUS "Modification of compilation option to prevent a crashing bug")
        MESSAGE(STATUS "    CMAKE_SYSTEM_VERSION ${CMAKE_SYSTEM_VERSION} -> Running on Windows XP")
        MESSAGE(STATUS "    There is a crashing bug when using optimized compilation on Windows XP with MinGW, Boost Asio and WebSocketPP")
        MESSAGE(STATUS "    This bug therefore appears in RELEASE mode (-O3)")
        MESSAGE(STATUS "    Apparently, the bug is located in a thread, needed to create WebSocketPP tests requiring a threaded server and a client")
        MESSAGE(STATUS "    To fix this bug, one forces the -O0 compilation option for all files that uses thread")
        MESSAGE(STATUS "    ")
        SET_SOURCE_FILES_PROPERTIES(src/WebSocketObserverTest.cpp PROPERTIES COMPILE_FLAGS -O0)
    ENDIF()
ENDIF()

add_library(${PROJECT_NAME} OBJECT ${SRC})
