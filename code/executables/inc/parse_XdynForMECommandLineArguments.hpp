/*
 * utilities_for_InputData.hpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#ifndef PARSE_XDYN_FOR_ME_COMMAND_LINE_ARGUMENTSHPP
#define PARSE_XDYN_FOR_ME_COMMAND_LINE_ARGUMENTSHPP

#include <string>

struct XdynForMECommandLineArguments;

#include "boost/program_options.hpp"
namespace po = boost::program_options;

bool invalid(const XdynForMECommandLineArguments& input);
po::options_description get_options_description(XdynForMECommandLineArguments& input_data);
int get_input_data(int argc, char **argv, XdynForMECommandLineArguments& input_data);
int fill_input_or_display_help(char *argv, XdynForMECommandLineArguments& input_data);

#endif /* PARSE_XDYN_FOR_ME_COMMAND_LINE_ARGUMENTSHPP */
