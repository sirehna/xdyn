IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    MESSAGE(STATUS "64 bits targets")
ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
    MESSAGE(STATUS "32 bits targets")
ELSE()
    MESSAGE(STATUS "8*${CMAKE_SIZEOF_VOID_P} bits targets")
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCC)
    EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                    OUTPUT_VARIABLE GCC_VERSION)
    STRING(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${GCC_VERSION})
    LIST(GET GCC_VERSION_COMPONENTS 0 GCC_MAJOR)
    LIST(GET GCC_VERSION_COMPONENTS 1 GCC_MINOR)
ENDIF()

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -pedantic -pedantic-errors -Wextra -Wall -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -fno-common -Wformat=2 -Winit-self -Wpacked -Wp,-D_FORTIFY_SOURCE=2 -Wpointer-arith -Wlarger-than-65500 -Wmissing-declarations -Wmissing-format-attribute -Wsign-compare -Wstrict-aliasing=2 -Wundef -ffast-math -Wconversion")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -Woverloaded-virtual -Weffc++ -Wwrite-strings -Wfatal-errors -Wno-deprecated -Wvariadic-macros")
IF(CMAKE_SIZEOF_VOID_P EQUAL 8) # If on a 64 bit machine
    IF(UNIX AND NOT WIN32) # If on Linux
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
        SET(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fPIC")
    ENDIF()
ENDIF()

IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ENDIF()

STRING(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)

IF(CMAKE_BUILD_TYPE_UPPER MATCHES COVERAGE)
    #SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage")
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage")
    #SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -g -fprofile-arcs -ftest-coverage")
    #SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g -fprofile-arcs -ftest-coverage")


    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pg --coverage")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg --coverage")
    SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -g -pg --coverage")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g -pg --coverage")
    INCLUDE(EnableCoverageReport)
ENDIF()

IF(CMAKE_BUILD_TYPE_UPPER MATCHES RELEASE)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DEIGEN_NO_DEBUG")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
ENDIF()

SET(C_FLAGS "${C_FLAGS} ${CMAKE_C_FLAGS}")
SET(CXX_FLAGS "${CXX_FLAGS} ${CMAKE_CXX_FLAGS}")

## with -fPIC
#IF(UNIX AND NOT WIN32)
#    FIND_PROGRAM(CMAKE_UNAME uname /bin /usr/bin /usr/local/bin )
#    IF(CMAKE_UNAME)
#        EXEC_PROGRAM(uname ARGS -m OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
#        SET(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE INTERNAL "processor type (i386 and x86_64)")
#        IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
#            SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
#            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
#            SET(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fPIC")
#            #SET(CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH /usr/lib64)
#            #ADD_DEFINITIONS(-fPIC)
#            #SET(EXTERNAL_PROJECT_OPTIONS -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_C_FLAGS=-fPIC)
#        ENDIF()
#    ENDIF()
#ENDIF()
