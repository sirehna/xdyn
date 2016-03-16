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
void serialize_context_if_necessary(std::vector<YamlOutput>& observers, const Sim& sys);
void serialize_context_if_necessary(std::vector<YamlOutput>& observers, const Sim& sys)
{
    for (const auto observer:observers)
    {
        if(observer.format=="hdf5")
        {
            for (const auto& bodies : sys.get_bodies())
            {
                writeMeshToHdf5File(observer.filename, "/inputs/meshes/"+bodies->get_states().name,
                                    bodies->get_states().mesh->nodes,
                                    bodies->get_states().mesh->facets);
            }
        }
    }
}

void run_simulation(const InputData& input_data)
{
    const auto f = [input_data](){
    {
        const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
        ssc::data_source::DataSource command_listener;
        if (not(input_data.command_file.empty()))
        {
            command_listener = listen_to_file(ssc::text_file_reader::TextFileReader(input_data.command_file).get_contents());
        }
        const auto yaml = yaml_reader.get_contents();
        auto sys = get_system(yaml,input_data.tstart,command_listener);
        auto observers_description = get_observers_description(yaml, input_data);
        serialize_context_if_necessary(observers_description, sys);
        ListOfObservers observers(observers_description);
        solve(input_data, sys, observers);
    }};
    if (input_data.catch_exceptions) catch_exceptions(f, input_data);
    else                             f();
}
