/*
 * utilities_for_InputData.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#include <iostream>
#include <cstdlib> // EXIT_FAILURE, EXIT_SUCCESS

#include "display_command_line_arguments.hpp"
#include "parse_XdynCommandLineArguments.hpp"
#include "OptionPrinter.hpp"
#include "XdynCommandLineArguments.hpp"



bool invalid(const XdynCommandLineArguments& input)
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

po::options_description attach_command_line_arguments_to_options_description(XdynCommandLineArguments& input_data)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h",                                                                       "Show this help message")
        ("yml,y",      po::value<std::vector<std::string> >(&input_data.yaml_filenames), "Name(s) of the YAML file(s)")
        ("solver,s",   po::value<std::string>(&input_data.solver)->default_value("rk4"), "Name of the solver: euler, rk4, rkck for Euler, Runge-Kutta 4 & Runge-Kutta-Cash-Karp respectively.")
        ("dt",         po::value<double>(&input_data.initial_timestep),                  "Initial time step (or value of the fixed time step for fixed step solvers)")
        ("tstart",     po::value<double>(&input_data.tstart)->default_value(0),          "Date corresponding to the beginning of the simulation (in seconds)")
        ("tend",       po::value<double>(&input_data.tend),                              "Last time step")
        ("output,o",   po::value<std::string>(&input_data.output_filename),              "Name of the output file where all computed data will be exported.\nPossible values/extensions are csv, tsv, json, hdf5, h5, ws")
        ("waves,w",    po::value<std::string>(&input_data.wave_output),                  "Name of the output file where the wave heights will be stored ('output' section of the YAML file). In case output is made to a HDF5 file or web sockets, this option appends the wave height to the main output")
        ("debug,d",                                                                      "Used by the application's support team to help error diagnosis. Allows us to pinpoint the exact location in code where the error occurred (do not catch exceptions), eg. for use in a debugger.")
    ;
    return desc;
}



int parse_command_line_for_xdyn(int argc, char **argv, XdynCommandLineArguments& input_data)
{
    const po::options_description desc = attach_command_line_arguments_to_options_description(input_data);
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

int fill_input_or_display_help(char *argv, XdynCommandLineArguments& input_data)
{
    const po::options_description desc = attach_command_line_arguments_to_options_description(input_data);
    print_usage(std::cout, desc, argv, "This is a ship simulator");
    return EXIT_SUCCESS;
}



