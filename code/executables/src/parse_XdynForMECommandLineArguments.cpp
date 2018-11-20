/*
 * utilities_for_InputData.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#include <iostream>
#include <cstdlib> // EXIT_FAILURE, EXIT_SUCCESS

#include <ssc/check_ssc_version.hpp>

#include "display_command_line_arguments.hpp"
#include "parse_XdynForMECommandLineArguments.hpp"
#include "XdynForMECommandLineArguments.hpp"


bool invalid(const XdynForMECommandLineArguments& input)
{
    if (input.empty()) return true;
    if (input.yaml_filenames.empty())
    {
        std::cerr << "Error: no input YAML files defined: need at least one." << std::endl;
        return true;
    }
    return false;
}

po::options_description get_options_description(XdynForMECommandLineArguments& input_data)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h",                                                                       "Show this help message")
        ("yml,y", po::value<std::vector<std::string> >(&input_data.yaml_filenames),      "Name(s) of the YAML file(s)")
        ("port,p", po::value<short unsigned int>(&input_data.port),                      "Port number on which to run this websocket server")
        ("debug,d",                                                                      "Used by the application's support team to help error diagnosis. Allows us to pinpoint the exact location in code where the error occurred (do not catch exceptions), eg. for use in a debugger.")
    ;
    return desc;
}

int get_input_data(int argc, char **argv, XdynForMECommandLineArguments& input_data)
{
    const po::options_description desc = get_options_description(input_data);
    const BooleanArguments has = parse_input(argc, argv, desc);
    input_data.catch_exceptions = not(has.debug);
    input_data.show_help = has.help;
    if (has.help)
    {
        print_usage(std::cout, desc, argv[0], "This is a ship simulator (model exchange version)");
        return EXIT_SUCCESS;
    }
    else if (invalid(input_data))
    {
        print_usage(std::cout, desc, argv[0], "\n\nThis is a ship simulator (model exchange version)");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int fill_input_or_display_help(char *argv, XdynForMECommandLineArguments& input_data)
{
    const po::options_description desc = get_options_description(input_data);
    print_usage(std::cout, desc, argv, "This is a ship simulator (model exchange server version)");
    return EXIT_SUCCESS;
}
