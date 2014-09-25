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

class YamlCheckerException: public Exception
{
    public:
        YamlCheckerException(const char* s) :
                Exception(s)
        {
        }
};

YamlSimulatorInput check_input_yaml(const YamlSimulatorInput& input);

#endif /* CHECK_INPUT_YAML_HPP_ */
