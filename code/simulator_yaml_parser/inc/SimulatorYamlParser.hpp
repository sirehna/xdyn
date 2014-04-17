/*
 * SimulatorYamlParser.hpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#ifndef SIMULATORYAMLPARSER_HPP_
#define SIMULATORYAMLPARSER_HPP_

#include <memory>

#include "YamlSimulatorInput.hpp"
#include "YamlParser.hpp"

class SimulatorYamlParser : public YamlParser
{
    public:
        SimulatorYamlParser(const std::string& data);
        YamlSimulatorInput parse();

    private:
        std::auto_ptr<YAML::Node> node;
};

#endif /* SIMULATORYAMLPARSER_HPP_ */
