/*
 * simulator.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include <ssc/text_file_reader.hpp>
#include "InputData.hpp"
#include "utilities_for_InputData.hpp"

#include "simulator_api.hpp"
#include "SimCsvObserver.hpp"
#include <ssc/solver.hpp>

#include <string>
#include <fstream>

int main(int argc, char** argv)
{
    InputData input_data;
    const int error = get_input_data(argc, argv, input_data);
    if (not(error))
    {
        const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
        auto sys = get_system(yaml_reader.get_contents());
        std::ofstream os(input_data.output_csv.c_str());
        std::ofstream ws(input_data.wave_output.c_str());
        initialize_stream(os, input_data);
        if (input_data.wave_output.empty()) copy_stream(dev_null_buffer, ws);
        SimCsvObserver observer(os, ws);
        if (input_data.solver=="euler")
        {
            ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, input_data.tstart, input_data.tend, input_data.initial_timestep, observer);
        }
        else if (input_data.solver=="rk4")
        {
            ssc::solver::quicksolve<ssc::solver::RK4Stepper>(sys, input_data.tstart, input_data.tend, input_data.initial_timestep, observer);
        }
        else if (input_data.solver=="rkck")
        {
            ssc::solver::quicksolve<ssc::solver::RKCK>(sys, input_data.tstart, input_data.tend, input_data.initial_timestep, observer);
        }
        else
        {
            ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, input_data.tstart, input_data.tend, input_data.initial_timestep, observer);
        }
    }
    return error;
}
