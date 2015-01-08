/*
 * forces_parsers.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */


#include "force_parsers.hpp"
#include "yaml.h"
#include "parse_unit_value.hpp"
#include "external_data_structures_parsers.hpp"
#include "SimulatorYamlParserException.hpp"

YamlWageningen parse_wageningen(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlWageningen ret;
    std::string rot;
    node["rotation"] >> rot;
    ret.rotating_clockwise = (rot == "clockwise");
    node["thrust deduction factor t"]        >> ret.thrust_deduction_factor;
    node["wake coefficient w"]               >> ret.wake_coefficient;
    node["name"]                             >> ret.name;
    node["blade area ratio AE/A0"]           >> ret.blade_area_ratio;
    node["number of blades"]                 >> ret.number_of_blades;
    node["position of propeller frame"]      >> ret.position_of_propeller_frame;
    node["relative rotative efficiency etaR"]>> ret.relative_rotative_efficiency;
    parse_uv(node["diameter"], ret.diameter);
    return ret;
}
