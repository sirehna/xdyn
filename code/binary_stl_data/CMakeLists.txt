cmake_minimum_required(VERSION 2.8.8)
project(binary_stl_data)

set(SRC src/generate_anthineas.cpp
        )

IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # Otherwise CLang generates fatal error: string literal of length 253876 exceeds maximum length 65536 that C++ compilers are required to support 
    # using Clang
    SET(CMAKE_C_FLAGS "-Werror -Wextra -Wall -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -fno-common -Wformat=2 -Winit-self -Wpacked -Wp,-D_FORTIFY_SOURCE=2 -Wpointer-arith -Wmissing-declarations -Wmissing-format-attribute -Wsign-compare -Wstrict-aliasing=2 -Wundef -Wconversion")
    SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Woverloaded-virtual -Weffc++ -Wwrite-strings -Wfatal-errors -Wno-deprecated -Wvariadic-macros")
ENDIF()

include_directories(inc)
include_directories(${exception_handling_INCLUDE_DIRS})
include_directories(${test_macros_INCLUDE_DIRS})
include_directories(${external_file_formats_INCLUDE_DIRS})
include_directories(${external_data_structures_INCLUDE_DIRS})
include_directories(SYSTEM ${eigen_INCLUDE_DIRS})
include_directories(SYSTEM ${base91x_INCLUDE_DIRS})

add_library (${PROJECT_NAME}_static STATIC ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")
