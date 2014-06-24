/*
 * utilities_for_InputData.hpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#ifndef UTILITIES_FOR_INPUTDATA_HPP_
#define UTILITIES_FOR_INPUTDATA_HPP_

#include <iostream> // std::cout

#include "DsObserver.hpp"

#include "tr1_macros.hpp"
#include TR1INC(memory)

class InputData;
#include "boost/program_options.hpp"
namespace po = boost::program_options;

void print_usage(std::ostream& os, const po::options_description& desc);
bool invalid(const InputData& input);
po::options_description get_options_description(InputData& input_data);
int get_input_data(int argc, char **argv, InputData& input_data);
std::string description();
TR1(shared_ptr)<DsObserver> build_observer(const InputData& input);
void copy_stream(const std::ostream& from_stream, std::ostream& to_stream);
void initialize_stream(std::ostream& os, const InputData& input);

#endif /* UTILITIES_FOR_INPUTDATA_HPP_ */
