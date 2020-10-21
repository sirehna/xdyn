/*
 * display_command_line_arguments.hpp
 *
 *  Created on: Nov 14, 2018
 *      Author: cady
 */

#ifndef EXECUTABLES_INC_DISPLAY_COMMAND_LINE_ARGUMENTS_HPP_
#define EXECUTABLES_INC_DISPLAY_COMMAND_LINE_ARGUMENTS_HPP_

#include <fstream> // std::cout

struct BooleanArguments
{
    bool help;
    bool debug;
    bool verbose;
    bool show_help;
    bool show_websocket_debug_information;
    bool grpc;
};

#include "boost/program_options.hpp"
namespace po = boost::program_options;

std::string description(const std::string& des);
void print_usage(std::ostream& os, const po::options_description& desc, const std::string& program_name, const std::string& des);
BooleanArguments parse_input(int argc, char **argv, const po::options_description& desc);
void copy_stream(const std::ostream& from_stream, std::ostream& to_stream);

#endif /* EXECUTABLES_INC_DISPLAY_COMMAND_LINE_ARGUMENTS_HPP_ */
