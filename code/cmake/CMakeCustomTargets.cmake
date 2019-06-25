ADD_CUSTOM_TARGET(
    sloccount
    sloccount `find . -maxdepth 1 -type d ! \\\( -name \".\" -o -name \"tools_ThirdParty\" -o -name \"doc\" -o -name \"data\" -o -name \"*bin*\" -o -name \".svn\" -o -name \".git\" -o -name \".settings\" \\\)`> "${PROJECT_BINARY_DIR}/sloccount.txt"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Evaluates the amount of work of the project"
)

INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/../postprocessing/MatLab/" DESTINATION "matlab")

MESSAGE(STATUS "Related xdyn proto files will be installed in proto directory")
INSTALL(FILES "${PROJECT_SOURCE_DIR}/grpc/force.proto" DESTINATION "proto")
INSTALL(FILES "${PROJECT_SOURCE_DIR}/waves_grpc/wave_grpc.proto" DESTINATION "proto")
INSTALL(FILES "${PROJECT_SOURCE_DIR}/waves_grpc/wave_types.proto" DESTINATION "proto")

FILE(GLOB script ${CMAKE_SOURCE_DIR}/integration_tests.py)
ADD_CUSTOM_TARGET(python_script)
ADD_CUSTOM_COMMAND(TARGET python_script POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${script}
                                               ${PROJECT_BINARY_DIR}/executables)
ADD_CUSTOM_TARGET(integration_tests
    DEPENDS x-dyn
            generate_yaml_example
            generate_stl_examples
            python_script
)

ADD_CUSTOM_COMMAND(
    TARGET integration_tests
    POST_BUILD
    COMMAND generate_yaml_example
    COMMAND generate_stl_examples
    COMMAND python integration_tests.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/executables
    COMMENT "Checking the tutorials execute OK"
)

ADD_CUSTOM_TARGET(
    archive
    COMMAND export dateSTR=`date +'%Y_%m_%d__%H_%M'` &&
            export pkg0=`echo $$PWD` &&
            export pkg=`echo \$\${PWD\#\#*/}` &&
            export tarName=`echo \$\${pkg}__\$\${dateSTR}` &&
            export dirToSave=`find . -maxdepth 1 ! \\\( -name \".\" -o -name \$\${pkg}__*.tar.gz -o -name \$\${pkg}__*.tgz -o -name \"Results*\" -o -name \"data\" -o -name \"bin\" -o -regex \"bin*\" -o -name \"build\" \\\) | cut -f2 -d"/"` &&
            tar -czf \$\${tarName}.tgz \$\${dirToSave}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Create an archive of the project"
)

# IF(CMAKE_BUILD_TYPE_UPPER MATCHES COVERAGE)
#     ENABLE_COVERAGE_REPORT(TARGETS ${TEST_EXE}
#                            FILTER boost eigen gmock lapack
#                                   /c/mingw/lib/gcc /usr/include)
# ENDIF()
