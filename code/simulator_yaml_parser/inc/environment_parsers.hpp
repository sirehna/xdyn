/*
 * environment_parsers.hpp
 *
 *  Created on: May 22, 2014
 *      Author: cady
 */

#ifndef ENVIRONMENT_PARSERS_HPP_
#define ENVIRONMENT_PARSERS_HPP_

#include "YamlWaveModelInput.hpp"

#include <string>

double parse_default_wave_model(const std::string& yaml);
YamlWaveModel parse_waves(const std::string& yaml);

#endif  /* ENVIRONMENT_PARSERS_HPP_ */
