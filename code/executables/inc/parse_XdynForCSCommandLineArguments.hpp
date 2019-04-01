/*
 * parse_XdynForCSCommandLineArguments.hpp
 *
 *  Created on: 25 nov. 2015
 *      Author: sirehna
 */

#ifndef EXECUTABLES_INC_PARSE_XDYNFORCSCOMMANDLINEARGUMENTS_HPP_
#define EXECUTABLES_INC_PARSE_XDYNFORCSCOMMANDLINEARGUMENTS_HPP_

#include <string>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "XdynForCSCommandLineArguments.hpp"

bool invalid(const XdynForCSCommandLineArguments& input);
po::options_description get_options_description(XdynForCSCommandLineArguments& input_data);
int get_input_data(int argc, char **argv, XdynForCSCommandLineArguments& input_data);
int display_help(char *argv, XdynForCSCommandLineArguments& input_data);

#endif /* EXECUTABLES_INC_PARSE_XDYNFORCSCOMMANDLINEARGUMENTS_HPP_ */
