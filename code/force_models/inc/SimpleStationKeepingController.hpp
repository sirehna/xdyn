/*
 * SimpleStationKeepingController.hpp
 *
 *  Created on: Jan 29, 2015
 *      Author: cady
 */

#ifndef SIMPLESTATIONKEEPINGCONTROLLER_HPP_
#define SIMPLESTATIONKEEPINGCONTROLLER_HPP_

#include "ControllableForceModel.hpp"

class SimpleStationKeepingController : public ControllableForceModel
{
    public:
        struct Yaml
        {
            Yaml();
            std::string name;
            double ksi_x;
            double T_x;
            double ksi_y;
            double T_y;
            double ksi_psi;
            double T_psi;
        };
        static Yaml parse(const std::string& yaml);
        static const std::string model_name;

        SimpleStationKeepingController(const Yaml& input, const std::string& body_name, const EnvironmentAndFrames& env);
        ssc::kinematics::Vector6d get_force(const BodyStates& states, const double t, std::map<std::string,double> commands) const;

    private:
        SimpleStationKeepingController();
        double ksi_x;
        double omega_x;
        double ksi_y;
        double omega_y;
        double ksi_psi;
        double omega_psi;
        YamlRotation rotation_convention;
};

#endif /* SIMPLESTATIONKEEPINGCONTROLLER_HPP_ */
