/*
 * make_sim_for_GZ.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#include "check_input_yaml.hpp"
#include "make_sim_for_GZ.hpp"
#include "simulator_api.hpp"
#include "SimulatorYamlParser.hpp"

Sim GZ::make_sim(const std::string& yaml, const std::string& stl)
{
    ssc::data_source::DataSource command_listener;
    auto input = SimulatorYamlParser(yaml).parse();
    YamlBody body = input.bodies.front();
    body.controlled_forces.clear();
    body.external_forces.clear();
    YamlModel gravity, hydrostatic;
    gravity.model = "gravity";
    hydrostatic.model = "non-linear hydrostatic (exact)";
    body.external_forces.push_back(gravity);
    body.external_forces.push_back(hydrostatic);
    input.bodies[0] = body;
    return get_system(check_input_yaml(input), stl, 0, command_listener);
}
