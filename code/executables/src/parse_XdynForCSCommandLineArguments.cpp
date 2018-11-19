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
        ("debug,d",                                                                      "Used by the application's support team to help error diagnosis. Allows us to pinpoint the exact location in code where the error occurred (do not catch exceptions), eg. for use in a debugger.")
        ("port,p",     po::value<short unsigned int>(&input_data.port),                  "port for the websocket server")
        ;
    return desc;
}

int get_input_data(int argc, char **argv, XdynForCSCommandLineArguments& input_data)
{
    const po::options_description desc = get_options_description(input_data);
    const BooleanArguments has = parse_input(argc, argv, desc);
    input_data.catch_exceptions = not(has.debug);
    if (has.help)
    {
        print_usage(std::cout, desc, argv[0], "This is a ship simulator");
        return EXIT_SUCCESS;
    }
    else if (invalid(input_data))
    {
        print_usage(std::cout, desc, argv[0], "This is a ship simulator");
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
