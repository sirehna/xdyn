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
                drag_coeff(0)
{

}

RudderForceModel::RudderModel::RudderModel(const Yaml& parameters_) :
                                           parameters(parameters_),
                                           chord(parameters.Ar/parameters.b),
                                           lambda(parameters.effective_aspect_ratio_factor * parameters.b*parameters.b / parameters.Ar)
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
                                               const double alpha //!< Angle between the propeller's wake & the rudder (in radian)
                                               ) const
{
    return 0.5 * parameters.rho * parameters.Ar * Vs*Vs * Cl * cos(alpha) * parameters.lift_coeff;
}

double RudderForceModel::RudderModel::get_drag(const double Vs,//!< Norm of the speed of the ship relative to the fluid
                                               const double Cl,//!< Rudder lift coefficient (non-dimensional)
                                               const double alpha //!< Angle between the propeller's wake & the rudder (in radian)
                                               ) const
{
    return 0.5 * parameters.rho * parameters.Ar * Vs*Vs * Cl * cos(alpha) * parameters.drag_coeff;
}

double RudderForceModel::InWake::get_wake_angle(const double , //!< Speed of the ship (relative to the water, taking current & wave orbital velocity into account) along the X-axis of the BODY frame, in m/s
        const double   //!< Speed of the ship (relative to the water, taking current & wave orbital velocity into account) along the X-axis of the BODY frame, in m/s
        ) const
{
    return 0;
}

double RudderForceModel::InWake::get_relative_ship_speed() const
{
    return 0;
}
//atan2 (V_nav_rel[1], V_nav_rel[0])(vitesse du courant et vitesse orbitale de la houle prises en compte)
double RudderForceModel::OutsideWake::get_wake_angle(const double u, //!< Speed of the ship (relative to the water, taking current & wave orbital velocity into account) along the X-axis of the BODY frame, in m/s
                                                     const double v  //!< Speed of the ship (relative to the water, taking current & wave orbital velocity into account) along the X-axis of the BODY frame, in m/s
        ) const
{
    return atan2 (v, u);
}

double RudderForceModel::OutsideWake::get_relative_ship_speed() const
{
    return 0;
}

RudderForceModel::OutsideWake::OutsideWake(const Yaml& parameters_) : RudderModel(parameters_)
{
}

RudderForceModel::InWake::InWake(const Yaml& parameters_) : RudderModel(parameters_)
{
}
