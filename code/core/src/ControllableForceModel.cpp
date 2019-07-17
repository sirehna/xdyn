/*
 * ControllableForceModel.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "ControllableForceModel.hpp"
#include "InvalidInputException.hpp"
#include "ForceModel.hpp"
#include "Observer.hpp"
#include "yaml2eigen.hpp"

#include <ssc/data_source.hpp>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

ControllableForceModel::ControllableForceModel(const std::string& name_, const std::vector<std::string>& commands_, const YamlPosition& internal_frame, const std::string& body_name_, const EnvironmentAndFrames& env_) :
    env(env_),
    commands(commands_),
    name(name_),
    body_name(body_name_),
    position_of_frame(internal_frame),
    latest_force_in_body_frame(),
    from_internal_frame_to_a_known_frame(make_transform(position_of_frame, name, env.rot))
{
    env.k->add(from_internal_frame_to_a_known_frame);
}

ControllableForceModel::~ControllableForceModel()
{
}

std::string ControllableForceModel::get_name() const
{
    return name;
}

std::map<std::string,double> ControllableForceModel::get_commands(ssc::data_source::DataSource& command_listener, const double t) const
{
    std::map<std::string,double> ret;
    for (auto that_command = commands.begin() ; that_command != commands.end() ; ++that_command)
    {
        ret[*that_command] = get_command(*that_command, command_listener, t);
    }
    auto m = command_listener.get_all<double>();
    ret.insert(m.begin(),m.end());
    return ret;
}

ssc::kinematics::Wrench ControllableForceModel::operator()(const BodyStates& states, const double t, ssc::data_source::DataSource& command_listener, const ssc::kinematics::KinematicsPtr& k, const ssc::kinematics::Point& G)
{
    const auto F = get_force(states,t,get_commands(command_listener,t));
    const Eigen::Vector3d force(F(0),F(1),F(2));
    const Eigen::Vector3d torque(F(3),F(4),F(5));
    const auto tau_in_internal_frame = ssc::kinematics::UnsafeWrench(ssc::kinematics::Point(name, 0, 0, 0), force, torque);
    ssc::kinematics::Transform T = k->get(body_name, name);

    // Origin of the internal frame is P
    // G is the point (not the origin) of the body frame where the forces are summed
    // Ob is the origin of the body frame


    const auto rot_from_internal_frame_to_body_frame = T.get_rot();
    const auto OP = T.get_point().v;
    const auto GO = -G.v;
    const auto GP = GO + OP;
    const auto force_in_G_expressed_in_body_frame = rot_from_internal_frame_to_body_frame*force;
    const auto torque_in_G_expressed_in_body_frame = rot_from_internal_frame_to_body_frame*(torque+GP.cross(force));

    const ssc::kinematics::UnsafeWrench tau_in_body_frame_at_G(states.G, force_in_G_expressed_in_body_frame, torque_in_G_expressed_in_body_frame);
    latest_force_in_body_frame = tau_in_body_frame_at_G;

    return tau_in_body_frame_at_G;
}

double ControllableForceModel::get_command(const std::string& command_name, ssc::data_source::DataSource& command_listener, const double t) const
{
    double ret = 0;
    try
    {
        command_listener.check_in(__PRETTY_FUNCTION__);
        command_listener.set("t", t);
        ret = command_listener.get<double>(name + "(" + command_name + ")");
        command_listener.check_out();
    }
    catch (const ssc::data_source::DataSourceException& e)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
                "Unable to retrieve command '" << command_name << "' for '" << name << "': " << e.get_message()
                << " Check that the YAML file containing the commands was supplied to the simulator & that the command exists in that file."
                );
    }
    return ret;
}

void ControllableForceModel::feed(Observer& observer, ssc::kinematics::KinematicsPtr& k, const ssc::kinematics::Point& G) const
{
    // G is the point in which 'latest_force_in_body_frame' is expressed (sum of forces)
    // O is the origin of the NED frame
    // O1 is the origin of the body frame (current ship position)
    // P is the origin of the ControllableForceModel's internal frame

    const auto Tbody_to_internal = k->get(body_name, name);
    const auto rot_from_body_frame_to_internal_frame = Tbody_to_internal.get_rot().transpose();
    const auto Tbody_to_ned = k->get(body_name, "NED");
    const auto rot_from_body_frame_to_ned = Tbody_to_ned.get_rot().transpose();

    const auto tau_in_body_frame_at_G = latest_force_in_body_frame;
    const auto O1P = Tbody_to_internal.get_point().v;
    const auto O1G = G.v;
    const auto OO1 = -Tbody_to_ned.get_point().v;

    const auto PG = O1G-O1P;
    const auto force_in_internal_frame_at_P = rot_from_body_frame_to_internal_frame*tau_in_body_frame_at_G.force;
    const auto torque_in_internal_frame_at_P = rot_from_body_frame_to_internal_frame*(tau_in_body_frame_at_G.torque+PG.cross(tau_in_body_frame_at_G.force));

    const auto OG = OO1+O1G;
    const auto force_in_ned_frame_at_O = rot_from_body_frame_to_ned*tau_in_body_frame_at_G.force;
    const auto torque_in_ned_frame_at_O = rot_from_body_frame_to_ned*(tau_in_body_frame_at_G.torque+OG.cross(tau_in_body_frame_at_G.force));;

    observer.write(tau_in_body_frame_at_G.X(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,name,"Fx"},std::string("Fx(")+name+","+body_name+","+body_name+")"));
    observer.write(tau_in_body_frame_at_G.Y(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,name,"Fy"},std::string("Fy(")+name+","+body_name+","+body_name+")"));
    observer.write(tau_in_body_frame_at_G.Z(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,name,"Fz"},std::string("Fz(")+name+","+body_name+","+body_name+")"));
    observer.write(tau_in_body_frame_at_G.K(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,name,"Mx"},std::string("Mx(")+name+","+body_name+","+body_name+")"));
    observer.write(tau_in_body_frame_at_G.M(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,name,"My"},std::string("My(")+name+","+body_name+","+body_name+")"));
    observer.write(tau_in_body_frame_at_G.N(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,name,"Mz"},std::string("Mz(")+name+","+body_name+","+body_name+")"));

    observer.write((double)force_in_internal_frame_at_P(0),DataAddressing(std::vector<std::string>{"efforts",body_name,name,body_name,"Fx"},std::string("Fx(")+name+","+body_name+","+name+")"));
    observer.write((double)force_in_internal_frame_at_P(1),DataAddressing(std::vector<std::string>{"efforts",body_name,name,body_name,"Fy"},std::string("Fy(")+name+","+body_name+","+name+")"));
    observer.write((double)force_in_internal_frame_at_P(2),DataAddressing(std::vector<std::string>{"efforts",body_name,name,body_name,"Fz"},std::string("Fz(")+name+","+body_name+","+name+")"));
    observer.write((double)torque_in_internal_frame_at_P(0),DataAddressing(std::vector<std::string>{"efforts",body_name,name,body_name,"Mx"},std::string("Mx(")+name+","+body_name+","+name+")"));
    observer.write((double)torque_in_internal_frame_at_P(1),DataAddressing(std::vector<std::string>{"efforts",body_name,name,body_name,"My"},std::string("My(")+name+","+body_name+","+name+")"));
    observer.write((double)torque_in_internal_frame_at_P(2),DataAddressing(std::vector<std::string>{"efforts",body_name,name,body_name,"Mz"},std::string("Mz(")+name+","+body_name+","+name+")"));

    observer.write((double)force_in_ned_frame_at_O(0),DataAddressing(std::vector<std::string>{"efforts",body_name,name,"NED","Fx"},std::string("Fx(")+name+","+body_name+",NED)"));
    observer.write((double)force_in_ned_frame_at_O(1),DataAddressing(std::vector<std::string>{"efforts",body_name,name,"NED","Fy"},std::string("Fy(")+name+","+body_name+",NED)"));
    observer.write((double)force_in_ned_frame_at_O(2),DataAddressing(std::vector<std::string>{"efforts",body_name,name,"NED","Fz"},std::string("Fz(")+name+","+body_name+",NED)"));
    observer.write((double)torque_in_ned_frame_at_O(0),DataAddressing(std::vector<std::string>{"efforts",body_name,name,"NED","Mx"},std::string("Mx(")+name+","+body_name+",NED)"));
    observer.write((double)torque_in_ned_frame_at_O(1),DataAddressing(std::vector<std::string>{"efforts",body_name,name,"NED","My"},std::string("My(")+name+","+body_name+",NED)"));
    observer.write((double)torque_in_ned_frame_at_O(2),DataAddressing(std::vector<std::string>{"efforts",body_name,name,"NED","Mz"},std::string("Mz(")+name+","+body_name+",NED)"));
    extra_observations(observer);
}

double ControllableForceModel::get_Tmax() const
{
    return 0.;
}

std::string ControllableForceModel::get_body_name() const
{
    return body_name;
}

void ControllableForceModel::extra_observations(Observer&) const
{
}
