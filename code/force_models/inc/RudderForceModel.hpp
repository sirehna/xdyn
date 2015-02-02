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
            RudderModel(const double nu, //!< Water viscosity (in m^2/s)
                        const double rho, //!< Water density (in kg/m^3)
                        const double Ar, //!< Rudder area (in m^2) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                        const double b,  //!< Rudder height (in m) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                        const double effective_aspect_ratio_factor, //!< Non-dimensional (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 97 § b)
                        const double lift_coeff, //!< Non-dimensional: lift is multiplied by it (for tuning)
                        const double drag_coeff //!< Non-dimensional: drag is multiplied by it (for tuning)
                        );
            virtual ~RudderModel(){}

            /**  \brief Calculates the angle between the propeller's wake & the rudder
              *  \details When positive, the wake is coming towards the rudder's port side
              *  \returns Angle of attack (in radian)
              *  \snippet force_models/unit_tests/src/RudderForceModelTest.cpp RudderForceModelTest get_alpha_example
              */
            double get_angle_of_attack(const double rudder_angle, //!< Rudder angle (in radian): positive if rudder on port side
                                       const double fluid_angle   //!< Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)
                                      ) const;

            /**  \brief Calculates the norm of the lift force acting on the rudder
              *  \returns Lift force (in Newton)
              */
            double get_lift(const double Vs,   //!< Norm of the speed of the ship relative to the fluid
                            const double Cl,   //!< Rudder lift coefficient (non-dimensional)
                            const double alpha //!< Angle between the propeller's wake & the rudder (in radian)
                    ) const;

            /**  \brief Calculates the norm of the drag force acting on the rudder
              *  \returns Drag force (in Newton)
              */
            double get_drag() const;

            /**  \brief Calculates the drag coefficient (non-dimensional)
              */
            double get_Cd(const double Vs, //!< Norm of the speed of the ship relative to the fluid
                          const double Cl  //!< Rudder lift coefficient (non-dimensional)
                          ) const;

            /**  \brief Calculates the lift coefficient (non-dimensional)
             *   \ref // Soeding formula, "Manoeuvring Technical Manual", J. Brix, Seehafen Verlag, p. 97 eq 1.2.48 & p. 77 eq. 1.2.8
              */
            double get_Cl(const double alpha_wake //!< Angle of the fluid in the ship's reference frame (in radian)
                         ) const;

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

            private:
                RudderModel(); // Disabled
                double nu;     //!< Water viscosity (in m^2/s)
                double rho;    //!< Water density (in kg/m^3)
                double chord;  //!< Chord length (from tip to tail) (in meters) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                double Ar;     //!< Rudder area (in m^2) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                double lambda; //!< Effective aspect ratio (non-dimensional) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 97 § b)
                double lift_coeff; //!< Non-dimensional: lift is multiplied by it (for tuning)
                double drag_coeff; //!< Non-dimensional: drag is multiplied by it (for tuning)
        };

        struct InWake : RudderModel
        {
            InWake(const double nu, //!< Water viscosity (in m^2/s)
                   const double rho, //!< Water density (in kg/m^3)
                   const double Ar, //!< Rudder area (in m^2) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                   const double b,  //!< Rudder height (in m) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                   const double effective_aspect_ratio_factor,  //!< Non-dimensional (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 97 § b)
                   const double lift_coeff, //!< Non-dimensional: lift is multiplied by it (for tuning)
                   const double drag_coeff //!< Non-dimensional: drag is multiplied by it (for tuning)
                    );
            /**  \brief Angle of the fluid in the ship's reference frame
             *   \details If the fluid is propagating along -X, the angle is 0.
              */
            double get_wake_angle() const;

            /**  \brief Norm of the speed of the ship relative to the fluid
             *   \details In m/s
              */
            double get_relative_ship_speed() const;

            private:
            InWake(); // Disabled
        };

        struct OutsideWake : RudderModel
        {
            OutsideWake(const double nu, //!< Water viscosity (in m^2/s)
                        const double rho, //!< Water density (in kg/m^3)
                        const double Ar, //!< Rudder area (in m^2) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                        const double b,  //!< Rudder height (in m) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                        const double effective_aspect_ratio_factor,  //!< Non-dimensional (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 97 § b)
                        const double lift_coeff, //!< Non-dimensional: lift is multiplied by it (for tuning)
                        const double drag_coeff //!< Non-dimensional: drag is multiplied by it (for tuning)
                       );
            /**  \brief Angle of the fluid in the ship's reference frame
             *   \details If the fluid is propagating along -X, the angle is 0.
              */
            double get_wake_angle() const;

            /**  \brief Norm of the speed of the ship relative to the fluid
             *   \details In m/s
              */
            double get_relative_ship_speed() const;


            private:
                OutsideWake(); // Disabled
        };

    private:
        WageningenControlledForceModel propulsion;
        double envergure;
        double rho;
        double surf;
        double c;
};


#endif /* RUDDERFORCEMODEL_HPP_ */
