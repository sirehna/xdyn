#include "ConnexionError.hpp"
#include "InternalErrorException.hpp"
#include "MeshException.hpp"
#include "NumericalErrorException.hpp"
#include "listeners.hpp"
#include "simulator_api.hpp"
#include "SurfaceElevationInterface.hpp"
#include "ConnexionError.hpp"

#include <ssc/text_file_reader.hpp>
#include <ssc/solver.hpp>
#include <ssc/exception_handling.hpp>

#include "yaml-cpp/exceptions.h"

#include <functional>

#include "report_xdyn_exceptions_to_user.hpp"
#include "XdynCommandLineArguments.hpp"
#include "build_observers_description.hpp"

void report_xdyn_exceptions_to_user(const std::function<void(void)>& f, const std::string& solver)
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
        std::cerr << "There is a syntax problem with the YAML file line " << e.mark.line+1 << ", column " << e.mark.column+1 << ": " << e.msg << "." << std::endl
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
