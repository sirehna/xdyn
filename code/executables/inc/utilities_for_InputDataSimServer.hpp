/*
 * utilities_for_InputDataSimServer.hpp
 *
 *  Created on: 25 nov. 2015
 *      Author: sirehna
 */

#ifndef EXECUTABLES_INC_UTILITIES_FOR_INPUTDATASIMSERVER_HPP_
#define EXECUTABLES_INC_UTILITIES_FOR_INPUTDATASIMSERVER_HPP_

#include <string>
#include <fstream> // std::cout

struct BooleanArguments
{
    bool help;
    bool debug;
};

struct InputDataSimServer;
#include "boost/program_options.hpp"
namespace po = boost::program_options;

void print_usage(std::ostream& os, const po::options_description& desc, const std::string& program_name, const std::string& des);
bool invalid(const InputDataSimServer& input);
BooleanArguments parse_input(int argc, char **argv, const po::options_description& desc);
po::options_description get_options_description(InputDataSimServer& input_data);
int get_input_data(int argc, char **argv, InputDataSimServer& input_data);
int display_help(char *argv, InputDataSimServer& input_data);
std::string description(const std::string& des);
void copy_stream(const std::ostream& from_stream, std::ostream& to_stream);



#endif /* EXECUTABLES_INC_UTILITIES_FOR_INPUTDATASIMSERVER_HPP_ */
