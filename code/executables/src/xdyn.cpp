/*
 * simulator.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include <exception>
#include <functional>
#include <iostream>
#include <google/protobuf/stubs/common.h>
#include "yaml-cpp/exceptions.h"

#include <ssc/check_ssc_version.hpp>
#include <ssc/text_file_reader.hpp>

#include <ssc/exception_handling.hpp>

#include "build_observers_description.hpp"
#include "ConnexionError.hpp"
#include "InternalErrorException.hpp"
#include "listeners.hpp"
#include "MeshException.hpp"
#include "NumericalErrorException.hpp"
#include "parse_XdynCommandLineArguments.hpp"
#include "simulator_api.hpp"
#include "SurfaceElevationInterface.hpp"
#include "XdynCommandLineArguments.hpp"

#include <ssc/solver.hpp>
#include "report_xdyn_exceptions_to_user.hpp"

CHECK_SSC_VERSION(8,0)

void solve(const XdynCommandLineArguments& input_data, Sim& sys, ListOfObservers& observer);
void solve(const XdynCommandLineArguments& input_data, Sim& sys, ListOfObservers& observer)
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

void serialize_context_if_necessary_new(ListOfObservers& observers, const Sim& sys);
void serialize_context_if_necessary_new(ListOfObservers& observers, const Sim& sys)
{
    const auto env = sys.get_env();
    const auto w = env.w;
    if (w)
    {
        for (auto observer:observers.get())
        {
            w->serialize_wave_spectra_before_simulation(observer);
        }
    }
}

#include "stl_io_hdf5.hpp"
#include "h5_tools.hpp"
void serialize_context_if_necessary(std::vector<YamlOutput>& observers, const Sim& sys, const std::string& yaml_input, const std::string& prog_command);
void serialize_context_if_necessary(std::vector<YamlOutput>& observers, const Sim& sys, const std::string& yaml_input, const std::string& prog_command)
{
    for (const auto observer:observers)
    {
        if(observer.format=="hdf5")
        {
            if (not(prog_command.empty()))
            {
                H5_Tools::write(observer.filename, "/inputs/command", prog_command);
            }
            if (not(yaml_input.empty()))
            {
                H5_Tools::write(observer.filename, "/inputs/yaml/input", yaml_input);
            }

            for (const auto& bodies : sys.get_bodies())
            {
                const auto& states = bodies->get_states();
                const auto& name = states.name;
                const auto& mesh = states.mesh;
                if (mesh->nb_of_static_nodes>0)
                {
                    writeMeshToHdf5File(observer.filename,
                                        "/inputs/meshes/"+name,
                                        mesh->nodes,
                                        mesh->facets);
                }
            }
        }
    }
}

std::string input_data_serialize(const XdynCommandLineArguments& inputData);
std::string input_data_serialize(const XdynCommandLineArguments& inputData)
{
    std::stringstream s;
    s << "xdyn ";
    if (not inputData.yaml_filenames.empty()) s << "-y ";
    for (const auto& f:inputData.yaml_filenames)
    {
        s << f << " ";
    }
    s << " --tstart " << inputData.tstart<<" ";
    s << " --tend " << inputData.tend<<" ";
    s << " --dt " << inputData.initial_timestep<<" ";
    s << " --solver "<<inputData.solver;
    if (not(inputData.output_filename.empty()))
    {
        s << " -o " << inputData.output_filename;
    }
    if (not(inputData.wave_output.empty()))
    {
        s << " -w " << inputData.wave_output;
    }
    return s.str();
}

void run_simulation(const XdynCommandLineArguments& input_data);
void run_simulation(const XdynCommandLineArguments& input_data)
{
    const auto f = [input_data](){
    {
        const auto yaml_input = ssc::text_file_reader::TextFileReader(input_data.yaml_filenames).get_contents();
        ssc::data_source::DataSource command_listener;
        auto sys = get_system(yaml_input, input_data.tstart);
        auto observers_description = build_observers_description(yaml_input, input_data);
        ListOfObservers observers(observers_description);
        serialize_context_if_necessary(observers_description, sys, yaml_input, input_data_serialize(input_data));
        serialize_context_if_necessary_new(observers, sys);
        solve(input_data, sys, observers);
    }};
    if (input_data.catch_exceptions) report_xdyn_exceptions_to_user(f, [](const std::string& s){std::cerr << s;} );
    else                             f();
}



int run(const XdynCommandLineArguments& input_data);
int run(const XdynCommandLineArguments& input_data)
{
    if (not(input_data.empty())) run_simulation(input_data);
    return EXIT_SUCCESS;
}



int main(int argc, char** argv)
{


    XdynCommandLineArguments input_data;
    int error = 0;
    try
    {
        if (argc==1) return fill_input_or_display_help(argv[0], input_data);
        error = parse_command_line_for_xdyn(argc, argv, input_data);
    }
    catch(boost::program_options::error& e)
    {
      std::cerr << "The command line you supplied is not valid: " << e.what() << std::endl << "Use --help to get the list of available parameters." << std::endl;
      return -1;
    }
    if (error)
    {
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
    const auto ret = run(input_data);
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
