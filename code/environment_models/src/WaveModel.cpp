/*
 * WaveModel.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#include "WaveModel.hpp"
#include "InternalErrorException.hpp"
#include <cmath> // For isnan

WaveModel::WaveModel(const FlatDiscreteDirectionalWaveSpectrum& flat_spectrum_, const DiscreteDirectionalWaveSpectrum& spectrum_) : flat_spectrum(flat_spectrum_), spectrum(spectrum_)
{
    if (flat_spectrum.omega.empty())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "No 'omega' values devined in DiscreteDirectionalWaveSpectrum");
    }
    if (flat_spectrum.a.empty())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "No 'a' values devined in DiscreteDirectionalWaveSpectrum");
    }
    if (flat_spectrum.k.empty())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "No 'k' values devined in DiscreteDirectionalWaveSpectrum");
    }

    for (const auto omega:flat_spectrum.omega)
    {
        if (std::isnan(omega))
        {
            THROW(__PRETTY_FUNCTION__, InternalErrorException, "DiscreteDirectionalWaveSpectrum contains NaN values for omega: " << flat_spectrum.omega);
        }
    }

    for (const auto k:flat_spectrum.k)
    {
        if (std::isnan(k))
        {
            THROW(__PRETTY_FUNCTION__, InternalErrorException, "DiscreteDirectionalWaveSpectrum contains NaN values for k: " << flat_spectrum.k);
        }
    }
    for (const auto Si:flat_spectrum.a)
    {
        if (std::isnan(Si))
        {
            THROW(__PRETTY_FUNCTION__, InternalErrorException, "DiscreteDirectionalWaveSpectrum contains NaN values for a: " << flat_spectrum.a);
        }
    }
}

WaveModel::~WaveModel()
{
}

std::vector<double> WaveModel::get_omegas() const
{
    return flat_spectrum.omega;
}

std::vector<double> WaveModel::get_psis() const
{
    return flat_spectrum.psi;
}

std::vector<double> WaveModel::get_elevation(const std::vector<double> &x, //!< x-positions in the NED frame (in meters)
                                             const std::vector<double> &y, //!< y-positions in the NED frame (in meters)
                                             const double t                //!< Current time instant (in seconds)
                                            ) const
{
    if (x.size() != y.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException,
            "Error when calculating surface elevation: the x and y vectors don't have the same size (size of x: " << x.size() << ", size of y: " << y.size() << ")");
    }
    return elevation(x, y, t);
}

ssc::kinematics::PointMatrix WaveModel::get_orbital_velocity(
        const double g,                //!< gravity (in m/s^2)
        const std::vector<double>& x,  //!< x-positions in the NED frame (in meters)
        const std::vector<double>& y,  //!< y-positions in the NED frame (in meters)
        const std::vector<double>& z,  //!< z-positions in the NED frame (in meters)
        const double t,                //!< Current time instant (in seconds)
        const std::vector<double>& eta //!< Wave heights at x,y,t (in meters)
        ) const
{
    if (x.size() != y.size() || x.size() != z.size() || x.size() != eta.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Error when calculating orbital velocity: the x, y, z and eta vectors don't have the same size (size of x: "
            << x.size() << ", size of y: " << y.size() << ", size of z: " << z.size() << ", size of eta: " << eta.size() << ")");
    }
    return orbital_velocity(g, x, y, z, t, eta);
}

std::vector<double> WaveModel::get_dynamic_pressure(const double rho,               //!< water density (in kg/m^3)
                                                    const double g,                 //!< gravity (in m/s^2)
                                                    const std::vector<double> &x,   //!< x-positions in the NED frame (in meters)
                                                    const std::vector<double> &y,   //!< y-positions in the NED frame (in meters)
                                                    const std::vector<double> &z,   //!< z-positions in the NED frame (in meters)
                                                    const std::vector<double> &eta, //!< Wave elevations at (x,y) in the NED frame (in meters)
                                                    const double t                  //!< Current time instant (in seconds)
                                                   ) const
{
    if (x.size() != y.size() || x.size() != z.size() || x.size() != eta.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException,
              "Error when calculating dynamic pressure: the x, y, z and eta vectors don't have the same size (size of x: " << x.size()
                << ", size of y: " << y.size() << ", size of z: " << z.size() << ", size of eta: " << eta.size() << ")");
    }
    return dynamic_pressure(rho, g, x, y, z, eta, t);
}
