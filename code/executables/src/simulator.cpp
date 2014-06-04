/*
 * simulator.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include "boost/program_options.hpp"
#include "OptionPrinter.hpp"

#include "TextFileReader.hpp"
#include "DataSource.hpp"
#include "SimulatorAPI.hpp"
#include "DsCsvObserver.hpp"
#include "DsSolve.hpp"

#include <iostream>
#include <string>
#include <cstdlib>

namespace po = boost::program_options;

std::string description();
std::string description()
{
    std::stringstream ss;
    ss << "This is the simulator created during the project 'Bassin Numerique (IRT Jules Verne)'." << std::endl
       << "(c) SIREHNA 2014." << std::endl
       << std::endl;
    return ss.str();
}

void print_usage(std::ostream& os, const po::options_description& desc);
void print_usage(std::ostream& os, const po::options_description& desc)
{
    po::positional_options_description positionalOptions;
    os << description() << std::endl;
    rad::OptionPrinter::printStandardAppDesc("eonav_cli",
                                             os,
                                             desc,
                                             &positionalOptions);
    os << desc << std::endl
       << std::endl;
}

struct InputData
{
    InputData() :   yaml_filenames(std::vector<std::string>()),
                    output_csv(std::string()),
                    solver(std::string()),
                    initial_timestep(0),
                    tstart(0),
                    tend(0),
                    help(false)
    {
    }
    std::vector<std::string> yaml_filenames;
    std::string output_csv;
    std::string solver;
    double initial_timestep;
    double tstart;
    double tend;
    bool help;
};

bool invalid(const InputData& input);
bool invalid(const InputData& input)
{
    if (input.output_csv.empty())
    {
        std::cerr << "Error: no output CSV file defined." << std::endl;
        return true;
    }
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

po::options_description get_options_description(InputData& input_data);
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

int get_input_data(int argc, char **argv, InputData& input_data);
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

int main(int argc, char** argv)
{
    InputData input_data;
    const int error = get_input_data(argc, argv, input_data);
    if (not(error))
    {
        const TextFileReader yaml_reader(input_data.yaml_filenames);
        DataSource ds = make_ds(yaml_reader.get_contents());
        DsCsvObserver observer(std::cout);
        integrate(ds, 0, 10, observer);
    }
    return error;
}
