/*
 * WaveModel.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#include "WaveModel.hpp"
#include "InternalErrorException.hpp"
#include <cmath> // For isnan

WaveModel::WaveModel(const FlatDiscreteDirectionalWaveSpectrum& spectrum_) : spectrum(spectrum_)
{
    if (spectrum.omega.empty())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "No 'omega' values devined in DiscreteDirectionalWaveSpectrum");
    }
    if (spectrum.a.empty())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "No 'a' values devined in DiscreteDirectionalWaveSpectrum");
    }
    if (spectrum.k.empty())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "No 'k' values devined in DiscreteDirectionalWaveSpectrum");
    }

    for (const auto omega:spectrum.omega)
    {
        if (std::isnan(omega))
        {
            THROW(__PRETTY_FUNCTION__, InternalErrorException, "DiscreteDirectionalWaveSpectrum contains NaN values for omega: " << spectrum.omega);
        }
    }

    for (const auto k:spectrum.k)
    {
        if (std::isnan(k))
        {
            THROW(__PRETTY_FUNCTION__, InternalErrorException, "DiscreteDirectionalWaveSpectrum contains NaN values for k: " << spectrum.k);
        }
    }
    for (const auto Si:spectrum.a)
    {
        if (std::isnan(Si))
        {
            THROW(__PRETTY_FUNCTION__, InternalErrorException, "DiscreteDirectionalWaveSpectrum contains NaN values for a: " << spectrum.a);
        }
    }
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
