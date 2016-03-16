#include "InternalErrorException.hpp"
#include "MeshException.hpp"
#include "NumericalErrorException.hpp"
#include "utilities_for_simulator.hpp"
#include "listeners.hpp"
#include "InputData.hpp"
#include "simulator_api.hpp"
#include "simulator_run.hpp"

#include <ssc/text_file_reader.hpp>
#include <ssc/solver.hpp>
#include <ssc/exception_handling.hpp>

#include <functional>

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

void catch_exceptions(const std::function<void(void)>& f, const InputData& input_data);
void catch_exceptions(const std::function<void(void)>& f, const InputData& input_data)
{
    try
    {
        f();
    }
    catch(const InternalErrorException& e)
    {
        std::cerr << "The following error should never arise & is clearly a sign of a bug in the simulator. Please contact the support team." << std::endl
                  << e.what() << std::endl;
    }
    catch(const MeshException& e)
    {
        std::cerr << "A problem was detected with the STL file (mesh): " << e.get_message() << std::endl;
    }
    catch(const NumericalErrorException& e)
    {
        std::cerr << "The simulation has diverged and cannot continue: " << e.get_message() << std::endl;
        if (input_data.solver=="euler")
        {
            std::cerr << "The simulation used a Euler integration scheme, maybe the simulation can be run with a " << std::endl
                      << "a Runge-Kutta 4 solver (--solver rk4) or a Runge-Kutta-Cash-Karp solver (--solver rkck)"<< std::endl;
        }
    }
    catch(ssc::exception_handling::Exception& e)
    {
        std::cerr << "The following problem was detected:" << std::endl << e.get_message() << std::endl;
    }
    catch(std::exception& e)
    {
        std::cerr << "Something bad has happened: please send an email to the support team containing the following:" << std::endl
                  << "- Input YAML file(s) + STL (if needed)" << std::endl
                  << "- Command-line arguments" << std::endl
                  << "- The following error message: " << e.what() << std::endl;
    }
}

#include "stl_io_hdf5.hpp"
#include "h5_tools.hpp"
void serialize_context_if_necessary(std::vector<YamlOutput>& observers, const Sim& sys, const std::string& yaml_input, const std::string& yaml_command, const std::string& prog_command);
void serialize_context_if_necessary(std::vector<YamlOutput>& observers, const Sim& sys, const std::string& yaml_input, const std::string& yaml_command, const std::string& prog_command)
{
    for (const auto observer:observers)
    {
        if(observer.format=="hdf5")
        {
            if (not(prog_command.empty()))
            {
                H5_Tools::writeString(observer.filename, "/inputs/command", prog_command);
            }
            if (not(yaml_input.empty()))
            {
                H5_Tools::writeString(observer.filename, "/inputs/yaml/input", yaml_input);
            }
            if (not(yaml_command.empty()))
            {
                H5_Tools::writeString(observer.filename, "/inputs/yaml/command", yaml_command);
            }
            for (const auto& bodies : sys.get_bodies())
            {
                writeMeshToHdf5File(observer.filename, "/inputs/meshes/"+bodies->get_states().name,
                                    bodies->get_states().mesh->nodes,
                                    bodies->get_states().mesh->facets);
            }
        }
    }
}

std::string input_data_serialize(const InputData& inputData);
std::string input_data_serialize(const InputData& inputData)
{
    std::stringstream s;
    s<<"sim ";
    if (not inputData.yaml_filenames.empty()) s<<"-y ";
    for (const auto& f:inputData.yaml_filenames)
    {
        s<<f<<" ";
    }
    if (not(inputData.command_file.empty()))
    {
        s<<" -c " << inputData.command_file;
    }
    s<<" --tstart " << inputData.tstart<<" ";
    s<<" --tend " << inputData.tend<<" ";
    s<<" --dt " << inputData.initial_timestep<<" ";
    s<<" --solver "<<inputData.solver;
    if (not(inputData.output_filename.empty()))
    {
        s<<" -o " << inputData.output_filename;
    }
    if (not(inputData.wave_output.empty()))
    {
        s<<" -w " << inputData.wave_output;
    }
    return s.str();
}

void run_simulation(const InputData& input_data)
{
    const auto f = [input_data](){
    {
        const auto yaml_input = ssc::text_file_reader::TextFileReader(input_data.yaml_filenames).get_contents();
        ssc::data_source::DataSource command_listener;
        std::string yaml_command;
        if (not(input_data.command_file.empty()))
        {
            yaml_command = ssc::text_file_reader::TextFileReader(input_data.command_file).get_contents();
            command_listener = listen_to_file(yaml_command);
        }
        auto sys = get_system(yaml_input, input_data.tstart, command_listener);
        auto observers_description = get_observers_description(yaml_input, input_data);
        ListOfObservers observers(observers_description);
        serialize_context_if_necessary(observers_description, sys, yaml_input, yaml_command, input_data_serialize(input_data));
        solve(input_data, sys, observers);
    }};
    if (input_data.catch_exceptions) catch_exceptions(f, input_data);
    else                             f();
}
