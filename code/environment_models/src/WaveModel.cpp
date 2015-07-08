/*
 * WaveModel.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#include "WaveModel.hpp"

WaveModel::WaveModel(const DiscreteDirectionalWaveSpectrum& spectrum_) : spectrum(spectrum_)
{
}

WaveModel::~WaveModel()
{
}

std::vector<double> WaveModel::get_omegas() const
{
    return spectrum.omega;
}

std::vector<double> WaveModel::get_psis() const
{
    return spectrum.psi;
}
