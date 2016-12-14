/*
 * ControllableForceModel.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef CONTROLLABLEFORCEMODEL_HPP_
#define CONTROLLABLEFORCEMODEL_HPP_

#include <map>

#include <boost/optional/optional.hpp>

#include <ssc/kinematics.hpp>

#include "yaml-cpp/exceptions.h"
#include "InvalidInputException.hpp"
#include "YamlBody.hpp"

#include "EnvironmentAndFrames.hpp"
#include "YamlPosition.hpp"

namespace ssc { namespace data_source { class DataSource;}}
struct BodyStates;
struct YamlRotation;

class ControllableForceModel;
typedef TR1(shared_ptr)<ControllableForceModel> ControllableForcePtr;
typedef std::vector<ControllableForcePtr> ListOfControlledForces;
typedef std::function<boost::optional<ControllableForcePtr>(const YamlModel&, const std::string&, const EnvironmentAndFrames&)> ControllableForceParser;

class Observer;

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
        ControllableForceModel(const std::string& name, const std::vector<std::string>& commands, const YamlPosition& position_of_frame, const std::string& body_name_, const EnvironmentAndFrames& env);
        virtual ~ControllableForceModel();
        ssc::kinematics::Wrench operator()(const BodyStates& states, const double t, ssc::data_source::DataSource& command_listener) const;
        ssc::kinematics::Wrench get_force_in_body_frame() const;
        void update(const BodyStates& states, const double t, ssc::data_source::DataSource& command_listener);
        void add_reference_frame(const ::ssc::kinematics::KinematicsPtr& k, const YamlRotation& rotations) const;
        virtual ssc::kinematics::Vector6d get_force(const BodyStates& states, const double t, std::map<std::string,double> commands) const = 0;
        std::string get_name() const;
        virtual double get_Tmax() const; // Can be overloaded if model needs access to History (not a problem, just has to say how much history to keep)
        std::string get_body_name() const;

        template <typename ControllableForceType>
        static ControllableForceParser build_parser()
        {
            auto parser = [](const YamlModel& yaml, const std::string& body_name, const EnvironmentAndFrames& env) -> boost::optional<ControllableForcePtr>
                          {
                              boost::optional<ControllableForcePtr> ret;
                              if (yaml.model == ControllableForceType::model_name())
                              {
                                  std::string context = "Invalid input data for model '" + ControllableForceType::model_name() + "'.";
                                  try
                                  {
                                      ret.reset(ControllableForcePtr(new ControllableForceType(ControllableForceType::parse(yaml.yaml), body_name, env)));
                                  }
                                  catch (const InvalidInputException& exception)
                                  {
                                      THROW(__PRETTY_FUNCTION__, InvalidInputException, context << std::endl << exception.get_message() << std::endl << "Model containing error is defined line "
                                              << yaml.index_of_first_line_in_global_yaml << " of the YAML file." << std::endl);
                                  }
                                  catch (const YAML::Exception& exception)
                                  {
                                      const size_t line_number = yaml.index_of_first_line_in_global_yaml;
                                      THROW(__PRETTY_FUNCTION__, InvalidInputException, context << std::endl << "Model containing error is defined line "
                                                                                         << line_number << " of the YAML file." << std::endl << "The error was: " << exception.msg);
                                  }
                              }
                              return ret;
                          };
            return parser;
        }

        void feed(Observer& observer) const;

    protected:
        EnvironmentAndFrames env;
        std::vector<std::string> commands;

    private:
        ControllableForceModel(); // Deactivated
        double get_command(const std::string& command_name, ssc::data_source::DataSource& command_listener, const double t) const;
        std::map<std::string,double> get_commands(ssc::data_source::DataSource& command_listener, const double t) const;

        std::string name;
        std::string body_name;
        YamlPosition position_of_frame;
        ssc::kinematics::Point point_of_application;
        ssc::kinematics::Wrench force_in_body_frame;
        ssc::kinematics::Wrench force_in_ned_frame;
};

#endif /* CONTROLLABLEFORCEMODEL_HPP_ */
