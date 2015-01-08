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

Eigen::Matrix<double,6,6> parse_quadratic_damping(const std::string& yaml)
{
    Eigen::Matrix<double,6,6> ret;
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlDynamics6x6Matrix M;
    node["damping matrix at the center of gravity projected in the body frame"] >> M;
    for (size_t j = 0 ; j < 6 ; ++j) ret(0,(int)j) = M.row_1[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret(1,(int)j) = M.row_2[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret(2,(int)j) = M.row_3[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret(3,(int)j) = M.row_4[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret(4,(int)j) = M.row_5[j];
    for (size_t j = 0 ; j < 6 ; ++j) ret(5,(int)j) = M.row_6[j];
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
    node["relative rotative efficiency etaR"]>> ret.relative_rotative_efficiency;
    parse_uv(node["diameter"], ret.diameter);
    return ret;
}

YamlResistanceCurve parse_resistance_curve(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlResistanceCurve ret;
    parse_uv(node["speed"], ret.Va);
    parse_uv(node["resistance"], ret.R);
    return ret;
}

TypeOfQuadrature parse_type_of_quadrature(const std::string& s)
{
    if      (s == "gauss-kronrod")   return TypeOfQuadrature::GAUSS_KRONROD;
    else if (s == "rectangle")       return TypeOfQuadrature::RECTANGLE;
    else if (s == "simpson")         return TypeOfQuadrature::SIMPSON;
    else if (s == "trapezoidal")     return TypeOfQuadrature::TRAPEZOIDAL;
    else if (s == "burcher")         return TypeOfQuadrature::BURCHER;
    else if (s == "clenshaw-curtis") return TypeOfQuadrature::CLENSHAW_CURTIS;
    else if (s == "filon")           return TypeOfQuadrature::FILON;
    else
    {
        std::stringstream ss;
        ss << "Unkown quadrature type: " << s << ". Should be one of 'gauss-kronrod', 'rectangle', ' simpson', 'trapezoidal', 'burcher', 'clenshaw-curtis' or 'filon'.";
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, ss.str());
    }
}

YamlRadiationDamping parse_radiation_damping(const std::string& yaml)
{
    std::stringstream stream(yaml);
    std::stringstream ss;
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlRadiationDamping ret;
    node["hdb"] >> ret.hdb_filename;
    std::string s;
    node["type of quadrature for cos transform"] >> s;
    ret.type_of_quadrature_for_cos_transform = parse_type_of_quadrature(s);
    node["type of quadrature for convolution"] >> s;
    ret.type_of_quadrature_for_convolution = parse_type_of_quadrature(s);
    node["nb of points for retardation function discretization"] >> ret.nb_of_points_for_retardation_function_discretization;
    parse_uv(node["omega min"], ret.omega_min);
    parse_uv(node["omega max"], ret.omega_max);
    parse_uv(node["tau min"], ret.tau_min);
    parse_uv(node["tau max"], ret.tau_max);
    node["output Br and K"] >> ret.output_Br_and_K;
    node["calculation point in body frame"] >> ret.calculation_point_in_body_frame;
    return ret;
}

YamlDiffraction parse_diffraction(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    YamlDiffraction ret;
    node["hdb"]                             >> ret.hdb_filename;
    node["calculation point in body frame"] >> ret.calculation_point;
    node["mirror for 180 to 360"]           >> ret.mirror;
    return ret;
}
