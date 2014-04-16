/*
 * SimulatorYamlParser.hpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#ifndef SIMULATORYAMLPARSER_HPP_
#define SIMULATORYAMLPARSER_HPP_

#include <memory>

#include "YamlParser.hpp"
#include "YamlRotation.hpp"

class SimulatorYamlParser : public YamlParser
{
    public:
        SimulatorYamlParser(const std::string& data);
        YamlRotation get_rotation();

    private:
        std::auto_ptr<YAML::Node> node;
};

#endif /* SIMULATORYAMLPARSER_HPP_ */
