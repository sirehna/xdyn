/*
 * simulator.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include <exception>

#include <ssc/check_ssc_version.hpp>
CHECK_SSC_VERSION(6,0)

#include <iostream>
#include "InputData.hpp"
#include "utilities_for_InputData.hpp"
#include "simulator_run.hpp"


int run(const InputData& input_data);
int run(const InputData& input_data)
{
    if (not(input_data.empty())) run_simulation(input_data);
    return EXIT_SUCCESS;
}



int main(int argc, char** argv)
{


    InputData input_data;
    int error = 0;
    try
    {
        if (argc==1) return fill_input_or_display_help(argv[0], input_data);
        error = get_input_data(argc, argv, input_data);
    }
    catch(boost::program_options::error& e)
    {
      std::cerr << "The command line you supplied is not valid: " << e.what() << std::endl << "Use --help to get the list of available parameters." << std::endl;
      return -1;
    }
    if (error)
    {
        std::cerr <<"A problem occurred while parsing inputs" << std::endl;
        return error;
    }
    if (input_data.catch_exceptions)
    {
        try
        {
            return run(input_data);
        }
        catch (const std::exception& e)
        {
            std::cerr << "An internal error has occurred: " << e.what() << std::endl;
            return -1;
        }
    }
    return run(input_data);
}
