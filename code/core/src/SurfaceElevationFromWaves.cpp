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
    if (x.size() != y.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calculating surface elevation from waves: the x and y vectors don't have the same size (size of x: " << x.size() << ", size of y: " << y.size() << ")");
    }
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

double SurfaceElevationFromWaves::evaluate_rao(const double x, //!< x-position of the RAO's calculation point in the NED frame (in meters)
                    const double y, //!< y-position of the RAO's calculation point in the NED frame (in meters)
                    const double t, //!< Current time instant (in seconds)
                    const std::vector<std::vector<double> >& rao_module, //!< Module of the RAO (spectrum_index, flattened_omega_x_psi_index)
                    const std::vector<std::vector<double> >& rao_phase //!< Phase of the RAO (spectrum_index, flattened_omega_x_psi_index)
                     ) const
{
    double rao = 0;
    // The RAOs from the HDB file are interpolated by hdb_interpolators/DiffractionInterpolator
    // called by class DiffractionForceModel::Impl's constructor which ensures that the first
    // dimension of rao_phase & rao_module is the index of the directional spectrum and the
    // second index is the position in the "flattened" (omega,psi) matrix. The RAO's are interpolated
    // at the periods and incidences specified by each wave directional spectrum.
    for (size_t spectrum_idx = 0 ; spectrum_idx < directional_spectra.size() ; ++spectrum_idx)
    {
        const auto rao_module_for_each_frequency_and_incidence = rao_module.at(spectrum_idx);
        const auto rao_phase_for_each_frequency_and_incidence = rao_phase.at(spectrum_idx);
        rao += directional_spectra.at(spectrum_idx)->evaluate_rao(x,y,t,rao_module_for_each_frequency_and_incidence,rao_phase_for_each_frequency_and_incidence);
    }
    return rao;
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

ssc::kinematics::Point SurfaceElevationFromWaves::orbital_velocity(const double g,   //!< gravity (in m/s^2)
                                                                   const double x,   //!< x-position in the NED frame (in meters)
                                                                   const double y,   //!< y-position in the NED frame (in meters)
                                                                   const double z,   //!< z-position in the NED frame (in meters)
                                                                   const double t,   //!< z-position in the NED frame (in meters)
                                                                   const double eta  //!< Wave elevation at (x,y) in the NED frame (in meters)
                                                                   ) const
{
    ssc::kinematics::Point Vwaves("NED", 0, 0, 0);
    for (auto model:directional_spectra)
    {
        auto vw = model->orbital_velocity(g, x, y, z, t, eta);
        Vwaves.x() += vw.x();
        Vwaves.y() += vw.y();
        Vwaves.z() += vw.z();
    }
    return Vwaves;
}

ssc::kinematics::PointMatrix SurfaceElevationFromWaves::orbital_velocity(const double g,                //!< gravity (in m/s^2)
                                                                         const std::vector<double>& x,  //!< x-positions in the NED frame (in meters)
                                                                         const std::vector<double>& y,  //!< y-positions in the NED frame (in meters)
                                                                         const std::vector<double>& z,  //!< z-positions in the NED frame (in meters)
                                                                         const double t,                //!< Current time instant (in seconds)
                                                                         const std::vector<double>& eta //!< Wave elevations at (x,y) in the NED frame (in meters)
                                                                        ) const
{
    if (x.size() != y.size() || x.size() != z.size() || x.size() != eta.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calculating orbital velocity: the x, y, z and eta vectors don't have the same size (size of x: "
            << x.size() << ", size of y: " << y.size() << ", size of z: " << z.size() << ", size of eta: " << eta.size() << ")");
    }
    ssc::kinematics::PointMatrix Vwaves(ssc::kinematics::Matrix3Xd::Zero(3,x.size()), "NED");
    
    for (auto model:models)
    {
        ssc::kinematics::PointMatrix vw = model->orbital_velocity(g, x, y, z, t, eta);
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
