/*
 * utilities_for_InputData.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#include <fstream>
#include <iostream>
#include <cstdlib> // EXIT_FAILURE, EXIT_SUCCESS

#include <ssc/check_ssc_version.hpp>

#include "utilities_for_InputData.hpp"
#include "InputData.hpp"
#include "OptionPrinter.hpp"

std::string description()
{
    std::stringstream ss;
    ss << "This is the simulator created during the project 'Bassin Numerique (IRT Jules Verne)'." << std::endl
       << "(c) SIREHNA 2014." << std::endl
       << std::endl
       << "ID: @GIT_SHA1@" << std::endl
       << "SHA of the SSC used: " << LONG_SSC_GIT_SHA << std::endl
       << std::endl;
    return ss.str();
}

bool invalid(const InputData& input)
{
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

po::options_description get_options_description(InputData& input_data)
{
    po::options_description desc("Options");
    desc.add_options()
        ("help,h",                                                                     "Show this help message")
        ("yml,y",    po::value<std::vector<std::string> >(&input_data.yaml_filenames), "Name(s) of the YAML file(s)")
        ("out,o",    po::value<std::string>(&input_data.output_csv),                   "Name of the generated CSV file. If none given, simulator writes CSV to standard output")
        ("solver,s", po::value<std::string>(&input_data.solver)->default_value("rk4"), "Name of the solver: euler,rk4,rkck for Euler, Runge-Kutta 4 & Runge-Kutta-Cash-Karp respectively.")
        ("dt",       po::value<double>(&input_data.initial_timestep),                  "Initial time step (or value of the fixed time step for fixed step solvers)")
        ("tstart",   po::value<double>(&input_data.tstart)->default_value(0),          "Date corresponding to the beginning of the simulation (in seconds)")
        ("tend",     po::value<double>(&input_data.tend),                              "Last time step")
        ("waves,w",  po::value<std::string>(&input_data.wave_output),                  "Name of the YAML output file where the wave heights will be stored ('output' section of the YAML file)")
    ;
    return desc;
}

int get_input_data(int argc, char **argv, InputData& input_data)
{
    const po::options_description desc = get_options_description(input_data);
    po::positional_options_description p;
    p.add("yml", -1);
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc)
                                                 .positional(p)
                                                 .run(), vm);
    po::notify(vm);
    input_data.help = vm.count("help");
    if (invalid(input_data) || input_data.help)
    {
        print_usage(std::cout, desc);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void print_usage(std::ostream& os, const po::options_description& desc)
{
    po::positional_options_description positionalOptions;
    os << description() << std::endl;
    rad::OptionPrinter::printStandardAppDesc("sim <yaml file>",
                                             os,
                                             desc,
                                             &positionalOptions);
    os << desc << std::endl
       << std::endl;
}

void copy_stream(const std::ostream& from_stream, std::ostream& to_stream)
{
    to_stream.copyfmt(from_stream);                                  //1
    to_stream.clear(from_stream.rdstate());                          //2
    to_stream.basic_ios<char>::rdbuf(from_stream.rdbuf());           //3
}

void initialize_stream(std::ofstream& os, const InputData& input)
{
    if (input.output_csv.empty()) copy_stream(std::cout, os);
}

