/*
 * simulator.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include <ssc/check_ssc_version.hpp>
CHECK_SSC_VERSION(0xcb9745b34670edcb)

#include <iostream>
#include "InputData.hpp"
#include "utilities_for_InputData.hpp"
#include "simulator_run.hpp"

int main(int argc, char** argv)
{
    InputData input_data;
    if (argc==1) return display_help(argv[0], input_data);
    int error = get_input_data(argc, argv, input_data);
    if (error)
    {
        std::cerr <<"A problem occurred while parsing inputs"<<std::endl;
        return error;
    }
    if (input_data.empty()) return EXIT_SUCCESS;
    run_simulation(input_data);
    return error;
}
