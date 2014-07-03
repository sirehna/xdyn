/*
 * simulator.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include "TextFileReader.hpp"
#include "InputData.hpp"
#include "utilities_for_InputData.hpp"

#include "simulator_api.hpp"
#include "SimCsvObserver.hpp"
#include "solve.hpp"
#include "steppers.hpp"

#include <string>
#include <cstdlib> // EXIT_FAILURE, EXIT_SUCCESS
#include <fstream>

int main(int argc, char** argv)
{
    InputData input_data;
    const int error = get_input_data(argc, argv, input_data);
    if (not(error))
    {
        const TextFileReader yaml_reader(input_data.yaml_filenames);
        auto sys = get_system(yaml_reader.get_contents());
        std::ofstream os(input_data.output_csv.c_str());
        initialize_stream(os, input_data);
        SimCsvObserver observer(os);
        if (input_data.solver=="euler")
        {
            quicksolve<EulerStepper>(sys, input_data.tstart, input_data.tend, input_data.initial_timestep, observer);
        }
        else if (input_data.solver=="rk4")
        {
            quicksolve<RK4Stepper>(sys, input_data.tstart, input_data.tend, input_data.initial_timestep, observer);
        }
        else if (input_data.solver=="rkck")
        {
            quicksolve<RKCK>(sys, input_data.tstart, input_data.tend, input_data.initial_timestep, observer);
        }
        else
        {
            quicksolve<EulerStepper>(sys, input_data.tstart, input_data.tend, input_data.initial_timestep, observer);
        }
    }
    return error;
}
