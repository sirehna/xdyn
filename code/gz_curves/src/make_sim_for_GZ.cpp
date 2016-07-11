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
    auto input = SimulatorYamlParser(yaml).parse();
    YamlBody body = input.bodies.front();
    body.controlled_forces.clear();
    body.external_forces.clear();
    YamlModel gravity, hydrostatic, waves;
    gravity.model = "gravity";
    //hydrostatic.model = "hydrostatic";
//    hydrostatic.model = "non-linear hydrostatic (exact)";
    hydrostatic.model = "non-linear hydrostatic (fast)";
    body.external_forces.push_back(gravity);
    body.external_forces.push_back(hydrostatic);
    input.bodies[0] = body;
    input.environment.clear();
    waves.model = "no waves";
    waves.yaml = "constant sea elevation in NED frame: {value: 0, unit: m}";
    input.environment.push_back(waves);

    return get_system(check_input_yaml(input), stl, 0);
}
