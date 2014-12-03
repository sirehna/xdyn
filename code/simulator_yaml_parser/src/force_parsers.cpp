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

YamlGravity parse_gravity(const std::string& yaml)
{
    YamlGravity ret;
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
    node["interpolation"] >> s;
    if      (s == "splines")            ret.interpolation = TypeOfInterpolation::SPLINES;
    else if (s == "piecewise constant") ret.interpolation = TypeOfInterpolation::PIECEWISE_CONSTANT;
    else if (s == "linear")             ret.interpolation = TypeOfInterpolation::LINEAR;
    else
    {
        ss << "Unkown interpolation type: " << s << ". Should be one of 'splines', 'piecewise constant' or 'linear'.";
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, ss.str());
    }
    node["nb of points in retardation function"] >> ret.nb_of_points_in_retardation_function;
    node["quadrature"] >> s;
    if      (s == "gauss-kronrod") ret.quadrature = TypeOfQuadrature::GAUSS_KRONROD;
    else if (s == "rectangle")     ret.quadrature = TypeOfQuadrature::RECTANGLE;
    else if (s == "simpson")       ret.quadrature = TypeOfQuadrature::SIMPSON;
    else if (s == "trapezoidal")   ret.quadrature = TypeOfQuadrature::TRAPEZOIDAL;
    else
    {
        ss.clear();
        ss << "Unkown quadrature type: " << s << ". Should be one of 'gauss-kronrod', 'rectangle', ' simpson' or 'trapezoidal'.";
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, ss.str());
    }
    node["quadrature tolerance"] >> ret.quadrature_tolerance;
    if ((ret.quadrature_tolerance>1) or (ret.quadrature_tolerance<0))
    {
        ss.clear();
        ss << "Invalid quadrature tolerance: " << s << ". Should be between 0 and 1.";
        THROW(__PRETTY_FUNCTION__, SimulatorYamlParserException, ss.str());
    }
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
