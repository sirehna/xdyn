/*
 * check_input_yaml.hpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#ifndef CHECK_INPUT_YAML_HPP_
#define CHECK_INPUT_YAML_HPP_

#include "YamlSimulatorInput.hpp"

#include <ssc/exception_handling.hpp>

class YamlCheckerException: public ::ssc::exception_handling::Exception
{
    public:
        YamlCheckerException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

YamlSimulatorInput check_input_yaml(const YamlSimulatorInput& input);

#endif /* CHECK_INPUT_YAML_HPP_ */
