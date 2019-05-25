/*
 * ConstantForce.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: cady
 */


#include "ConstantForceModel.hpp"

#include "Body.hpp"
#include "yaml.h"
#include <ssc/yaml_parser.hpp>

#include <ssc/kinematics.hpp>

std::string ConstantForceModel::model_name() {return "constant force";}

ConstantForceModel::Input::Input() :
        frame(),
        x(),
        y(),
        z(),
        X(),
        Y(),
        Z(),
        K(),
        M(),
        N()
{
}

ConstantForceModel::Input ConstantForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    ConstantForceModel::Input ret;
    node["frame"] >> ret.frame;
    ssc::yaml_parser::parse_uv(node["x"], ret.x);
    ssc::yaml_parser::parse_uv(node["y"], ret.y);
    ssc::yaml_parser::parse_uv(node["z"], ret.z);
    ssc::yaml_parser::parse_uv(node["X"], ret.X);
    ssc::yaml_parser::parse_uv(node["Y"], ret.Y);
    ssc::yaml_parser::parse_uv(node["Z"], ret.Z);
    ssc::yaml_parser::parse_uv(node["K"], ret.K);
    ssc::yaml_parser::parse_uv(node["M"], ret.M);
    ssc::yaml_parser::parse_uv(node["N"], ret.N);
    return ret;
}

ConstantForceModel::ConstantForceModel(
    const ConstantForceModel::Input& input,
    const std::string& body_name_,
    const EnvironmentAndFrames& env_) :
        ForceModel(model_name(), body_name_),
        env(env_),
        internal_frame(input.frame),
        force(),
        torque(),
        point_of_application_in_internal_frame()

{
    force << input.X
           , input.Y
           , input.Z;
    torque << input.K
            , input.M
            , input.N;
    point_of_application_in_internal_frame << input.x
                                            , input.y
                                            , input.z;
}

ssc::kinematics::Wrench ConstantForceModel::operator()(const BodyStates& states, const double ) const
{
    const ssc::kinematics::Transform T_body_to_internal = env.k->get(states.name, internal_frame);
    // P is the point of application of the force in the internal frame
    // G is the point (not the origin) of the body frame where the forces are summed
    // O is the origin of the body frame
    const auto rot_from_internal_frame_to_body_frame = T_body_to_internal.get_rot();
    const auto OP = point_of_application_in_internal_frame;
    const auto GO = -states.G.v;
    const auto GP = GO + OP;
    const auto force_in_G_expressed_in_body_frame = rot_from_internal_frame_to_body_frame*force;
    const auto torque_in_G_expressed_in_body_frame = rot_from_internal_frame_to_body_frame*(torque+GP.cross(force));
    const ssc::kinematics::Wrench tau_in_body_frame_at_G(states.G, force_in_G_expressed_in_body_frame, torque_in_G_expressed_in_body_frame);
    return tau_in_body_frame_at_G;
}
