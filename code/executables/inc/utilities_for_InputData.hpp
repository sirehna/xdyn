/*
 * utilities_for_InputData.hpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#ifndef UTILITIES_FOR_INPUTDATA_HPP_
#define UTILITIES_FOR_INPUTDATA_HPP_

#include <string>
#include <fstream> // std::cout

struct BooleanArguments
{
    bool help;
    bool debug;
};

struct XdynCommandLineArguments;

#include "boost/program_options.hpp"
namespace po = boost::program_options;

void print_usage(std::ostream& os, const po::options_description& desc, const std::string& program_name, const std::string& des);
bool invalid(const XdynCommandLineArguments& input);
BooleanArguments parse_input(int argc, char **argv, const po::options_description& desc);
po::options_description attach_command_line_arguments_to_options_description(XdynCommandLineArguments& input_data);
int parse_command_line_for_xdyn(int argc, char **argv, XdynCommandLineArguments& input_data);
int fill_input_or_display_help(char *argv, XdynCommandLineArguments& input_data);
std::string description(const std::string& des);
void copy_stream(const std::ostream& from_stream, std::ostream& to_stream);

// For Model Exchange
struct InputDataForME;
bool invalid(const InputDataForME& input);
po::options_description get_options_description(InputDataForME& input_data);
int get_input_data(int argc, char **argv, InputDataForME& input_data);
int fill_input_or_display_help(char *argv, InputDataForME& input_data);

#endif /* UTILITIES_FOR_INPUTDATA_HPP_ */
