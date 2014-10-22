/*
 * ControllableForceModel.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef CONTROLLABLEFORCEMODEL_HPP_
#define CONTROLLABLEFORCEMODEL_HPP_

#include <map>

#include <ssc/kinematics.hpp>

#include "YamlPosition.hpp"

namespace ssc { namespace data_source { class DataSource;}}
class Body;
class YamlRotation;

/** \brief These force models read commands from a DataSource.
 *  \details Provides facilities to the derived classes to retrieve the commands
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 *  \section ex1 Example
 *  \snippet model_wrappers/unit_tests/src/ControllableForceModelTest.cpp ControllableForceModelTest example
 *  \section ex2 Expected output
 *  \snippet model_wrappers/unit_tests/src/ControllableForceModelTest.cpp ControllableForceModelTest expected output
 */
class ControllableForceModel
{
    public:
        ControllableForceModel(const std::string& name, const std::vector<std::string>& commands, const YamlPosition& position_of_frame);
        virtual ~ControllableForceModel();
        ssc::kinematics::Wrench operator()(const Body& body, const double t, ssc::data_source::DataSource& command_listener) const;
        void add_reference_frame(const std::string& body_name, const ::ssc::kinematics::KinematicsPtr& k, const YamlRotation& rotations) const;
        virtual ssc::kinematics::Wrench get_force(const Body& body, const double t, std::map<std::string,double> commands) const = 0;

    private:
        ControllableForceModel(); // Deactivated
        double get_command(const std::string& command_name, ssc::data_source::DataSource& command_listener) const;
        std::map<std::string,double> get_commands(ssc::data_source::DataSource& command_listener) const;

        std::string name;
        std::vector<std::string> commands;
        YamlPosition position_of_frame;
};

#endif /* CONTROLLABLEFORCEMODEL_HPP_ */
