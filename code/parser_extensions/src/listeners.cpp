/*
 * listeners.cpp
 *
 *  Created on: Oct 21, 2014
 *      Author: cady
 */

#include "InterpolationModule.hpp"
#include "InvalidInputException.hpp"
#include "listeners.hpp"
#include "YamlCommands.hpp"

#include <ssc/macros.hpp>
#include TR1INC(memory)

void add_interpolation_table(const std::string& x_name, const std::vector<double>& x, const std::string& y_name, const std::vector<double>& y, ssc::data_source::DataSource& ds);
void add_interpolation_table(const std::string& x_name, const std::vector<double>& x, const std::string& y_name, const std::vector<double>& y, ssc::data_source::DataSource& ds)
{
    ds.check_in(__PRETTY_FUNCTION__);
    TR1(shared_ptr)<ssc::interpolation::LinearInterpolationVariableStep> I(new ssc::interpolation::LinearInterpolationVariableStep(x, y));

    const std::string module_name = x_name + "->" + y_name;
    InterpolationModule module(&ds, module_name, x_name, y_name, I);
    ds.add(module);
    ds.check_out();
}

std::string namify(const std::string& command_name, const std::string& model_name);
std::string namify(const std::string& command_name, const std::string& model_name)
{
    return model_name + "(" + command_name + ")";
}

void add(std::vector<YamlCommands>::const_iterator& that_command, ssc::data_source::DataSource& ds);
void add(std::vector<YamlCommands>::const_iterator& that_command, ssc::data_source::DataSource& ds)
{
    ds.check_in(__PRETTY_FUNCTION__);
    const auto t = that_command->t;
    if (t.size() == 1)
    {
        for (auto it = that_command->commands.begin() ; it != that_command->commands.end() ; ++it)
        {
            ds.set<double>(namify(it->first, that_command->name), it->second.front());
        }
    }
    else
    {
        for (auto it = that_command->commands.begin() ; it != that_command->commands.end() ; ++it)
        {
            try
            {
                add_interpolation_table("t", t, namify(it->first, that_command->name), it->second, ds);
            }
            catch(const ssc::interpolation::PiecewiseConstantVariableStepException& e)
            {
                THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unable to build interpolation table between 't' and '" << it->first << "' for force model '" << that_command->name << "': " << e.get_message());
            }
        }
    }
    ds.check_out();
}

ssc::data_source::DataSource make_command_listener(const std::vector<YamlCommands>& commands //!< Parsed YAML commands
                       )
{
    ssc::data_source::DataSource ds;
    ds.check_in(__PRETTY_FUNCTION__);
    for (auto that_command = commands.begin() ; that_command != commands.end() ; ++that_command)
    {
        add(that_command, ds);
    }
    ds.check_out();
    return ds;
}
