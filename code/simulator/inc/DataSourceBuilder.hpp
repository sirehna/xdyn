/*
 * DataSourceBuilder.hpp
 *
 *  Created on: 18 avr. 2014
 *      Author: cady
 */

#ifndef DATASOURCEBUILDER_HPP_
#define DATASOURCEBUILDER_HPP_

#include "DataSource.hpp"
#include "YamlSimulatorInput.hpp"

class DataSourceBuilder
{
    public:
        DataSourceBuilder(const YamlSimulatorInput& input);
        DataSource build_ds();

    private:
        void add_initial_conditions(const YamlBody& body);
        void add_initial_quaternions(const YamlBody& body);
        void add_states(const YamlBody& body);
        void add_forces(const YamlBody& body);
        void add_gravity(const std::string& body_name, const std::string& yaml, const double mass);

        YamlSimulatorInput input;
        DataSource ds;
};

#endif /* DATASOURCEBUILDER_HPP_ */
