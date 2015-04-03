/*
 * utilities_for_InputData.hpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#ifndef UTILITIES_FOR_INPUTDATA_HPP_
#define UTILITIES_FOR_INPUTDATA_HPP_

#include <fstream> // std::cout

#include <ssc/macros.hpp>
#include TR1INC(memory)

struct InputData;
#include "boost/program_options.hpp"
namespace po = boost::program_options;

void print_usage(std::ostream& os, const po::options_description& desc, const std::string& program_name, const std::string& des);
bool invalid(const InputData& input);
bool parse_input(int argc, char **argv, const po::options_description& desc);
po::options_description get_options_description(InputData& input_data);
int get_input_data(int argc, char **argv, InputData& input_data);
std::string description(const std::string& des);
void copy_stream(const std::ostream& from_stream, std::ostream& to_stream);

#endif /* UTILITIES_FOR_INPUTDATA_HPP_ */
