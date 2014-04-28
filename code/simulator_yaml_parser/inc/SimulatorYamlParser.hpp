/*
 * SimulatorYamlParser.hpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#ifndef SIMULATORYAMLPARSER_HPP_
#define SIMULATORYAMLPARSER_HPP_

#include "YamlSimulatorInput.hpp"
#include "YamlParser.hpp"

class SimulatorYamlParser : public YamlParser
{
    public:
        SimulatorYamlParser(const std::string& data);
        YamlSimulatorInput parse() const;
};

#endif /* SIMULATORYAMLPARSER_HPP_ */
