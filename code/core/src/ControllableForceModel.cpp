/*
 * ControllableForceModel.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include <ssc/data_source.hpp>

#include "ControllableForceModel.hpp"
#include "ControllableForceModelException.hpp"
#include "yaml2eigen.hpp"

ControllableForceModel::ControllableForceModel(const std::string& name_, const std::vector<std::string>& commands_, const YamlPosition& position_of_frame_, const EnvironmentAndFrames& env_) :
    env(env_), name(name_), commands(commands_), position_of_frame(position_of_frame_), point_of_application(make_point(position_of_frame.coordinates, name))
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
    return ret;
}

ssc::kinematics::Wrench ControllableForceModel::operator()(const BodyStates& states, const double t, ssc::data_source::DataSource& command_listener) const
{
    return ssc::kinematics::Wrench(point_of_application, get_force(states, t, get_commands(command_listener, t)));
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
