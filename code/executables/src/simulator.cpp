/*
 * simulator.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include "InputData.hpp"
#include "ListOfObservers.hpp"
#include "utilities_for_InputData.hpp"
#include "utilities_for_simulator.hpp"
#include "listeners.hpp"
#include "simulator_api.hpp"

#include <ssc/check_ssc_version.hpp>
#include <ssc/text_file_reader.hpp>
#include <ssc/solver.hpp>

CHECK_SSC_VERSION(0x95e96c28e3b2eedd)

void solve(const InputData& input_data, Sim& sys, ListOfObservers& observer);
void solve(const InputData& input_data, Sim& sys, ListOfObservers& observer)
{
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

void run_simulation(const InputData& input_data);
void run_simulation(const InputData& input_data)
{
    const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
    ssc::data_source::DataSource command_listener;
    if (not(input_data.command_file.empty()))
    {
        command_listener = listen_to_file(ssc::text_file_reader::TextFileReader(std::vector<std::string>(1,input_data.command_file)).get_contents());
    }
    const auto yaml = yaml_reader.get_contents();
    auto sys = get_system(yaml,input_data.tstart,command_listener);
    auto observer = get_observers(yaml, input_data);
    solve(input_data, sys, observer);
}

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
