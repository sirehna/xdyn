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

double Airy::evaluate_rao(
        const double x,                           //!< x-position of the RAO's calculation point in the NED frame (in meters)
        const double y,                           //!< y-position of the RAO's calculation point in the NED frame (in meters)
        const double t,                           //!< Current time instant (in seconds)
        const std::vector<double>& rao_module_for_each_frequency_and_incidence,    //!< "Flattened" matrix containing the module of the RAO for each angular frequency omega and each wave incidence beta
        const std::vector<double>& rao_phase_for_each_frequency_and_incidence      //!< "Flattened" matrix containing the phase of the RAO for each angular frequency omega and each wave incidence beta
        ) const
{
    double F = 0;
    const size_t nb_of_omegas_x_nb_of_directions = rao_module_for_each_frequency_and_incidence.size();
    if (nb_of_omegas_x_nb_of_directions != spectrum.k.size())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Number of angular frequencies times number of incidences in HDB RAO is " << nb_of_omegas_x_nb_of_directions << ", which does not match spectrum size (" << spectrum.k.size() << " (omega,psi) pairs)");
    }
    for (size_t i = 0 ; i < nb_of_omegas_x_nb_of_directions ; ++i) // For each (omega,beta) pair
    {
        const double rao_amplitude = rao_module_for_each_frequency_and_incidence[i] * spectrum.a[i];
        const double omega_t = spectrum.omega[i] * t;
        const double k_xCosPsi_ySinPsi = spectrum.k[i] * (x * spectrum.cos_psi[i] + y * spectrum.sin_psi[i]);
        const double theta = spectrum.phase[i];
        F -= rao_amplitude * sin(-omega_t + k_xCosPsi_ySinPsi + theta + rao_phase_for_each_frequency_and_incidence[i]);
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

ssc::kinematics::PointMatrix Airy::orbital_velocity(
        const double g,                //!< gravity (in m/s^2)
        const std::vector<double>& x,  //!< x-positions in the NED frame (in meters)
        const std::vector<double>& y,  //!< y-positions in the NED frame (in meters)
        const std::vector<double>& z,  //!< z-positions in the NED frame (in meters)
        const double t,                //!< Current time instant (in seconds)
        const std::vector<double>& eta //!< Wave heights at x,y,t (in meters)
        ) const
{
    ssc::kinematics::PointMatrix M("NED", x.size());
    for (size_t point_index = 0; point_index < x.size(); ++point_index) {

        if (z.at(point_index) < eta.at(point_index)) 
        {
            M.m(0, point_index) = 0;
            M.m(1, point_index) = 0;
            M.m(2, point_index) = 0;
        } else {
            const size_t n = spectrum.psi.size();
            double u = 0;
            double v = 0;
            double w = 0;
            for (size_t i = 0 ; i < n ; ++i)
            {
                const double omega = spectrum.omega[i];
                const double k = spectrum.k[i];
                const double pdyn_factor = spectrum.pdyn_factor(k,z[point_index],0); // No stretching for the orbital velocity
                const double pdyn_factor_sh = spectrum.pdyn_factor_sh(k,z[point_index],0); // No stretching for the orbital velocity
                const double k_xCosPsi_ySinPsi = k * (x[point_index] * spectrum.cos_psi[i] + y[point_index] * spectrum.sin_psi[i]);
                const double theta = -omega * t + k_xCosPsi_ySinPsi + spectrum.phase[i];
                const double cos_theta = cos(theta);
                const double sin_theta = sin(theta);
                const double a_k_omega = spectrum.a[i] * k / omega;
                const double a_k_omega_pdyn_factor_sin_theta = a_k_omega * pdyn_factor * sin_theta;
                u += a_k_omega_pdyn_factor_sin_theta * spectrum.cos_psi[i];
                v += a_k_omega_pdyn_factor_sin_theta * spectrum.sin_psi[i];
                w += a_k_omega * pdyn_factor_sh * cos_theta;
            }
            M.m(0, point_index) = u * g;
            M.m(1, point_index) = v * g;
            M.m(2, point_index) = w * g;
        }
    }
    return M;

}
