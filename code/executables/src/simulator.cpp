/*
 * simulator.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include <ssc/check_ssc_version.hpp>
CHECK_SSC_VERSION(0x95e96c28e3b2eedd)

#include <ssc/exception_handling.hpp>
#include "InputData.hpp"
#include "utilities_for_InputData.hpp"
#include "simulator_run.hpp"

int main(int argc, char** argv)
{
    InputData input_data;
    int error = get_input_data(argc, argv, input_data);
    if (error)
    {
        std::cerr <<"A problem occurred while parsing inputs"<<std::endl;
        return error;
    }
    try
    {
        run_simulation(input_data);
    }
    catch(const ssc::exception_handling::Exception& e)
    {
        std::cerr <<"Error detected:"<<std::endl;
        std::cerr << e.what();
        error = 1;
    }
    return error;
}
