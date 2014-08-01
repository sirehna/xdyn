/*
 * WaveModel.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */
#include <boost/foreach.hpp>

#include "WaveModel.hpp"

WaveModel::WaveModel(const std::vector<DiscreteDirectionalWaveSpectrum>& spectra_) : spectra(std::vector<DiscreteDirectionalWaveSpectrum::Output>())
{
    BOOST_FOREACH(DiscreteDirectionalWaveSpectrum s, spectra_) spectra.push_back(s.get_spectrum());
}


WaveModel::~WaveModel()
{
}

double WaveModel::elevation(const double x,//!< x-position in the NED frame (in meters)
                            const double y,//!< y-position in the NED frame (in meters)
                            const double t //!< Current time instant (in seconds)
                           ) const
{
    double zeta = 0;
    BOOST_FOREACH(DiscreteDirectionalWaveSpectrum::Output spectrum, spectra) zeta += elevation_single_spectrum(spectrum, x, y, t);
    return zeta;
}
