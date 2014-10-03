/*
 * WaveModelFromSpectra.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#include <boost/foreach.hpp>

#include "WaveModel.hpp"
#include "SurfaceElevationFromWaves.hpp"

SurfaceElevationFromWaves::SurfaceElevationFromWaves(const std::vector<TR1(shared_ptr)<WaveModel> >& models_, const TR1(shared_ptr)<ssc::kinematics::PointMatrix>& output_mesh_) : SurfaceElevationInterface(output_mesh_), models(models_)
{
}

SurfaceElevationFromWaves::SurfaceElevationFromWaves(const TR1(shared_ptr)<WaveModel>& model, const TR1(shared_ptr)<ssc::kinematics::PointMatrix>& output_mesh_) : SurfaceElevationInterface(output_mesh_), models(std::vector<TR1(shared_ptr)<WaveModel> >(1,model))
{
}

double SurfaceElevationFromWaves::wave_height(const double x, //!< x-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                              const double y, //!< y-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                              const double z, //!< z-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                              const double t  //!< Current instant (in seconds)
                                         ) const
{
    double zwave = 0;
    BOOST_FOREACH(const TR1(shared_ptr)<WaveModel> model, models) zwave += model->elevation(x,y,t);
    return z - zwave;
}

double SurfaceElevationFromWaves::dynamic_pressure(const double rho, //!< water density (in kg/m^3)
                                                   const double g,   //!< gravity (in m/s^2)
                                                   const double x,   //!< x-position in the NED frame (in meters)
                                                   const double y,   //!< y-position in the NED frame (in meters)
                                                   const double z,   //!< z-position in the NED frame (in meters)
                                                   const double t    //!< Current time instant (in seconds)
                                                   ) const
{
    double pdyn = 0;
    BOOST_FOREACH(const TR1(shared_ptr)<WaveModel> model, models) pdyn += model->dynamic_pressure(rho,g,x,y,z,t);
    return pdyn;
}
