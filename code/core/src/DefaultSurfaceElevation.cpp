/*
 * DefaultSurfaceElevation.cpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#include "DefaultSurfaceElevation.hpp"

DefaultSurfaceElevation::DefaultSurfaceElevation(
        const double wave_height_,
        const ssc::kinematics::PointMatrixPtr& output_mesh_,
        const std::pair<std::size_t,std::size_t> output_mesh_size_):
            SurfaceElevationInterface(output_mesh_, output_mesh_size_), zwave(wave_height_)
{
}

std::vector<double> DefaultSurfaceElevation::wave_height(const std::vector<double> &x, const std::vector<double> &, const double) const
{
    return std::vector<double>(x.size(), zwave);
}

ssc::kinematics::PointMatrix DefaultSurfaceElevation::orbital_velocity(
    const double,                 //!< gravity (in m/s^2)
    const std::vector<double>& x, //!< x-positions in the NED frame (in meters)
    const std::vector<double>&,   //!< y-positions in the NED frame (in meters)
    const std::vector<double>&,   //!< z-positions in the NED frame (in meters)
    const double,                 //!< current instant (in seconds)
    const std::vector<double>&    //!< Wave elevations at (x,y) in the NED frame (in meters)
    ) const
{
    return ssc::kinematics::PointMatrix(ssc::kinematics::Matrix3Xd::Zero(3,x.size()), "NED");
}

std::vector<double> DefaultSurfaceElevation::dynamic_pressure(const double,                 //!< water density (in kg/m^3)
                                                              const double,                 //!< gravity (in m/s^2)
                                                              const std::vector<double> &x, //!< x-positions in the NED frame (in meters)
                                                              const std::vector<double> &,  //!< y-positions in the NED frame (in meters)
                                                              const std::vector<double> &,  //!< z-positions in the NED frame (in meters)
                                                              const std::vector<double> &,  //!< Wave elevations at (x,y) in the NED frame (in meters)
                                                              const double                  //!< Current time instant (in seconds)
                                                              ) const
{
    return std::vector<double>(x.size(), 0);
}
