#include "simulator_run.hpp"
#include "ConnexionError.hpp"
#include "InternalErrorException.hpp"
#include "MeshException.hpp"
#include "NumericalErrorException.hpp"
#include "utilities_for_simulator.hpp"
#include "listeners.hpp"
#include "InputData.hpp"
#include "simulator_api.hpp"
#include "SurfaceElevationInterface.hpp"
#include "ConnexionError.hpp"

#include <ssc/text_file_reader.hpp>
#include <ssc/solver.hpp>
#include <ssc/exception_handling.hpp>

#include "yaml-cpp/exceptions.h"

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

void catch_exceptions(const std::function<void(void)>& f, const std::string& solver)
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
        if (solver=="euler")
        {
            std::cerr << "The simulation used a Euler integration scheme, maybe the simulation can be run with" << std::endl
                      << "a Runge-Kutta 4 solver (--solver rk4) or a Runge-Kutta-Cash-Karp solver (--solver rkck)"<< std::endl;
        }
    }
    catch(const ConnexionError& e)
    {
        std::cerr << "This simulation requires X-DYN to connect to a server but there was a problem with that connection: " << e.get_message() << std::endl;
    }
    catch(ssc::exception_handling::Exception& e)
    {
        std::cerr << "The following problem was detected:" << std::endl << e.get_message() << std::endl;
    }
    catch(const YAML::Exception& e)
    {
        std::cerr << "There is a syntax problem with the YAML file: couldn't parse it properly." << std::endl
                  << "Line " << e.mark.line+1 << ", column " << e.mark.column+1 << ": " << e.msg << "." << std::endl
                  << "Please note that as all YAML files supplied on the command-line are concatenated, the line number given here corresponds to the line number in the concatenated YAML." << std::endl;
    }
    catch(std::exception& e)
    {
        std::cerr << "Something bad has happened: please send an email to the support team containing the following:" << std::endl
                  << "- Input YAML file(s) + STL (if needed)" << std::endl
                  << "- Command-line arguments" << std::endl
                  << "- The following error message: " << e.what() << std::endl;
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

std::string input_data_serialize(const InputData& inputData);
std::string input_data_serialize(const InputData& inputData)
{
    std::stringstream s;
    s << "sim ";
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

void run_simulation(const InputData& input_data)
{
    const auto f = [input_data](){
    {
        const auto yaml_input = ssc::text_file_reader::TextFileReader(input_data.yaml_filenames).get_contents();
        ssc::data_source::DataSource command_listener;
        auto sys = get_system(yaml_input, input_data.tstart);
        auto observers_description = get_observers_description(yaml_input, input_data);
        ListOfObservers observers(observers_description);
        serialize_context_if_necessary(observers_description, sys, yaml_input, input_data_serialize(input_data));
        serialize_context_if_necessary_new(observers, sys);
        solve(input_data, sys, observers);
    }};
    if (input_data.catch_exceptions) catch_exceptions(f, input_data.solver);
    else                             f();
}
