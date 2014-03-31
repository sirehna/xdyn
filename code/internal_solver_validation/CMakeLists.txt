CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(internal_solver_validation)

ADD_SUBDIRECTORY(external_data_structures)
ADD_SUBDIRECTORY(parser)
ADD_SUBDIRECTORY(test_utilities)
ADD_SUBDIRECTORY(dynamic_model)
ADD_SUBDIRECTORY(dynamho_interface)
ADD_SUBDIRECTORY(physical_models)
ADD_SUBDIRECTORY(integration_tests)
ADD_SUBDIRECTORY(DyNaMHo)


ADD_LIBRARY(${PROJECT_NAME} STATIC  
        $<TARGET_OBJECTS:dynamho_interface_object>
        $<TARGET_OBJECTS:dynamho_yaml_data_generator>
        $<TARGET_OBJECTS:dynamho_object>
        $<TARGET_OBJECTS:dynamic_model_object>
        $<TARGET_OBJECTS:external_data_structures_object>
        $<TARGET_OBJECTS:parser_object>
        $<TARGET_OBJECTS:physical_models_object>)
