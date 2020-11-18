IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    MESSAGE(STATUS "64 bits targets")
ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
    MESSAGE(STATUS "32 bits targets")
ELSE()
    MESSAGE(STATUS "8*${CMAKE_SIZEOF_VOID_P} bits targets")
ENDIF()

MESSAGE(STATUS "CMAKE_CXX_COMPILER_ID ${CMAKE_CXX_COMPILER_ID}")
STRING(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)

IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # using Clang
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wextra -Wall -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -fno-common -Wformat=2 -Winit-self -Wpacked -Wpointer-arith -Wmissing-declarations -Wmissing-format-attribute -Wsign-compare -Wstrict-aliasing=2 -Wundef -Wconversion")
    if (NOT("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -pedantic-errors")
    endif()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -Woverloaded-virtual -Weffc++ -Wwrite-strings -Wfatal-errors -Wno-deprecated -Wvariadic-macros")
    IF(CMAKE_SIZEOF_VOID_P EQUAL 8) # If on a 64 bit machine
        IF(UNIX AND NOT WIN32) # If on Linux
            SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
            SET(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fPIC")
        ENDIF()
    ENDIF()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -pedantic -pedantic-errors -Wextra -Wall -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -fno-common -Wformat=2 -Winit-self -Wpacked -Wp,-D_FORTIFY_SOURCE=2 -Wpointer-arith -Wmissing-declarations -Wmissing-format-attribute -Wsign-compare -Wstrict-aliasing=2 -Wundef -Wconversion")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -Woverloaded-virtual -Weffc++ -Wwrite-strings -Wfatal-errors -Wno-deprecated -Wvariadic-macros")
    IF(CMAKE_SIZEOF_VOID_P EQUAL 8) # If on a 64 bit machine
        IF(UNIX AND NOT WIN32) # If on Linux
            SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
            SET(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fPIC")
        ENDIF()
    ENDIF()

    EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                    OUTPUT_VARIABLE GCC_VERSION)
    STRING(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${GCC_VERSION})
    LIST(GET GCC_VERSION_COMPONENTS 0 GCC_MAJOR)
    # LIST(GET GCC_VERSION_COMPONENTS 1 GCC_MINOR)

    IF(CMAKE_BUILD_TYPE_UPPER MATCHES RELEASE)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DEIGEN_NO_DEBUG")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
        IF (MINGW AND (CMAKE_SIZEOF_VOID_P EQUAL 4))
            MESSAGE(STATUS "Adding specific compilation flag for Eigen with MinGW 32 Bits")
            MESSAGE(STATUS "Without this flag, program crashes...")
            # http://eigen.tuxfamily.org/bz/show_bug.cgi?id=556
            SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-ipa-cp-clone")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-ipa-cp-clone")
        ENDIF()
    ELSEIF(CMAKE_BUILD_TYPE_UPPER MATCHES COVERAGE)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pg --coverage")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg --coverage")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -pg -lgcov --coverage")
        SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -g -pg -lgcov --coverage")
        SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g -pg -lgcov --coverage")
        # INCLUDE(EnableCoverageReport)
        MESSAGE(STATUS "Adding coverage")
        FIND_PACKAGE(codecov)
    ELSEIF(CMAKE_BUILD_TYPE_UPPER MATCHES PROFILE)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -pg")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pg")
        SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -O3 -pg")
        SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -O3 -pg")
    ENDIF()

ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # using Intel C++
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Za")
ENDIF()


SET(C_FLAGS "${C_FLAGS} ${CMAKE_C_FLAGS}")
SET(CXX_FLAGS "${CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
