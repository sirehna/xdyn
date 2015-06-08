ADD_CUSTOM_TARGET(
    sloccount
    sloccount `find . -maxdepth 1 -type d ! \\\( -name \".\" -o -name \"tools_ThirdParty\" -o -name \"doc\" -o -name \"data\" -o -name \"*bin*\" -o -name \".svn\" -o -name \".git\" -o -name \".settings\" \\\)`> "${PROJECT_BINARY_DIR}/sloccount.txt"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Evaluates the amount of work of the project"
)

FILE(GLOB script ${CMAKE_SOURCE_DIR}/integration_tests.py)
ADD_CUSTOM_TARGET(python_script)
ADD_CUSTOM_COMMAND(TARGET python_script POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${script}
                                               ${PROJECT_BINARY_DIR}/executables)
ADD_CUSTOM_TARGET(integration_tests
    DEPENDS sim
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

ADD_CUSTOM_TARGET(sim_server)
IF(PYTHONINTERP_FOUND AND PY_CX_FREEZE AND PY_TORNADO)
    FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/python_server/server)
    ADD_CUSTOM_COMMAND(
        TARGET sim_server
        POST_BUILD
        COMMAND ${PYTHON_EXECUTABLE} setup.py install_exe -d ${CMAKE_CURRENT_BINARY_DIR}/python_server/server
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../html
        COMMENT "Generate simulator server"
    )
    INSTALL(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/python_server/server"
            DESTINATION ".")
    FILE(GLOB files "${CMAKE_CURRENT_BINARY_DIR}/python_server/server/server*")
    FOREACH(f ${files})
        INSTALL(FILES ${f} DESTINATION server PERMISSIONS OWNER_EXECUTE)
    ENDFOREACH()
    INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/../html/css"
            DESTINATION "server")
    INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/../html/static"
            DESTINATION "server")
    INSTALL(FILES "${PROJECT_SOURCE_DIR}/../html/websocket_test.html"
            DESTINATION "server")
ELSE()
    ADD_CUSTOM_COMMAND(
        TARGET sim_server
        POST_BUILD
        COMMENT "No simulator server will be built: platform does not meet requirements"
    )
ENDIF()

ADD_CUSTOM_TARGET(
    md5sum
    COMMAND find . -type f \\\( -name \"*.c\" -o -name \"*.h\" \\\) -print0 | xargs -0 md5sum > "${PROJECT_BINARY_DIR}/md5sum.txt"
    COMMAND find . -type f \\\( -name \"*.cpp\" -o -name \"*.hpp\" \\\) -print0 | xargs -0 md5sum >> "${PROJECT_BINARY_DIR}/md5sum.txt"
    COMMAND find . -type f \\\( -name \"*.cxx\" -o -name \"*.hxx\" \\\) -print0 | xargs -0 md5sum >> "${PROJECT_BINARY_DIR}/md5sum.txt"
    COMMAND find . -type f -name \"*.sh\" -print0 | xargs -0 md5sum >> "${PROJECT_BINARY_DIR}/md5sum.txt"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Create a file named md5sum.txt that contains the md5 hash of all source files"
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

IF(CMAKE_BUILD_TYPE_UPPER MATCHES COVERAGE)
    ENABLE_COVERAGE_REPORT(TARGETS ${TEST_EXE}
                           FILTER boost eigen gmock lapack
                                  /c/mingw/lib/gcc /usr/include)
ENDIF()

