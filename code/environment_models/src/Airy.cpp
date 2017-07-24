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


Airy::Airy(const DiscreteDirectionalWaveSpectrum& spectrum_, const double constant_random_phase) : WaveModel(flatten2(spectrum_)),
phase(std::vector<std::vector<double> >()),
rng(),
generate_random_phase(boost::random::uniform_real_distribution<double>(0,2*PI))
{
    const size_t n = spectrum.psi.size();
    spectrum.phase = std::vector<double> (n, constant_random_phase);
}

Airy::Airy(const DiscreteDirectionalWaveSpectrum& spectrum_, const int random_number_generator_seed) : WaveModel(flatten2(spectrum_)),
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

Airy::Airy(const FlatDiscreteDirectionalWaveSpectrum2& spectrum_, const double constant_random_phase) : WaveModel(spectrum_),
phase(std::vector<std::vector<double> >()),
rng(),
generate_random_phase(boost::random::uniform_real_distribution<double>(0,2*PI))
{
    const size_t n = spectrum.psi.size();
    spectrum.phase = std::vector<double> (n, constant_random_phase);
}

Airy::Airy(const FlatDiscreteDirectionalWaveSpectrum2& spectrum_, const int random_number_generator_seed) : WaveModel(spectrum_),
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
        const double x,                                         //!< x-position of the RAO's calculation point in the NED frame (in meters)
        const double y,                                         //!< y-position of the RAO's calculation point in the NED frame (in meters)
        const double t,                                         //!< Current time instant (in seconds)
        const std::vector<std::vector<double> >& rao_module,    //!< Module of the RAO (indexed by RAO[omega][psi])
        const std::vector<std::vector<double> >& rao_phase      //!< Phase of the RAO (indexed by RAO[omega][psi])
        ) const
{
    double F = 0;
    const size_t nOmega = rao_module.size();
    const size_t nPsi = rao_module.at(0).size();
    size_t index = 0;
    if (nOmega * nPsi != spectrum.k.size())
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "rao size mismatch spectrum size");
    }
    for (size_t i = 0 ; i < nOmega ; ++i)
    {
        for (size_t j = 0 ; j < nPsi ; ++j)
        {
            const double Aij = rao_module[i][j] * spectrum.a[index];
            const double omega_t = spectrum.omega[index] * t;
            const double k_xCosPsi_ySinPsi = spectrum.k[index] * (x * spectrum.cos_psi[index] + y * spectrum.sin_psi[index]);
            F += Aij * cos(omega_t - k_xCosPsi_ySinPsi + rao_phase[i][j] + spectrum.phase[index]);
            ++index;
        }
    }
    return F;
}

double Airy::elevation(
        const double x,     //!< x-position in the NED frame (in meters)
        const double y,     //!< y-position in the NED frame (in meters)
        const double t      //!< Current time instant (in seconds)
        ) const
{
    double zeta = 0;
    const size_t n = spectrum.psi.size();
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double a = spectrum.a[i];
        const double omega_t = spectrum.omega[i] * t;
        const double k_xCosPsi_ySinPsi = spectrum.k[i] * (x * spectrum.cos_psi[i] + y * spectrum.sin_psi[i]);
        const double theta = spectrum.phase[i];
        zeta -= a * sin(-omega_t + k_xCosPsi_ySinPsi + theta);
    }
    return zeta;
}

double Airy::dynamic_pressure(
        const double rho, //!< water density (in kg/m^3)
        const double g,   //!< gravity (in m/s^2)
        const double x,   //!< x-position in the NED frame (in meters)
        const double y,   //!< y-position in the NED frame (in meters)
        const double z,   //!< z-position in the NED frame (in meters)
        const double eta, //!< Wave elevation at (x,y) in the NED frame (in meters)
        const double t    //!< Current time instant (in seconds)
        ) const
{
    if (std::isnan(z))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "z (value to rescale, in meters) was NaN");
    }
    if (std::isnan(eta))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "eta (wave height, in meters) was NaN");
    }
    double p = 0;
    if (z<eta) return 0;
    const size_t n = spectrum.psi.size();
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double a = spectrum.a[i];
        const double k = spectrum.k[i];
        const double omega_t = spectrum.omega[i] * t;
        const double pdyn_fact = spectrum.pdyn_factor(k, z, eta);
        const double k_xCosPsi_ySinPsi = k * (x * spectrum.cos_psi[i] + y * spectrum.sin_psi[i]);
        const double theta = spectrum.phase[i];
        p += a * pdyn_fact * sin(-omega_t + k_xCosPsi_ySinPsi + theta);
    }
    p *= rho*g;
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
        const double theta = omega * t- k_xCosPsi_ySinPsi + spectrum.phase[i];
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
