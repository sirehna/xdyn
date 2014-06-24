/*
 * simulator.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include "TextFileReader.hpp"
#include "DataSource.hpp"
#include "simulator_api.hpp"
#include "DsSolve.hpp"
#include "DsSystemMacros.hpp"
#include "DataSource.hpp"
#include "InputData.hpp"
#include "utilities_for_InputData.hpp"

#include <string>
#include <cstdlib> // EXIT_FAILURE, EXIT_SUCCESS

int main(int argc, char** argv)
{
    InputData input_data;
    const int error = get_input_data(argc, argv, input_data);
    if (not(error))
    {
        const TextFileReader yaml_reader(input_data.yaml_filenames);
        DataSource ds = make_ds(yaml_reader.get_contents(),input_data.initial_timestep,input_data.solver);
        ds.check_in(__PRETTY_FUNCTION__);
        TR1(shared_ptr)<DsObserver> observer = build_observer(input_data);
        integrate(ds, input_data.tstart, input_data.tend, *observer);
        ds.check_out();
    }
    return error;
}
