/*
 * Airy.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#include "Airy.hpp"
#include "InternalErrorException.hpp"
#include "InvalidInputException.hpp"
#include "discretize.hpp"

#include <vector>
#include <ssc/macros.hpp>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI


Airy::Airy(const DiscreteDirectionalWaveSpectrum& spectrum_, const double constant_random_phase) : WaveModel(flatten(spectrum_)),
phase(std::vector<std::vector<double> >()),
rng(),
generate_random_phase(boost::random::uniform_real_distribution<double>(0,2*PI))
{
    const size_t n = spectrum.psi.size();
    spectrum.phase = std::vector<double> (n, constant_random_phase);
}

Airy::Airy(const DiscreteDirectionalWaveSpectrum& spectrum_, const int random_number_generator_seed) : WaveModel(flatten(spectrum_)),
phase(std::vector<std::vector<double> >()),
rng(boost::mt19937((unsigned int)random_number_generator_seed)),
generate_random_phase(boost::random::uniform_real_distribution<double>(0,2*PI))
{
    const size_t n = spectrum.psi.size();
    for (size_t i = 0 ; i < n ; ++i)
    {
        spectrum.phase.push_back(generate_random_phase(rng));
    }
}

Airy::Airy(const FlatDiscreteDirectionalWaveSpectrum& spectrum_, const double constant_random_phase) : WaveModel(spectrum_),
phase(std::vector<std::vector<double> >()),
rng(),
generate_random_phase(boost::random::uniform_real_distribution<double>(0,2*PI))
{
    const size_t n = spectrum.psi.size();
    spectrum.phase = std::vector<double> (n, constant_random_phase);
}

Airy::Airy(const FlatDiscreteDirectionalWaveSpectrum& spectrum_, const int random_number_generator_seed) : WaveModel(spectrum_),
phase(std::vector<std::vector<double> >()),
rng(boost::mt19937((unsigned int)random_number_generator_seed)),
generate_random_phase(boost::random::uniform_real_distribution<double>(0,2*PI))
{
    const size_t n = spectrum.psi.size();
    for (size_t i = 0 ; i < n ; ++i)
    {
        spectrum.phase.push_back(generate_random_phase(rng));
    }
}

double Airy::evaluate_rao(
        const double x,                           //!< x-position of the RAO's calculation point in the NED frame (in meters)
        const double y,                           //!< y-position of the RAO's calculation point in the NED frame (in meters)
        const double t,                           //!< Current time instant (in seconds)
        const std::vector<double>& rao_module,    //!< Module of the RAO
        const std::vector<double>& rao_phase      //!< Phase of the RAO
        ) const
{
    double F = 0;
    const size_t n = rao_module.size();
    if (n != spectrum.k.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "rao size is " << n << ", which does not match spectrum size (" << spectrum.k.size() << ")");
    }
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double rao_a = rao_module[i] * spectrum.a[i];
        const double omega_t = spectrum.omega[i] * t;
        const double k_xCosPsi_ySinPsi = spectrum.k[i] * (x * spectrum.cos_psi[i] + y * spectrum.sin_psi[i]);
        const double theta = spectrum.phase[i];
        F -= rao_a * sin(-omega_t + k_xCosPsi_ySinPsi + theta + rao_phase[i]);
    }
    return F;
}

std::vector<double> Airy::elevation(
    const std::vector<double> &x, //!< x-positions in the NED frame (in meters)
    const std::vector<double> &y, //!< y-positions in the NED frame (in meters)
    const double t                //!< Current time instant (in seconds)
    ) const
{
    std::vector<double> zeta(x.size());
    const size_t n = spectrum.psi.size();

    for (size_t j = 0; j < zeta.size(); ++j) {
        for (size_t i = 0 ; i < n ; ++i)
        {
            const double a = spectrum.a[i];
            const double omega_t = spectrum.omega[i] * t;
            const double k_xCosPsi_ySinPsi = spectrum.k[i] * (x.at(j) * spectrum.cos_psi[i] + y.at(j) * spectrum.sin_psi[i]);
            const double theta = spectrum.phase[i];
            zeta.at(j) -= a * sin(-omega_t + k_xCosPsi_ySinPsi + theta);
        }
    }

    return zeta;
}

std::vector<double> Airy::dynamic_pressure(
    const double rho,               //!< water density (in kg/m^3)
    const double g,                 //!< gravity (in m/s^2)
    const std::vector<double> &x,   //!< x-positions in the NED frame (in meters)
    const std::vector<double> &y,   //!< y-positions in the NED frame (in meters)
    const std::vector<double> &z,   //!< z-positions in the NED frame (in meters)
    const std::vector<double> &eta, //!< Wave elevations at (x,y) in the NED frame (in meters)
    const double t                  //!< Current time instant (in seconds)
    ) const
{
    std::vector<double> p(x.size(), 0);

    for (size_t j = 0; j < p.size(); ++j)
    {
        if (std::isnan(z[j]))
        {
            THROW(__PRETTY_FUNCTION__, InternalErrorException, "z (value to rescale, in meters) was NaN");
        }
        if (std::isnan(eta[j]))
        {
            THROW(__PRETTY_FUNCTION__, InternalErrorException, "eta (wave height, in meters) was NaN");
        }

        if (z[j] < eta[j])
        {
            p[j] = 0;
        }
        else
        {
            const size_t n = spectrum.psi.size();
            for (size_t i = 0; i < n; ++i)
            {
                const double a = spectrum.a[i];
                const double k = spectrum.k[i];
                const double omega_t = spectrum.omega[i] * t;
                const double pdyn_fact = spectrum.pdyn_factor(k, z[j], eta[j]);
                const double k_xCosPsi_ySinPsi = k * (x[j] * spectrum.cos_psi[i] + y[j] * spectrum.sin_psi[i]);
                const double theta = spectrum.phase[i];
                p[j] += a * pdyn_fact * sin(-omega_t + k_xCosPsi_ySinPsi + theta);
            }
            p[j] *= rho * g;
        }
        
    }
    return p;
}

ssc::kinematics::Point Airy::orbital_velocity(
        const double g,   //!< gravity (in m/s^2)
        const double x,   //!< x-position in the NED frame (in meters)
        const double y,   //!< y-position in the NED frame (in meters)
        const double z,   //!< z-position in the NED frame (in meters)
        const double t,   //!< Current time instant (in seconds)
        const double eta  //!< Wave height at x,y,t (in meters)
        ) const
{
    double u = 0;
    double v = 0;
    double w = 0;
    if (z < eta) return ssc::kinematics::Point("NED", 0, 0, 0);

    const size_t n = spectrum.psi.size();
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double omega = spectrum.omega[i];
        const double k = spectrum.k[i];
        const double pdyn_factor = spectrum.pdyn_factor(k,z,0); // No stretching for the orbital velocity
        const double pdyn_factor_sh = spectrum.pdyn_factor_sh(k,z,0); // No stretching for the orbital velocity
        const double k_xCosPsi_ySinPsi = k * (x * spectrum.cos_psi[i] + y * spectrum.sin_psi[i]);
        const double theta = -omega * t + k_xCosPsi_ySinPsi + spectrum.phase[i];
        const double cos_theta = cos(theta);
        const double sin_theta = sin(theta);
        const double a_k_omega = spectrum.a[i] * k / omega;
        const double a_k_omega_pdyn_factor_sin_theta = a_k_omega * pdyn_factor * sin_theta;
        u += a_k_omega_pdyn_factor_sin_theta * spectrum.cos_psi[i];
        v += a_k_omega_pdyn_factor_sin_theta * spectrum.sin_psi[i];
        w += a_k_omega * pdyn_factor_sh * cos_theta;
    }
    return ssc::kinematics::Point("NED", u * g, v * g, w * g);
}
