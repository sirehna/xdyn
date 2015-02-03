/*
 * RudderForceModel.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "RudderForceModel.hpp"

RudderForceModel::Yaml::Yaml() :
                nu(0),
                rho(0),
                Ar(0),
                b(0),
                effective_aspect_ratio_factor(0),
                lift_coeff(0),
                drag_coeff(0),
                distance_between_rudder_and_screw(0)
{

}

RudderForceModel::RudderModel::RudderModel(const Yaml& parameters_) :
                                           parameters(parameters_),
                                           chord(parameters.Ar/parameters.b),
                                           lambda(parameters.effective_aspect_ratio_factor * parameters.b*parameters.b / parameters.Ar),
                                           D(parameters_.diameter),
                                           Kr(0.5+0.5/(1+0.15/std::abs(parameters_.distance_between_rudder_and_screw/parameters_.diameter)))
{
}

double RudderForceModel::RudderModel::get_angle_of_attack(const double rudder_angle, //!< Rudder angle (in radian): positive if rudder on port side
                                                          const double fluid_angle   //!< Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)
                                                          ) const
{
    return rudder_angle-fluid_angle;
}

double RudderForceModel::RudderModel::get_Cd(const double Vs, //!< Norm of the speed of the ship relative to the fluid
                                             const double Cl  //!< Rudder lift coefficient (non-dimensional)
                                             ) const
{
    // Reynolds number of the rudder (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 78 eq. 1.2.12)
    const double Rn = Vs * chord / parameters.nu;
    // ITTC resistance coefficient , "Marine rudders and Control Surfaces" p.31 eq. 3.18
    const double Cf = 0.075 / pow((log(Rn)/log(10.0)-2),2);
    // Resistance coefficient (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 78 (§ "for Cd0"))
    const double Cd0 = 2.5 * Cf;
    // Rudder drag coefficient (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 78 eq. 1.2.9)
    return 1.1 * Cl*Cl / (PI * lambda) + Cd0;
}

double RudderForceModel::RudderModel::get_Cl(const double alpha_wake //!< Angle of the fluid in the ship's reference frame (in radian)
                                            ) const
{
    return 2 * PI * lambda * (lambda + 1) / (lambda + 2) / (lambda + 2) * sin(alpha_wake);
}

double RudderForceModel::RudderModel::get_lift(const double Vs,//!< Norm of the speed of the ship relative to the fluid
                                               const double Cl,//!< Rudder lift coefficient (non-dimensional)
                                               const double alpha, //!< Angle between the propeller's wake & the rudder (in radian)
                                               const double area   //!< Rudder area (in or outside wake) in m^2
                                               ) const
{
    return 0.5 * parameters.rho * area * Vs*Vs * Cl * cos(alpha) * parameters.lift_coeff;
}

double RudderForceModel::RudderModel::get_drag(const double Vs,//!< Norm of the speed of the ship relative to the fluid
                                               const double Cl,//!< Rudder lift coefficient (non-dimensional)
                                               const double alpha, //!< Angle between the propeller's wake & the rudder (in radian)
                                               const double area   //!< Rudder area (in or outside wake) in m^2
                                               ) const
{
    return 0.5 * parameters.rho * area * Vs*Vs * Cl * cos(alpha) * parameters.drag_coeff;
}

ssc::kinematics::Vector6d RudderForceModel::RudderModel::get_force(const double lift, //!< Norm of the lift (in N)
                                                                   const double drag, //!< Norm of the drag (in N)
                                                                   const double angle //!< Angle between the fluid & the rudder (in radian)
                                    ) const
{
    ssc::kinematics::Vector6d ret = ssc::kinematics::Vector6d::Zero();
    ret(0) = - lift * sin (angle) - drag * cos (angle);
    ret(1) = + lift * cos (angle) - drag * sin (angle);
    return ret;
}

ssc::kinematics::Vector6d RudderForceModel::RudderModel::get_wrench(const double rudder_angle, //!< Rudder angle (in radian): positive if rudder on port side
                                                                    const double fluid_angle,  //!< Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)
                                                                    const double Vs,           //!< Norm of the speed of the ship relative to the fluid (in m/s)
                                                                    const double area          //!< Rudder area (in or outside wake) in m^2
                                                                    ) const
{
    const double alpha = get_angle_of_attack(rudder_angle, fluid_angle);
    const double Cl = get_Cl(fluid_angle);
    const double lift = get_lift(Vs, Cl, alpha, area);
    const double Cd = get_Cd(Vs, Cl);
    const double drag = get_drag(Vs, Cd, alpha, area);
    return get_force(lift, drag, fluid_angle);
}

RudderForceModel::Ar RudderForceModel::RudderModel::get_Ar(const double CTh //!< Thrust loading coefficient, Cf. "Manoeuvring Technical Manual", J. Brix, Seehafen Verlag p. 84, eq. 1.2.20
                                                          ) const
{
    Ar ar;
    // Jet speed coefficient, "Manoeuvring Technical Manual", J. Brix, Seehafen Verlag p. 96 eq. 1.2.44
    const double Cj = 1 + Kr * (sqrt(1 + CTh) -1);
    const double Dwake = D * sqrt((1 + 0.5 * (sqrt(1 + CTh) - 1)) / Cj);
    ar.in_wake = std::min(parameters.Ar, chord*Dwake);
    ar.outside_wake = ar.in_wake-parameters.Ar;
    return ar;
}
