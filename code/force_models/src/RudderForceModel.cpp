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
