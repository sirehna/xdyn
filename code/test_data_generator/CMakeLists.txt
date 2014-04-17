cmake_minimum_required (VERSION 2.8.8)
project (test_data_generator)

set(SRC src/STL_data.cpp
<<<<<<< HEAD
=======
        src/yaml_data.cpp
>>>>>>> 76267ed94b4d397077d5e77b61863f3dde24d7fd
        )


include_directories(inc)

add_library (${PROJECT_NAME}_static STATIC ${SRC})
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH "Path to ${PROJECT_NAME}'s include directory")

