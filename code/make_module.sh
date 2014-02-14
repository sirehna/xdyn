#!/bin/sh
mkdir -p $1/inc
mkdir -p $1/src
mkdir -p $1/unit_tests/inc
mkdir -p $1/unit_tests/src
echo "cmake_minimum_required (VERSION 2.8.8)" >> $1/CMakeLists.txt
echo "project ($1)" >> $1/CMakeLists.txt
echo "" >> $1/CMakeLists.txt
echo "set(SRC src/.cpp" >> $1/CMakeLists.txt
echo "        )" >> $1/CMakeLists.txt
echo "" >> $1/CMakeLists.txt
echo "" >> $1/CMakeLists.txt
echo "include_directories(inc)" >> $1/CMakeLists.txt
echo "include_directories(\${exception_handling_INCLUDE_DIRS})" >> $1/CMakeLists.txt
echo "include_directories(\${test_macros_INCLUDE_DIRS})" >> $1/CMakeLists.txt
echo "" >> $1/CMakeLists.txt
echo "add_library (\${PROJECT_NAME}_static STATIC \${SRC})" >> $1/CMakeLists.txt
echo "set(\${PROJECT_NAME}_INCLUDE_DIRS \${\${PROJECT_NAME}_SOURCE_DIR}/inc CACHE PATH \"Path to \${PROJECT_NAME}'s include directory\")" >> $1/CMakeLists.txt
echo "" >> $1/CMakeLists.txt
echo "add_subdirectory(unit_tests)" >> $1/CMakeLists.txt
echo "# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----" >> $1/unit_tests/CMakeLists.txt
echo "set(MODULE_UNDER_TEST $1)" >> $1/unit_tests/CMakeLists.txt
echo "project (\${MODULE_UNDER_TEST}_tests)" >> $1/unit_tests/CMakeLists.txt
echo "FILE(GLOB SRC src/DataSourceTest.cpp" >> $1/unit_tests/CMakeLists.txt
echo "              src/SignalContainerTest.cpp" >> $1/unit_tests/CMakeLists.txt
echo "              src/TypeCoercionTest.cpp)" >> $1/unit_tests/CMakeLists.txt
echo "# ------8<---------------------------------------------->8-----" >> $1/unit_tests/CMakeLists.txt
echo "" >> $1/unit_tests/CMakeLists.txt
echo "# Include directories" >> $1/unit_tests/CMakeLists.txt
echo "include_directories(SYSTEM inc)" >> $1/unit_tests/CMakeLists.txt
#echo "include_directories(\${\${MODULE_UNDER_TEST}_INCLUDE_DIRS})" >> $1/unit_tests/CMakeLists.txt
#echo "include_directories(\${random_data_generator_INCLUDE_DIRS})" >> $1/unit_tests/CMakeLists.txt
#echo "include_directories(\${test_macros_SOURCE_DIR})" >> $1/unit_tests/CMakeLists.txt
#echo "include_directories(SYSTEM \${GTEST_INCLUDE_DIRS})" >> $1/unit_tests/CMakeLists.txt
#echo "include_directories(SYSTEM \${GMOCK_INCLUDE_DIRS})" >> $1/unit_tests/CMakeLists.txt
#echo "" >> $1/unit_tests/CMakeLists.txt
#echo "add_library (\${PROJECT_NAME} OBJECT \${SRC})" >> $1/unit_tests/CMakeLists.txt
