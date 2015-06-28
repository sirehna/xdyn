/*
 * KtKqForceModel.cpp
 *
 *  Created on: Jun 27, 2015
 *      Author: cady
 */
#include <ssc/yaml_parser.hpp>
#include "external_data_structures_parsers.hpp"

#include "KtKqForceModel.hpp"
#include "yaml.h"

KtKqForceModel::Yaml::Yaml() :
            name(),
            position_of_propeller_frame(),
            wake_coefficient(),
            relative_rotative_efficiency(),
            thrust_deduction_factor(),
            rotating_clockwise(),
            diameter(),
            J(),
            Kt(),
            Kq()
{

}

KtKqForceModel::Yaml KtKqForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Yaml ret;
    std::string rot;
    node["rotation"] >> rot;
    ret.rotating_clockwise = (rot == "clockwise");
    node["thrust deduction factor t"]         >> ret.thrust_deduction_factor;
    node["wake coefficient w"]                >> ret.wake_coefficient;
    node["name"]                              >> ret.name;
    node["position of propeller frame"]       >> ret.position_of_propeller_frame;
    node["relative rotative efficiency etaR"] >> ret.relative_rotative_efficiency;
    ssc::yaml_parser::parse_uv(node["diameter"], ret.diameter);
    node["J"]                                 >>ret.J;
    node["Kt"]                                >>ret.Kt;
    node["Kq"]                                >>ret.Kq;

    return ret;
}
