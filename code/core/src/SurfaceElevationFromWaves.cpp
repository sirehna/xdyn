/*
 * SurfaceElevationFromWaves.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#include "SurfaceElevationFromWaves.hpp"
#include "InternalErrorException.hpp"

#include <ssc/exception_handling.hpp>


SurfaceElevationFromWaves::SurfaceElevationFromWaves(
        const std::vector<WaveModelPtr>& models_,
        const std::pair<std::size_t,std::size_t> output_mesh_size_,
        const ssc::kinematics::PointMatrixPtr& output_mesh_) :
                SurfaceElevationInterface(output_mesh_, output_mesh_size_),
                models(models_)
{
    if(output_mesh_size_.first*output_mesh_size_.second != (std::size_t)output_mesh_->m.cols())
    {
        THROW(__PRETTY_FUNCTION__,ssc::exception_handling::Exception,"Number of columns in PointMatrix input should match the product of Nx*Ny");
    }
}

SurfaceElevationFromWaves::SurfaceElevationFromWaves(
        const WaveModelPtr& model,
        const std::pair<std::size_t,std::size_t> output_mesh_size_,
        const ssc::kinematics::PointMatrixPtr& output_mesh_) :
                SurfaceElevationInterface(output_mesh_, output_mesh_size_),
                models(std::vector<TR1(shared_ptr)<WaveModel> >(1,model))
{
    if(output_mesh_size_.first*output_mesh_size_.second != (std::size_t)output_mesh_->m.cols())
    {
        THROW(__PRETTY_FUNCTION__,ssc::exception_handling::Exception,"Number of columns in PointMatrix input should match the product of Nx*Ny");
    }
}

std::vector<double> SurfaceElevationFromWaves::wave_height(const std::vector<double> &x, //!< x-coordinates of the points, relative to the centre of the NED frame, projected in the NED frame
                                                           const std::vector<double> &y, //!< y-coordinates of the points, relative to the centre of the NED frame, projected in the NED frame
                                                           const double t                //!< Current instant (in seconds)
                                                           ) const
{
    if (x.size() != y.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calculating surface elevation from waves: the x and y vectors don't have the same size (size of x: " << x.size() << ", size of y: " << y.size() << ")");
    }
    std::vector<double> zwave(x.size(), 0);

    for (const auto model:models)
    {
        const std::vector<double> model_wave_height = model->get_elevation(x, y, t);
        for (size_t i = 0; i < model_wave_height.size(); ++i)
        {
            zwave.at(i) += model_wave_height.at(i);
        }
    }

    return zwave;
}

double SurfaceElevationFromWaves::evaluate_rao(const double x, //!< x-position of the RAO's calculation point in the NED frame (in meters)
                    const double y, //!< y-position of the RAO's calculation point in the NED frame (in meters)
                    const double t, //!< Current time instant (in seconds)
                    const std::vector<std::vector<double> >& rao_module, //!< Module of the RAO
                    const std::vector<std::vector<double> >& rao_phase //!< Phase of the RAO
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

std::vector<double> SurfaceElevationFromWaves::dynamic_pressure(const double rho,               //!< water density (in kg/m^3)
                                                                const double g,                 //!< gravity (in m/s^2)
                                                                const std::vector<double> &x,   //!< x-positions in the NED frame (in meters)
                                                                const std::vector<double> &y,   //!< y-positions in the NED frame (in meters)
                                                                const std::vector<double> &z,   //!< z-positions in the NED frame (in meters)
                                                                const std::vector<double> &eta, //!< Wave elevations at (x,y) in the NED frame (in meters)
                                                                const double t                  //!< Current time instant (in seconds)
                                                                ) const
{
    std::vector<double> pdyn(x.size(), 0);
    for (const auto model : models)
    {
        std::vector<double> dynamic_pressure_for_model = model->get_dynamic_pressure(rho, g, x, y, z, eta, t);
        for (size_t i = 0; i < pdyn.size(); ++i)
        {
            pdyn[i] += dynamic_pressure_for_model.at(i);
        }
    }
    return pdyn;
}

ssc::kinematics::Point SurfaceElevationFromWaves::orbital_velocity(const double g,   //!< gravity (in m/s^2)
                                                                   const double x,   //!< x-position in the NED frame (in meters)
                                                                   const double y,   //!< y-position in the NED frame (in meters)
                                                                   const double z,   //!< z-position in the NED frame (in meters)
                                                                   const double t,   //!< z-position in the NED frame (in meters)
                                                                   const double eta  //!< Wave elevation at (x,y) in the NED frame (in meters)
                                                                   ) const
{
    ssc::kinematics::Point Vwaves("NED", 0, 0, 0);
    for (auto model:models)
    {
        auto vw = model->orbital_velocity(g, x, y, z, t, eta);
        Vwaves.x() += vw.x();
        Vwaves.y() += vw.y();
        Vwaves.z() += vw.z();
    }
    return Vwaves;
}

void SurfaceElevationFromWaves::serialize_wave_spectra_before_simulation(ObserverPtr& observer) const
{
    std::vector<FlatDiscreteDirectionalWaveSpectrum> spectra;
    spectra.reserve(models.size());
    for (const auto model:models) spectra.push_back(model->get_spectrum());
    const DataAddressing address;
    observer->write_before_simulation(spectra, address);
}
