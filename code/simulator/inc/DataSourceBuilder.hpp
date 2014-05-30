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
#include "RotationMatrix.hpp"

class EulerAngles;
class DataSourceBuilder
{
    public:
        DataSourceBuilder(const YamlSimulatorInput& input);
        DataSource build_ds();

    private:
        DataSourceBuilder();
        void add_initial_conditions(const YamlBody& body);
        void add_initial_quaternions(const YamlBody& body);
        void add_states(const YamlBody& body);
        void add_forces(const YamlBody& body);
        void add_mesh(const YamlBody& body);
        void add_gravity(const std::string& body_name, const std::string& yaml, const double mass);
        void add_hydrostatic(const std::string& body_name, const std::string& yaml);
        void add_kinematics(const std::vector<YamlBody>& bodies);
        void add_centre_of_gravity(const YamlBody& body);
        void add_wave_height_module(const YamlBody& body);
        void add_sum_of_forces_module(const YamlBody& body);
        void add_default_wave_model(const std::string& yaml);
        void add_environment_model(const YamlModel& model);
        RotationMatrix angle2matrix(const EulerAngles& angle) const;

        YamlSimulatorInput input;
        DataSource ds;
        YamlRotation rotations;
};

#endif /* DATASOURCEBUILDER_HPP_ */
