/*
 * DefaultSurfaceElevation.cpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#include "DefaultSurfaceElevation.hpp"

DefaultSurfaceElevation::DefaultSurfaceElevation(const double wave_height_, const TR1(shared_ptr)<ssc::kinematics::PointMatrix>& output_mesh_) : SurfaceElevationInterface(output_mesh_), zwave(wave_height_)
{
}

double DefaultSurfaceElevation::wave_height(const double , const double , const double z, const double ) const
{
    return z - zwave;
}

double DefaultSurfaceElevation::dynamic_pressure(const double ,   //!< water density (in kg/m^3)
                                                 const double ,   //!< gravity (in m/s^2)
                                                 const double ,   //!< x-position in the NED frame (in meters)
                                                 const double ,   //!< y-position in the NED frame (in meters)
                                                 const double ,   //!< z-position in the NED frame (in meters)
                                                 const double     //!< Current time instant (in seconds)
                                ) const
{
    return 0;
}
