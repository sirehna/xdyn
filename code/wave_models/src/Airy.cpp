/*
 * AiryInfiniteDepth.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#include <boost/foreach.hpp>
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "Airy.hpp"

Airy::Airy(const DiscreteDirectionalWaveSpectrum& spectrum_, const double constant_random_phase) : WaveModel(spectrum_),
phase(std::vector<std::vector<double> >()),
rng(),
generate_random_phase(boost::random::uniform_real_distribution<double>(0,2*PI))
{
    for (size_t i = 0 ; i < spectrum.omega.size() ; ++i)
    {
        std::vector<double> p;
        for (size_t j = 0 ; j < spectrum.psi.size() ; ++j)
        {
            p.push_back(constant_random_phase);

        }
        spectrum.phase.push_back(p);
    }
}


Airy::Airy(const DiscreteDirectionalWaveSpectrum& spectrum_, const int random_number_generator_seed) : WaveModel(spectrum_),
phase(std::vector<std::vector<double> >()),
rng(boost::mt19937(random_number_generator_seed)),
generate_random_phase(boost::random::uniform_real_distribution<double>(0,2*PI))
{
    for (size_t i = 0 ; i < spectrum.omega.size() ; ++i)
    {
        std::vector<double> p;
        for (size_t j = 0 ; j < spectrum.psi.size() ; ++j)
        {
            p.push_back(generate_random_phase(rng));

        }
        spectrum.phase.push_back(p);
    }
}

double Airy::elevation(const double x,                                  //!< x-position in the NED frame (in meters)
                       const double y,                                  //!< y-position in the NED frame (in meters)
                       const double t                                   //!< Current time instant (in seconds)
                       ) const
{
    double zeta = 0;
    for (size_t i = 0 ; i < spectrum.omega.size() ; ++i)
    {
        const double Ai = sqrt(spectrum.Si[i]);
        const double k = spectrum.k[i];
        const double omega = spectrum.omega[i];
        for (size_t j = 0 ; j < spectrum.psi.size() ; ++j)
        {
            const double Dj = sqrt(spectrum.Dj[j]);
            const double psi = spectrum.psi[j];
            const double theta = spectrum.phase[i][j];
            zeta += Ai*Dj*cos(omega*t - k*(x*cos(psi)+y*sin(psi)) + theta);
        }
    }
    zeta *= sqrt(2*spectrum.domega*spectrum.dpsi);
    return zeta;
}


double Airy::dynamic_pressure(const double rho, //!< water density (in kg/m^3)
                              const double g,   //!< gravity (in m/s^2)
                              const double x,   //!< x-position in the NED frame (in meters)
                              const double y,   //!< y-position in the NED frame (in meters)
                              const double z,   //!< z-position in the NED frame (in meters)
                              const double t    //!< Current time instant (in seconds)
                             ) const
{
    double p = 0;
    for (size_t i = 0 ; i < spectrum.omega.size() ; ++i)
    {
        const double Ai = sqrt(spectrum.Si[i]);
        const double k = spectrum.k[i];
        const double omega = spectrum.omega[i];
        for (size_t j = 0 ; j < spectrum.psi.size() ; ++j)
        {
            const double Dj = sqrt(spectrum.Dj[j]);
            const double psi = spectrum.psi[j];
            const double theta = spectrum.phase[i][j];
            p += Ai*Dj*spectrum.pdyn_factor(k,z)*cos(k*(x*cos(psi)+y*sin(psi))-omega*t+theta);
        }
    }
    p *= rho*g*sqrt(2*spectrum.domega*spectrum.dpsi);
    return p;
}
