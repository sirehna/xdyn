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

struct XdynCommandLineArguments;

#include "boost/program_options.hpp"
namespace po = boost::program_options;

bool invalid(const XdynCommandLineArguments& input);
po::options_description attach_command_line_arguments_to_options_description(XdynCommandLineArguments& input_data);
int parse_command_line_for_xdyn(int argc, char **argv, XdynCommandLineArguments& input_data);
int fill_input_or_display_help(char *argv, XdynCommandLineArguments& input_data);


#endif /* UTILITIES_FOR_INPUTDATA_HPP_ */
