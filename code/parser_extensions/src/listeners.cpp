/*
 * listeners.cpp
 *
 *  Created on: Oct 21, 2014
 *      Author: cady
 */

#include "InterpolationModule.hpp"
#include "listeners.hpp"
#include "YamlCommands.hpp"
#include "parse_commands.hpp"

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

void add(std::vector<YamlCommands>::iterator& that_command, ssc::data_source::DataSource& ds);
void add(std::vector<YamlCommands>::iterator& that_command, ssc::data_source::DataSource& ds)
{
    ds.check_in(__PRETTY_FUNCTION__);
    const auto t = that_command->t;
    if (t.size() == 1)
    {
        for (auto it = that_command->commands.begin() ; it != that_command->commands.end() ; ++it)
        {
            ds.set<double>(it->first, it->second.front());
        }
    }
    else
    {
        for (auto it = that_command->commands.begin() ; it != that_command->commands.end() ; ++it)
        {
            add_interpolation_table("t", t, that_command->name + "(" + it->first + ")", it->second, ds);
        }
    }
    ds.check_out();
}

ssc::data_source::DataSource listen_to_file(const std::string& yaml //!< YAML data containing the interpolation tables
                       )
{
    ssc::data_source::DataSource ds;
    ds.check_in(__PRETTY_FUNCTION__);
    auto commands = parse_command_yaml(yaml);
    for (auto that_command = commands.begin() ; that_command != commands.end() ; ++that_command)
    {
        add(that_command, ds);
    }
    ds.check_out();
    return ds;
}
