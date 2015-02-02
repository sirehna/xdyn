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

RudderForceModel::RudderModel::RudderModel(const double nu_, //!< Water viscosity (in m^2/s)
                                           const double Ar_, //!< Rudder area (in m^2) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                                           const double b,   //!< Rudder height (in m) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                                           const double effective_aspect_ratio_factor  //!< Non-dimensional (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 97 § b)
                                           ) :
                                           nu(nu_),
                                           chord(Ar_/b),
                                           Ar(Ar_),
                                           lambda(effective_aspect_ratio_factor * b*b / Ar)
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
    const double Rn = Vs * chord / nu;
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

double RudderForceModel::InWake::get_wake_angle() const
{
    return 0;
}

double RudderForceModel::InWake::get_relative_ship_speed() const
{
    return 0;
}

double RudderForceModel::OutsideWake::get_wake_angle() const
{
    return 0;
}

double RudderForceModel::OutsideWake::get_relative_ship_speed() const
{
    return 0;
}

RudderForceModel::OutsideWake::OutsideWake(const double nu_, //!< Water viscosity (in m^2/s)
                                           const double Ar_, //!< Rudder area (in m^2) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                                           const double b_,  //!< Rudder height (in m) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                                           const double effective_aspect_ratio_factor_  //!< Non-dimensional (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 97 § b)
                                           ) : RudderModel(nu_, Ar_, b_, effective_aspect_ratio_factor_)
{
}

RudderForceModel::InWake::InWake(const double nu_, //!< Water viscosity (in m^2/s)
                                 const double Ar_, //!< Rudder area (in m^2) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                                 const double b_,  //!< Rudder height (in m) (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 76 fig. 1.2.4)
                                 const double effective_aspect_ratio_factor_  //!< Non-dimensional (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 97 § b)
                               ) : RudderModel(nu_, Ar_, b_, effective_aspect_ratio_factor_)
{
}
