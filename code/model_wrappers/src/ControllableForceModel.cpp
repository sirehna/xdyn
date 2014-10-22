/*
 * ControllableForceModel.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include <ssc/data_source.hpp>

#include "ControllableForceModel.hpp"
#include "ControllableForceModelException.hpp"

ControllableForceModel::ControllableForceModel(const std::string& name_, const std::vector<std::string>& commands_) : name(name_), commands(commands_)
{
}

ControllableForceModel::~ControllableForceModel()
{
}

std::map<std::string,double> ControllableForceModel::get_commands(ssc::data_source::DataSource& command_listener) const
{
    std::map<std::string,double> ret;
    for (auto that_command = commands.begin() ; that_command != commands.end() ; ++that_command)
    {
        ret[*that_command] = get_command(*that_command, command_listener);
    }
    return ret;
}

ssc::kinematics::Wrench ControllableForceModel::operator()(const Body& body, const double t, ssc::data_source::DataSource& command_listener) const
{
    return get_force(body, t, get_commands(command_listener));
}

double ControllableForceModel::get_command(const std::string& command_name, ssc::data_source::DataSource& command_listener) const
{
    double ret = 0;
    try
    {
        ret = command_listener.get<double>(name + "(" + command_name + ")");
    }
    catch (const ssc::data_source::DataSourceException& e)
    {
        const std::string msg = std::string("Unable to retrieve command '") + command_name + "' for '" + name + "'\n" + e.what();
        THROW(__PRETTY_FUNCTION__, ControllableForceModelException, msg);
    }
    return ret;
}
