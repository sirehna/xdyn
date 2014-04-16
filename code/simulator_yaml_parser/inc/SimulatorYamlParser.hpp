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
#include "YamlModel.hpp"

class SimulatorYamlParser : public YamlParser
{
    public:
        SimulatorYamlParser(const std::string& data);
        YamlRotation get_rotation();
        YamlModel parse_model(const std::string& yaml) const;
        std::vector<YamlModel> get_environement();

    private:
        std::auto_ptr<YAML::Node> node;
};

#endif /* SIMULATORYAMLPARSER_HPP_ */
