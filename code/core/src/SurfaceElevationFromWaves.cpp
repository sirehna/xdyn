/*
 * SurfaceElevationFromWaves.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#include "SurfaceElevationFromWaves.hpp"

#include <ssc/exception_handling.hpp>


SurfaceElevationFromWaves::SurfaceElevationFromWaves(
        const std::vector<WaveModelPtr>& models_,
        const std::pair<std::size_t,std::size_t> output_mesh_size_,
        const ssc::kinematics::PointMatrixPtr& output_mesh_) :
                SurfaceElevationInterface(output_mesh_, output_mesh_size_),
                directional_spectra(models_)
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
                directional_spectra(std::vector<WaveModelPtr>(1,model))
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
    std::vector<double> zwave(x.size(), 0);

    for (const auto directional_spectrum:directional_spectra)
    {
        const std::vector<double> wave_heights = directional_spectrum->get_elevation(x, y, t);
        for (size_t i = 0; i < wave_heights.size(); ++i)
        {
            zwave.at(i) += wave_heights.at(i);
        }
    }

    return zwave;
}

std::vector<FlatDiscreteDirectionalWaveSpectrum> SurfaceElevationFromWaves::get_directional_spectra(const double, const double, const double) const
{
    std::vector<FlatDiscreteDirectionalWaveSpectrum> ret;
    ret.reserve(directional_spectra.size());
    for (const auto& spectrum:directional_spectra)
    {
        ret.push_back(spectrum->get_spectrum());
    }
    return ret;
}

// For each spectrum, the wave propagation directions
// Usually, a vector of vectors of size 1: {{psi_1},{psi_2},...,{psi_n}}
std::vector<std::vector<double> > SurfaceElevationFromWaves::get_wave_directions_for_each_model() const
{
    std::vector<std::vector<double> > ret;
    for (auto model:directional_spectra)
    {
        ret.push_back(model->get_psis());
    }
    return ret;
}

// For each directional spectrum (i.e. for each direction), the wave angular frequencies the spectrum was discretized at.
// v[direction][omega]
std::vector<std::vector<double> > SurfaceElevationFromWaves::get_wave_angular_frequency_for_each_model() const
{
    std::vector<std::vector<double> > ret;
    for (auto model:directional_spectra)
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
    for (const auto spectrum : directional_spectra)
    {
        std::vector<double> dynamic_pressure_for_spectrum = spectrum->get_dynamic_pressure(rho, g, x, y, z, eta, t);
        for (size_t i = 0; i < pdyn.size(); ++i)
        {
            pdyn[i] += dynamic_pressure_for_spectrum.at(i);
        }
    }
    return pdyn;
}

ssc::kinematics::PointMatrix SurfaceElevationFromWaves::orbital_velocity(const double g,                //!< gravity (in m/s^2)
                                                                         const std::vector<double>& x,  //!< x-positions in the NED frame (in meters)
                                                                         const std::vector<double>& y,  //!< y-positions in the NED frame (in meters)
                                                                         const std::vector<double>& z,  //!< z-positions in the NED frame (in meters)
                                                                         const double t,                //!< Current time instant (in seconds)
                                                                         const std::vector<double>& eta //!< Wave elevations at (x,y) in the NED frame (in meters)
                                                                        ) const
{
    ssc::kinematics::PointMatrix Vwaves(ssc::kinematics::Matrix3Xd::Zero(3,x.size()), "NED");
    for (auto spectrum:directional_spectra)
    {
        ssc::kinematics::PointMatrix vw = spectrum->get_orbital_velocity(g, x, y, z, t, eta);
        for (int i = 0; i < vw.m.cols(); ++i) {
            Vwaves.m(0, i) += vw.m(0, i);
            Vwaves.m(1, i) += vw.m(1, i);
            Vwaves.m(2, i) += vw.m(2, i);
        }
    }
    return Vwaves;
}

void SurfaceElevationFromWaves::serialize_wave_spectra_before_simulation(ObserverPtr& observer) const
{
    std::vector<FlatDiscreteDirectionalWaveSpectrum> spectra;
    spectra.reserve(directional_spectra.size());
    for (const auto spectrum:directional_spectra) spectra.push_back(spectrum->get_spectrum());
    const DataAddressing address;
    observer->write_before_simulation(spectra, address);
}
