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

            /**  \brief Calculates the angle between the propeller's wake & the rudder
              *  \details When positive, the wake is coming towards the rudder's port side
              *  \returns Angle of attack (in radian)
              *  \snippet force_models/unit_tests/src/RudderForceModelTest.cpp RudderForceModelTest get_alpha_example
              */
            double get_angle_of_attack() const;

            /**  \brief Calculates the norm of the lift force acting on the rudder
              *  \returns Lift force (in Newton)
              */
            double get_lift() const;

            /**  \brief Calculates the norm of the drag force acting on the rudder
              *  \returns Drag force (in Newton)
              */
            double get_drag() const;

            /**  \brief Calculates the drag coefficient (non-dimensional)
              */
            double get_Cd() const;

            /**  \brief Calculates the lift coefficient (non-dimensional)
              */
            double get_Cl() const;

            /**  \brief Wrench created by the rudder on the ship
             *   \details Expressed in the rudder's reference frame
              */
            ssc::kinematics::Vector6d get_wrench() const;

            /**  \brief Angle of the fluid in the ship's reference frame
             *   \details If the fluid is propagating along -X, the angle is 0.
              */
            virtual double get_wake_angle() const = 0;

            /**  \brief Norm of the speed of the ship relative to the fluid
             *   \details In m/s
              */
            virtual double get_relative_ship_speed() const = 0;
        };

        struct RudderInWake : RudderModel
        {
            /**  \brief Angle of the fluid in the ship's reference frame
             *   \details If the fluid is propagating along -X, the angle is 0.
              */
            double get_wake_angle() const;

            /**  \brief Norm of the speed of the ship relative to the fluid
             *   \details In m/s
              */
            double get_relative_ship_speed() const;
        };

        struct RudderOutsideWake : RudderModel
        {
            /**  \brief Angle of the fluid in the ship's reference frame
             *   \details If the fluid is propagating along -X, the angle is 0.
              */
            double get_wake_angle() const;

            /**  \brief Norm of the speed of the ship relative to the fluid
             *   \details In m/s
              */
            double get_relative_ship_speed() const;
        };

    private:
        WageningenControlledForceModel propulsion;
        double envergure;
        double rho;
        double surf;
        double c;
};


#endif /* RUDDERFORCEMODEL_HPP_ */
