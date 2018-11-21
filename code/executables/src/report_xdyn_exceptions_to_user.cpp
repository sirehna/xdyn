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
#include <ssc/json/JSONException.hpp>
#include <ssc/websocket.hpp>

#include "yaml-cpp/exceptions.h"

#include <functional>
#include <boost/program_options.hpp>

#include "report_xdyn_exceptions_to_user.hpp"
#include "XdynCommandLineArguments.hpp"
#include "build_observers_description.hpp"

void report_xdyn_exceptions_to_user(const std::function<void(void)>& f, const std::function<void(const std::string&)>& outputter)
{
    std::stringstream ss;
    try
    {
        f();
    }
    catch(const boost::program_options::error& e)
    {
        ss << "The command-line you supplied is not valid:" << std::endl << '\t' << e.what() << std::endl << "Try running the program again with the -h flag to get a list of supported options." << std::endl;
        outputter(ss.str());
    }
    catch(const InvalidInputException& e)
    {
        ss << "The input you provided is invalid & we cannot simulate. " << e.get_message() << std::endl;
        outputter(ss.str());
    }
    catch(const InternalErrorException& e)
    {
        ss << "The following error should never arise & is clearly a sign of a bug in the simulator. Please contact the support team." << std::endl
                  << e.what() << std::endl;
        outputter(ss.str());
    }
    catch(const MeshException& e)
    {
        std::cerr << "A problem was detected with the STL file (mesh): " << e.get_message() << std::endl;
    }
    catch(const NumericalErrorException& e)
    {
        ss << "The simulation has diverged and cannot continue: " << e.get_message() << std::endl;
        ss << "Maybe you can use another solver? For example, if you used a Euler integration scheme, maybe the simulation can be run with" << std::endl
           << "a Runge-Kutta 4 solver (--solver rk4) or a Runge-Kutta-Cash-Karp solver (--solver rkck)"<< std::endl;
        outputter(ss.str());
    }
    catch(const ssc::websocket::WebSocketException& e)
    {
        ss << "Unable to launch the websocket server: maybe the port you specified not available?" << std::endl
           << " You can run sudo lsof -i :9002 to check which process is using the port (eg. for port 9002 in this case)." << std::endl
           << "Try specifying a different port number using the -p flag." << std::endl
           << "The full error message was: " << e.what() << std::endl;
    }
    catch(const ConnexionError& e)
    {
        ss << "This simulation requires X-DYN to connect to a server but there was a problem with that connection: " << e.get_message() << std::endl;
        outputter(ss.str());
    }
    catch(const ssc::json::Exception& e)
    {
        ss << "There is a syntax problem with the supplied JSON: " << e.get_message() << std::endl;
        outputter(ss.str());
    }
    catch(ssc::exception_handling::Exception& e)
    {
        ss << "The following problem was detected:" << std::endl << e.get_message() << std::endl;
        outputter(ss.str());
    }
    catch(const YAML::Exception& e)
    {
        ss << "There is a syntax problem with the YAML file line " << e.mark.line+1 << ", column " << e.mark.column+1 << ": " << e.msg << "." << std::endl
           << "Please note that as all YAML files supplied on the command-line are concatenated, the line number given here corresponds to the line number in the concatenated YAML." << std::endl;
        outputter(ss.str());
    }
    catch(std::exception& e)
    {
        ss << "Something bad has happened: please send an email to the support team containing the following:" << std::endl
           << "- Input YAML file(s) + STL (if needed)" << std::endl
           << "- Command-line arguments" << std::endl
           << "- The following error message: " << e.what() << std::endl;
        outputter(ss.str());
    }
}


#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>


std::string current_date_time()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}
