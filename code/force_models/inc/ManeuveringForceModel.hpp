/*
 * ManeuveringForceModel.hpp
 *
 *  Created on: Jan 19, 2015
 *      Author: cady
 */

#ifndef MANEUVERINGFORCEMODEL_HPP_
#define MANEUVERINGFORCEMODEL_HPP_

#include <map>

#include <ssc/data_source.hpp>
#include <ssc/macros.hpp>

#include "ControllableForceModel.hpp"
#include "YamlPosition.hpp"
#include "ManeuveringInternal.hpp"


#include TR1INC(memory)

class ManeuveringForceModel : public ControllableForceModel
{
    public:

        struct Yaml
        {
            Yaml();
            std::string name;
            YamlPosition frame_of_reference;
            std::vector<std::string> commands;
            std::map<std::string, std::string> var2expr;
        };
        ManeuveringForceModel(const Yaml& data, const std::string& body_name, const EnvironmentAndFrames& env);
        static Yaml parse(const std::string& yaml);
        ssc::kinematics::Vector6d get_force(const BodyStates& states, const double t, std::map<std::string,double> commands) const;
        static const std::string model_name;

    private:
        ManeuveringForceModel();
        ssc::kinematics::Point point_of_application;
        std::map<std::string, maneuvering::NodePtr> m;
        double g, nu, rho;
};

#endif /* MANEUVERINGFORCEMODEL_HPP_ */
