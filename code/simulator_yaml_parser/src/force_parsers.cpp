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

GravityParameters parse_gravity(const std::string& yaml)
{
    GravityParameters ret;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    parse_uv(node["g"], ret.g);
    return ret;
}

double parse_hydrostatic(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    double rho = 0;
    parse_uv(node["rho"], rho);
    return rho;
}

Eigen::Matrix<double,6,6> parse_quadratic_damping(const std::string& yaml)
{
    Eigen::Matrix<double,6,6> ret;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlDynamics6x6Matrix M;
    node["damping matrix at the center of gravity projected in the body frame"] >> M;
    for (size_t j = 0 ; j < 6 ; ++j) ret(0,j) = M.row_1[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret(1,j) = M.row_2[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret(2,j) = M.row_3[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret(3,j) = M.row_4[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret(4,j) = M.row_5[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret(5,j) = M.row_6[j];
    return ret;
}

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
    node["relative rotative efficiency eta"] >> ret.relative_rotative_efficiency;
    return ret;
}
