/*
 * WaveModel.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#include "WaveModel.hpp"
#include "InternalErrorException.hpp"
#include "discretize.hpp"
#include <cmath> // For isnan
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

DiscreteDirectionalWaveSpectrum add_constant_phases(DiscreteDirectionalWaveSpectrum spectrum, const double constant_phase);
DiscreteDirectionalWaveSpectrum add_constant_phases(DiscreteDirectionalWaveSpectrum spectrum, const double constant_phase)
{
    const size_t n = spectrum.Si.size();
    const size_t p = spectrum.Dj.size();
    if (n*p>0)
    {
        spectrum.phase.resize(n);
        for (size_t i = 0 ; i < n ; ++i)
        {
           spectrum.phase[i] = std::vector<double>(n, constant_phase);
        }

    }
    return spectrum;
}

DiscreteDirectionalWaveSpectrum add_random_phases(DiscreteDirectionalWaveSpectrum spectrum, const int random_number_generator_seed);
DiscreteDirectionalWaveSpectrum add_random_phases(DiscreteDirectionalWaveSpectrum spectrum, const int random_number_generator_seed)
{
    const size_t n = spectrum.Si.size();
    const size_t p = spectrum.Dj.size();
    if (n*p>0)
    {
        spectrum.phase.resize(n);
        boost::mt19937 rng((unsigned int)random_number_generator_seed);
        boost::random::uniform_real_distribution<double> generate_random_phase(0,2*PI);
        for (size_t i = 0 ; i < n ; ++i)
        {
           spectrum.phase[i].resize(p);
           for (size_t j = 0 ; j < p ; ++j)
           {
               spectrum.phase[i][j] = generate_random_phase(rng);
           }
        }

    }
    return spectrum;
}

WaveModel::WaveModel(const DiscreteDirectionalWaveSpectrum& spectrum_, const double constant_phase) : spectrum(add_constant_phases(spectrum_, constant_phase)), flat_spectrum(flatten(spectrum))
{
    check_sizes();
}

WaveModel::WaveModel(const DiscreteDirectionalWaveSpectrum& spectrum_, const int random_number_generator_seed) : spectrum(add_random_phases(spectrum_, random_number_generator_seed)), flat_spectrum(flatten(spectrum))
{
    check_sizes();
}

void WaveModel::check_sizes() const
{
    if (flat_spectrum.omega.empty())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "No 'omega' values defined in DiscreteDirectionalWaveSpectrum");
    }
    if (flat_spectrum.a.empty())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "No 'a' values defined in DiscreteDirectionalWaveSpectrum");
    }
    if (flat_spectrum.k.empty())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "No 'k' values defined in DiscreteDirectionalWaveSpectrum");
    }
    if (flat_spectrum.phase.empty())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "No 'phase' values defined in DiscreteDirectionalWaveSpectrum");
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
