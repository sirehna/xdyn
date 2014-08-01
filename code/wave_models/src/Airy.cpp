/*
 * AiryInfiniteDepth.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#include <boost/foreach.hpp>

#include "Airy.hpp"

Airy::Airy(const std::vector<DiscreteDirectionalWaveSpectrum>& spectra_, const int random_number_generator_seed) : WaveModel(spectra_),
phase(std::vector<std::vector<double> >()),
rng(boost::mt19937(random_number_generator_seed)),
generate_random_phase(boost::uniform_01<double>())

{
    BOOST_FOREACH(DiscreteDirectionalWaveSpectrum& spectrum, spectra)
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
}

double Airy::elevation_single_spectrum(const DiscreteDirectionalWaveSpectrum& spectrum, //!< Discrete spectrum under consideration
                                       const double x,                                  //!< x-position in the NED frame (in meters)
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
            zeta += Ai*Dj*cos(k*(x*cos(psi)+y*sin(psi))-omega*t+theta);
        }
    }
    zeta *= sqrt(2*spectrum.domega*spectrum.dpsi);
    return zeta;
}
