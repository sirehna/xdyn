#include <iostream>
#include <cstdlib> // EXIT_FAILURE, EXIT_SUCCESS

#include <ssc/check_ssc_version.hpp>

#include "display_command_line_arguments.hpp"
#include "parse_XdynForCSCommandLineArguments.hpp"
#include "OptionPrinter.hpp"

bool invalid(const XdynForCSCommandLineArguments& input)
{
    if (input.empty()) return true;
    if (input.yaml_filenames.empty())
    {
        std::cerr << "Error: no input YAML files defined: need at least one." << std::endl;
        return true;
    }
    if (input.solver.empty())
    {
        std::cerr << "Error: no solver defined." << std::endl;
        return true;
    }
    if (input.initial_timestep<=0)
    {
        std::cerr << "Error: initial time step is negative or zero." << std::endl;
        return true;
    }
    if (input.port==0)
    {
        std::cerr << "Error: no port number was defined (or port 0 was defined, which is equally invalid)." << std::endl
                 << "You can define one using the -p flag. Check the available port range using the -h flag." << std::endl;
        return true;
    }
    if (input.port < 1024) // input.port is always less than 65536 because it is a short unsigned int.
    {
        std::cerr << "Error: you cannot start this websocket server on port " << input.port << ": only range 1024-65535 is available." << std::endl;
        return true;
    }
    return false;
}

po::options_description get_options_description(XdynForCSCommandLineArguments& input_data)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h",                                                                       "Show this help message")
        ("yml,y",      po::value<std::vector<std::string> >(&input_data.yaml_filenames), "Name(s) of the YAML file(s)")
        ("solver,s",   po::value<std::string>(&input_data.solver)->default_value("rk4"), "Name of the solver: euler,rk4,rkck for Euler, Runge-Kutta 4 & Runge-Kutta-Cash-Karp respectively.")
        ("dt",         po::value<double>(&input_data.initial_timestep),                  "Initial time step (or value of the fixed time step for fixed step solvers)")
        ("verbose,v",                                                                    "Display all information received & emitted by the server on the standard output.")
        ("websocket-debug,w",                                                            "Display *all* websocket-related information (connect/disconnect, payload, etc.): very chatty.")
        ("debug,d",                                                                      "Used by the application's support team to help error diagnosis. Allows us to pinpoint the exact location in code where the error occurred (do not catch exceptions), eg. for use in a debugger.")
        ("port,p",     po::value<short unsigned int>(&input_data.port),                  "port for the websocket server. Available values are 1024-65535 (2^16, but port 0 is reserved and unavailable and ports in range 1-1023 are privileged (application needs to be run as root to have access to those ports)")
        ("grpc,g",                                                                       "Launch a gRPC server instead of the (default) JSON+websocket server.")
        ;
    return desc;
}

int get_input_data(int argc, char **argv, XdynForCSCommandLineArguments& input_data)
{
    const po::options_description desc = get_options_description(input_data);
    const BooleanArguments has = parse_input(argc, argv, desc);
    input_data.catch_exceptions = not(has.debug);
    input_data.verbose = has.verbose;
    input_data.show_help = has.help;
    input_data.show_websocket_debug_information = has.show_websocket_debug_information;
    if (has.help)
    {
        print_usage(std::cout, desc, argv[0], "This is a ship simulator (co-simulation server version)");
        return EXIT_SUCCESS;
    }
    else if (invalid(input_data))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int display_help(char *argv, XdynForCSCommandLineArguments& input_data)
{
    const po::options_description desc = get_options_description(input_data);
    print_usage(std::cout, desc, argv, "This is a ship simulator");
    return EXIT_SUCCESS;
}
