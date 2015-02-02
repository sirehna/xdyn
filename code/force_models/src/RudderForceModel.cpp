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

double RudderForceModel::RudderModel::get_angle_of_attack(const double rudder_angle, //!< Rudder angle (in radian): positive if rudder on port side
                                                          const double fluid_angle   //!< Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)
                                                          ) const
{
    return rudder_angle-fluid_angle;
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
