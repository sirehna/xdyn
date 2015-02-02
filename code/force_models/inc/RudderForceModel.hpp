/*
 * RudderForceModel.hpp
 *
 *  Created on: Jan 30, 2015
 *      Author: cady
 */

#ifndef RUDDERFORCEMODEL_HPP_
#define RUDDERFORCEMODEL_HPP_

#include "WageningenControlledForceModel.hpp"

class RudderForceModel : public ControllableForceModel
{
    public:
        struct Yaml : WageningenControlledForceModel::Yaml
        {

        };
        RudderForceModel(const Yaml& input, const std::string& body_name, const EnvironmentAndFrames& env);
        ssc::kinematics::Vector6d get_force(const BodyStates& states, const double t, std::map<std::string,double> commands) const;

        struct RudderModel
        {
            virtual ~RudderModel(){}
            double get_alpha() const;
            double get_lift() const;
            double get_drag() const;
            double get_Cd() const;
            double get_Cl() const;
            ssc::kinematics::Vector6d get_wrench() const;

            virtual double get_wake_angle() const = 0;
            virtual double get_wake_speed() const = 0;
        };

        struct RudderInWake : RudderModel
        {
            double get_wake_angle() const;
            double get_wake_speed() const;
        };

        struct RudderOutsideWake : RudderModel
        {
            double get_wake_angle() const;
            double get_wake_speed() const;
        };

    private:
        WageningenControlledForceModel propulsion;
        double envergure;
        double rho;
        double surf;
        double c;
};


#endif /* RUDDERFORCEMODEL_HPP_ */
