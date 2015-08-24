/*
 * ControllableForceModel.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include <ssc/data_source.hpp>

#include "ControllableForceModel.hpp"
#include "ControllableForceModelException.hpp"
#include "ForceModel.hpp"
#include "Observer.hpp"
#include "yaml2eigen.hpp"

ControllableForceModel::ControllableForceModel(const std::string& name_, const std::vector<std::string>& commands_, const YamlPosition& position_of_frame_, const std::string& body_name_, const EnvironmentAndFrames& env_) :
    env(env_),
    commands(commands_),
    name(name_),
    body_name(body_name_),
    position_of_frame(position_of_frame_),
    point_of_application(make_point(position_of_frame.coordinates, position_of_frame.frame)),
    force_in_body_frame(),
    force_in_ned_frame()
{
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

ssc::kinematics::Wrench ControllableForceModel::operator()(const BodyStates& states, const double t, ssc::data_source::DataSource& command_listener) const
{
    const auto F = get_force(states,t,get_commands(command_listener,t));
    const Eigen::Vector3d force(F(0),F(1),F(2));
    const Eigen::Vector3d torque(F(3),F(4),F(5));
    const ssc::kinematics::UnsafeWrench tau_body(states.G, force, torque + (point_of_application-states.G).cross(force));
    return tau_body;
}

ssc::kinematics::Wrench ControllableForceModel::get_force_in_body_frame() const
{
    return force_in_body_frame;
}

void ControllableForceModel::update(const BodyStates& states, const double t, ssc::data_source::DataSource& command_listener)
{
    body_name = states.name;
    force_in_body_frame = this->operator()(states, t, command_listener);
    force_in_ned_frame = ::ForceModel::project_into_NED_frame(force_in_body_frame, states.get_rot_from_ned_to_body());
}

void ControllableForceModel::add_reference_frame(const ::ssc::kinematics::KinematicsPtr& k, const YamlRotation& rotations) const
{
    k->add(make_transform(position_of_frame, name, rotations));
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
        const std::string msg = std::string("Unable to retrieve command '") + command_name + "' for '" + name + "'\n" + e.what();
        THROW(__PRETTY_FUNCTION__, ControllableForceModelException, msg);
    }
    return ret;
}

void ControllableForceModel::feed(Observer& observer) const
{
    observer.write(force_in_body_frame.X(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,body_name,"Fx"},std::string("Fx(")+name+","+body_name+","+body_name+")"));
    observer.write(force_in_body_frame.Y(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,body_name,"Fy"},std::string("Fy(")+name+","+body_name+","+body_name+")"));
    observer.write(force_in_body_frame.Z(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,body_name,"Fz"},std::string("Fz(")+name+","+body_name+","+body_name+")"));
    observer.write(force_in_body_frame.K(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,body_name,"Mx"},std::string("Mx(")+name+","+body_name+","+body_name+")"));
    observer.write(force_in_body_frame.M(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,body_name,"My"},std::string("My(")+name+","+body_name+","+body_name+")"));
    observer.write(force_in_body_frame.N(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,body_name,"Mz"},std::string("Mz(")+name+","+body_name+","+body_name+")"));

    observer.write(force_in_ned_frame.X(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,"NED","Fx"},std::string("Fx(")+name+","+body_name+",NED)"));
    observer.write(force_in_ned_frame.Y(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,"NED","Fy"},std::string("Fy(")+name+","+body_name+",NED)"));
    observer.write(force_in_ned_frame.Z(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,"NED","Fz"},std::string("Fz(")+name+","+body_name+",NED)"));
    observer.write(force_in_ned_frame.K(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,"NED","Mx"},std::string("Mx(")+name+","+body_name+",NED)"));
    observer.write(force_in_ned_frame.M(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,"NED","My"},std::string("My(")+name+","+body_name+",NED)"));
    observer.write(force_in_ned_frame.N(),DataAddressing(std::vector<std::string>{"efforts",body_name,name,"NED","Mz"},std::string("Mz(")+name+","+body_name+",NED)"));
}

double ControllableForceModel::get_Tmax() const
{
    return 0.;
}

std::string ControllableForceModel::get_body_name() const
{
    return body_name;
}
