/*
 * SimulatorYamlParser.hpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#ifndef SIMULATORYAMLPARSER_HPP_
#define SIMULATORYAMLPARSER_HPP_

#include "YamlSimulatorInput.hpp"

#include "Exception.hpp"

class YamlCheckerException: public Exception
{
    public:
        YamlCheckerException(const char* s) :
                Exception(s)
        {
        }
};

YamlSimulatorInput check_input_yaml(const YamlSimulatorInput& input);

#endif /* SIMULATORYAMLPARSER_HPP_ */
