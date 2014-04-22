cmake_minimum_required(VERSION 2.8.8)
project(boost_program_options_descriptions)

set(SRC src/CustomOptionDescription.cpp
        src/OptionPrinter.cpp
        )


include_directories(inc)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_library (${PROJECT_NAME}_static STATIC ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")
