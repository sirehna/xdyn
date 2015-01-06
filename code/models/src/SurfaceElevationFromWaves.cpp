/*
 * WaveModelFromSpectra.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#include <boost/foreach.hpp>

#include <ssc/exception_handling.hpp>

#include "WaveModel.hpp"
#include "SurfaceElevationFromWaves.hpp"

SurfaceElevationFromWaves::SurfaceElevationFromWaves(
        const std::vector<TR1(shared_ptr)<WaveModel> >& models_,
        const std::pair<std::size_t,std::size_t> output_mesh_size_,
        const TR1(shared_ptr)<ssc::kinematics::PointMatrix>& output_mesh_) :
                SurfaceElevationInterface(output_mesh_), models(models_), output_mesh_size(output_mesh_size_)
{
    if(output_mesh_size.first*output_mesh_size.second != (std::size_t)output_mesh_->m.cols())
    {
        THROW(__PRETTY_FUNCTION__,ssc::exception_handling::Exception,"Number of columns in PointMatrix input should match the product of Nx*Ny");
    }
}

SurfaceElevationFromWaves::SurfaceElevationFromWaves(
        const TR1(shared_ptr)<WaveModel>& model,
        const std::pair<std::size_t,std::size_t> output_mesh_size_,
        const TR1(shared_ptr)<ssc::kinematics::PointMatrix>& output_mesh_) :
                SurfaceElevationInterface(output_mesh_), models(std::vector<TR1(shared_ptr)<WaveModel> >(1,model)), output_mesh_size(output_mesh_size_)
{
    if(output_mesh_size.first*output_mesh_size.second != (std::size_t)output_mesh_->m.cols())
    {
        THROW(__PRETTY_FUNCTION__,ssc::exception_handling::Exception,"Number of columns in PointMatrix input should match the product of Nx*Ny");
    }
}

double SurfaceElevationFromWaves::wave_height(const double x, //!< x-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                              const double y, //!< y-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                              const double t  //!< Current instant (in seconds)
                                         ) const
{
    double zwave = 0;
    BOOST_FOREACH(const TR1(shared_ptr)<WaveModel> model, models) zwave += model->elevation(x,y,t);
    return zwave;
}

double SurfaceElevationFromWaves::evaluate_rao(const double x, //!< x-position of the RAO's calculation point in the NED frame (in meters)
                    const double y, //!< y-position of the RAO's calculation point in the NED frame (in meters)
                    const double t, //!< Current time instant (in seconds)
                    const std::vector<std::vector<std::vector<double> > >& rao_module, //!< Module of the RAO
                    const std::vector<std::vector<std::vector<double> > >& rao_phase //!< Phase of the RAO
                     ) const
{
    double rao = 0;
    for (size_t i = 0 ; i < models.size() ; ++i)
    {
        rao += models.at(i)->evaluate_rao(x,y,t,rao_module.at(i),rao_phase.at(i));
    }
    return rao;
}

std::vector<std::vector<double> > SurfaceElevationFromWaves::get_wave_directions_for_each_model() const
{
    std::vector<std::vector<double> > ret;
    for (auto model:models)
    {
        ret.push_back(model->get_psis());
    }
    return ret;
}

std::vector<std::vector<double> > SurfaceElevationFromWaves::get_wave_angular_frequency_for_each_model() const
{
    std::vector<std::vector<double> > ret;
    for (auto model:models)
    {
        ret.push_back(model->get_omegas());
    }
    return ret;
}

double SurfaceElevationFromWaves::dynamic_pressure(const double rho, //!< water density (in kg/m^3)
                                                   const double g,   //!< gravity (in m/s^2)
                                                   const double x,   //!< x-position in the NED frame (in meters)
                                                   const double y,   //!< y-position in the NED frame (in meters)
                                                   const double z,   //!< z-position in the NED frame (in meters)
                                                   const double eta, //!< Wave elevation at (x,y) in the NED frame (in meters)
                                                   const double t    //!< Current time instant (in seconds)
                                                   ) const
{
    double pdyn = 0;
    BOOST_FOREACH(const TR1(shared_ptr)<WaveModel> model, models) pdyn += model->dynamic_pressure(rho,g,x,y,z,eta,t);
    return pdyn;
}
