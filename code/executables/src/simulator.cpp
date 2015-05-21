/*
 * simulator.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include "InputData.hpp"
#include "ListOfObservers.hpp"
#include "utilities_for_InputData.hpp"
#include "listeners.hpp"
#include "parse_output.hpp"
#include "simulator_api.hpp"

#include <ssc/check_ssc_version.hpp>
#include <ssc/text_file_reader.hpp>
#include <ssc/solver.hpp>

CHECK_SSC_VERSION(0x95e96c28e3b2eedd)

ListOfObservers get_observers(const std::string& yaml, const InputData& input_data);
ListOfObservers get_observers(const std::string& yaml, const InputData& input_data)
{
    auto out = parse_output(yaml);
    if (not(input_data.wave_output.empty()))
    {
        YamlOutput o;
        o.data = {"waves"};
        o.filename = input_data.wave_output;
        o.format = get_format(input_data.wave_output);
        out.push_back(o);
    }
    out.push_back(generate_default_outputter_with_all_states_in_it(yaml, input_data.output_filename));
    return ListOfObservers(out);
}

int main(int argc, char** argv)
{
    InputData input_data;
    int error = get_input_data(argc, argv, input_data);
    try
    {
        if (not(error))
        {
            const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
            ssc::data_source::DataSource command_listener;
            if (not(input_data.command_file.empty()))
            {
                command_listener = listen_to_file(ssc::text_file_reader::TextFileReader(std::vector<std::string>(1,input_data.command_file)).get_contents());
            }
            const auto yaml = yaml_reader.get_contents();
            auto observer = get_observers(yaml, input_data);
            auto sys = get_system(yaml,input_data.tstart,command_listener);
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
    }
    catch(const ssc::exception_handling::Exception& e)
    {
        std::cerr <<"Error detected:"<<std::endl;
        std::cerr << e.what();
        error = 1;
    }
    return error;
}
