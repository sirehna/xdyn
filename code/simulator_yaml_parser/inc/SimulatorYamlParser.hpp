/*
 * SimulatorYamlParser.hpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#ifndef SIMULATORYAMLPARSER_HPP_
#define SIMULATORYAMLPARSER_HPP_

#include "YamlParser.hpp"
#include "YamlRotation.hpp"

class SimulatorYamlParser : public YamlParser
{
    public:
        SimulatorYamlParser(const std::string& data);
};

#endif /* SIMULATORYAMLPARSER_HPP_ */
