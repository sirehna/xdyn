/*
 * SimpleTrackKeepingController.hpp
 *
 *  Created on: Jan 15, 2015
 *      Author: cady
 */

#ifndef SIMPLETRACKKEEPINGCONTROLLER_HPP_
#define SIMPLETRACKKEEPINGCONTROLLER_HPP_

#include "ControllableForceModel.hpp"

class SimpleTrackKeepingController : public ControllableForceModel
{
    public:
        struct Yaml
        {
            Yaml();
            std::string name;
            double ksi;
            double Tp;
        };
        static Yaml parse(const std::string& yaml);
        static const std::string model_name;

        SimpleTrackKeepingController(const Yaml& input, const std::string& body_name, const EnvironmentAndFrames& env);
        ssc::kinematics::Vector6d get_force(const BodyStates& states, const double t, std::map<std::string,double> commands) const;

    private:
        SimpleTrackKeepingController();
        double ksi;
        double omega0;
        YamlRotation rotation_convention;
};

#endif /* SIMPLETRACKKEEPINGCONTROLLER_HPP_ */
