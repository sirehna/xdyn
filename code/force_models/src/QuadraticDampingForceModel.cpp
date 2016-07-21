/*
 * QuadraticDampingForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#include <Eigen/Dense>
#include <ssc/kinematics.hpp>
#include "Body.hpp"
#include "QuadraticDampingForceModel.hpp"

#include "yaml.h"
#include "external_data_structures_parsers.hpp"

std::string QuadraticDampingForceModel::model_name() {return "quadratic damping";}

QuadraticDampingForceModel::QuadraticDampingForceModel(const QuadraticDampingForceModel::Input& D_, const std::string& body_name_, const EnvironmentAndFrames&) : DampingForceModel("quadratic damping", body_name_, D_)
{
}

Eigen::Matrix<double, 6, 1> QuadraticDampingForceModel::get_force_and_torque(const Eigen::Matrix<double,6,6>& D, const Eigen::Matrix<double, 6, 1>& W) const
{
    return (Eigen::Matrix<double, 6, 1>)(-D * (Eigen::Matrix<double, 6, 1>)(W.cwiseAbs().array() * W.array()));
}

QuadraticDampingForceModel::Input QuadraticDampingForceModel::parse(const std::string& yaml)
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
