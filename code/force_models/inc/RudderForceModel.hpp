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
            Yaml();
            Yaml(const WageningenControlledForceModel::Yaml& yaml);
            double Ar;                                                      //!< Rudder area (in m^2) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
            double b;                                                       //!< Rudder height (in m) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
            double effective_aspect_ratio_factor;                           //!< Non-dimensional (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 97 § b)
            double lift_coeff;                                              //!< Non-dimensional: lift is multiplied by it (for tuning)
            double drag_coeff;                                              //!< Non-dimensional: drag is multiplied by it (for tuning)
            YamlCoordinates position_of_the_rudder_frame_in_the_body_frame;
        };

        RudderForceModel(const Yaml& input, const std::string& body_name, const EnvironmentAndFrames& env);
        ssc::kinematics::Vector6d get_force(const BodyStates& states, const double t, std::map<std::string,double> commands) const;
        ssc::kinematics::Point get_ship_speed(const BodyStates& states, const double t) const;
        static Yaml parse(const std::string& yaml);


        template <typename T> struct InOutWake
        {
            InOutWake() : in_wake(), outside_wake() {}
            T in_wake;
            T outside_wake;
        };

        struct RudderModel
        {
            RudderModel(const Yaml& parameters,
                        const double rho,
                        const double nu
                        );
            virtual ~RudderModel(){}

            /**  \brief Calculates the rudder area (in or outside wake)
              *  \returns Rudder area (in m^2)
              */
            InOutWake<double> get_Ar(const double CTh //!< Thrust loading coefficient, Cf. "Manoeuvring Technical Manual", J. Brix, Seehafen Verlag p. 84, eq. 1.2.20
                                    ) const;

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
            double get_lift(const double Vs,    //!< Norm of the speed of the ship relative to the fluid
                            const double Cl,    //!< Rudder lift coefficient (non-dimensional)
                            const double alpha, //!< Angle between the propeller's wake & the rudder (in radian)
                            const double area   //!< Rudder area (in or outside wake) in m^2
                            ) const;

            /**  \brief Calculates the norm of the drag force acting on the rudder
              *  \returns Drag force (in Newton)
              */
            double get_drag(const double Vs,    //!< Norm of the speed of the ship relative to the fluid
                            const double Cl,    //!< Rudder lift coefficient (non-dimensional)
                            const double alpha, //!< Angle between the propeller's wake & the rudder (in radian)
                            const double area   //!< Rudder area (in or outside wake) in m^2
                            ) const;

            /**  \brief Calculates the drag coefficient (non-dimensional)
              */
            double get_Cd(const double Vs, //!< Norm of the speed of the ship relative to the fluid
                          const double Cl  //!< Rudder lift coefficient (non-dimensional)
                          ) const;

            /**  \brief Calculates the lift coefficient (non-dimensional)
             *   \ref // Soeding formula, "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 97 eq 1.2.48 & p. 77 eq. 1.2.8
              */
            double get_Cl(const double alpha_wake //!< Angle of the fluid in the ship's reference frame (in radian)
                         ) const;

            /**  \brief Wrench created by the rudder on the ship
             *   \details Expressed in the rudder's reference frame
              */
            InOutWake<ssc::kinematics::Vector6d> get_wrench(const double rudder_angle, //!< Rudder angle (in radian): positive if rudder on port side
                                                            const InOutWake<double>& fluid_angle,  //!< Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)
                                                            const InOutWake<ssc::kinematics::Point>& Vs,           //!< Norm of the speed of the ship relative to the fluid (in m/s)
                                                            const InOutWake<double>& area          //!< Rudder area (in or outside wake) in m^2
                                                             ) const;

            /**  \brief Wrench created by the rudder on the ship
             *   \details Expressed in the rudder's reference frame
              */
            ssc::kinematics::Vector6d get_wrench(const double rudder_angle, //!< Rudder angle (in radian): positive if rudder on port side
                                                 const double fluid_angle,  //!< Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)
                                                 const double Vs,           //!< Norm of the speed of the ship relative to the fluid (in m/s)
                                                 const double area          //!< Rudder area (in or outside wake) in m^2
                                                             ) const;

            /**  \brief Wrench created by the rudder on the ship
             *   \details Expressed in the rudder's reference frame
              */
            ssc::kinematics::Vector6d get_force(const double lift, //!< Norm of the lift (in N)
                                                const double drag, //!< Norm of the drag (in N)
                                                const double angle //!< Angle between the fluid & the rudder (in radian)
                                                ) const;

            /**  \brief Calculates speed inside & outside wake
             *   \details Ship speed relative to the fluid, in m/s
              *  \returns Rudder area (in m^2)
              */
            InOutWake<ssc::kinematics::Point> get_vs(const double CTh, //!< Thrust loading coefficient, Cf. "Manoeuvring Technical Manual", J. Brix, Seehafen Verlag p. 84, eq. 1.2.20
                                                     const double Va,  //!< Projection of the ship speed (relative to the current) on the X-axis of the ship's reference frame (m/s)
                                                     const double v,   //!< Projection of the ship speed (relative to the current) on the X-axis of the ship's reference frame (m/s)
                                                     const double T    //!< Propeller thrust (in N)
                                                     ) const;

            /**  \brief Calculates the angle of incidence of the fluid, inside & outside wake
              *  \returns Angle in radian
              */
            InOutWake<double> get_fluid_angle(const InOutWake<ssc::kinematics::Point>& Vs   //!< Ship speed relative to the fluid, inside & outside wake
                                              ) const;

            double get_D() const;

            private:
                RudderModel(); // Disabled
                Yaml parameters;
                double chord;
                double lambda;
                double D; //!< Propeller diameter (in m)
                double Kr;//!< Contraction factor (cf. Marine Rudders & Control Surfaces, Molland & Turnock, eq. 3.37 p.51
                double rho;
                double nu;
        };

    private:
        WageningenControlledForceModel propulsion;
        ssc::kinematics::Point rudder_position;
        ssc::kinematics::Point translation_from_rudder_to_propeller;
        RudderModel model;
};


#endif /* RUDDERFORCEMODEL_HPP_ */
