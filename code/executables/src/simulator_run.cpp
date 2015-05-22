#include "utilities_for_simulator.hpp"
#include "listeners.hpp"
#include "simulator_api.hpp"
#include "simulator_run.hpp"

#include <ssc/text_file_reader.hpp>
#include <ssc/solver.hpp>

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
