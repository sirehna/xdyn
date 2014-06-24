/*
 * utilities_for_InputData.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#include <fstream>

#include "utilities_for_InputData.hpp"
#include "InputData.hpp"
#include "OptionPrinter.hpp"
#include "DsCsvObserver.hpp"

std::string description()
{
    std::stringstream ss;    ss << "This is the simulator created during the project 'Bassin Numerique (IRT Jules Verne)'." << std::endl
       << "(c) SIREHNA 2014." << std::endl
       << std::endl
       << "ID : @GIT_SHA1@" << std::endl
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
        ("csv,c",    po::value<std::string>(&input_data.output_csv),                   "Name of the generated CSV file")
        ("solver,s", po::value<std::string>(&input_data.solver),                       "Name of the solver: euler,rk4,rkck for Euler, Runge-Kutta 4 & Runge-Kutta-Cash-Karp respectively.")
        ("dt",       po::value<double>(&input_data.initial_timestep),                  "Initial time step (or value of the fixed time step for fixed step solvers)")
        ("tstart",   po::value<double>(&input_data.tstart),                            "Date corresponding to the beginning of the simulation (in seconds)")
        ("tend",     po::value<double>(&input_data.tend),                              "Last time step")
    ;
    return desc;
}

int get_input_data(int argc, char **argv, InputData& input_data)
{
    const po::options_description desc = get_options_description(input_data);
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
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
    rad::OptionPrinter::printStandardAppDesc("sim",
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

void initialize_stream(std::ostream& os, const InputData& input)
{
    if (input.output_csv.empty()) copy_stream(std::cout, os);
    else copy_stream(std::ofstream(input.output_csv.c_str()), os);
}

TR1(shared_ptr)<DsObserver> build_observer(const InputData& input)
{
    std::ofstream os;
    initialize_stream(os, input);
    return TR1(shared_ptr)<DsObserver>(new DsCsvObserver(os));
}
