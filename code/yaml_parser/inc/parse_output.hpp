/*
 * parse_output.hpp
 *
 *  Created on: Jan 13, 2015
 *      Author: cady
 */

#ifndef PARSE_OUTPUT_HPP_
#define PARSE_OUTPUT_HPP_

#include <string>
#include <vector>

#include "YamlOutput.hpp"

std::vector<YamlOutput> parse_output(const std::string yaml);
YamlOutput generate_default_outputter_with_all_states_in_it(const std::string yaml, const std::string& filename);


#endif /* PARSE_OUTPUT_HPP_ */
