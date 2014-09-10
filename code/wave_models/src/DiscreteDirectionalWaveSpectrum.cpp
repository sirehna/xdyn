/*
 * DiscreteDirectionalWaveSpectrum.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include "DiscreteDirectionalWaveSpectrum.hpp"


DiscreteDirectionalWaveDensity::DiscreteDirectionalWaveDensity() :
    sqrt_2_SiDj(0),
    omega(0),
    psi(0),
    k(0),
    phase(0)
{
}

FlatDiscreteDirectionalWaveSpectrum::FlatDiscreteDirectionalWaveSpectrum() :
    spectrum(std::vector<DiscreteDirectionalWaveDensity>()),
    domega(0),
    dpsi(0)
{

}

DiscreteDirectionalWaveSpectrum::DiscreteDirectionalWaveSpectrum() :
                    Si(std::vector<double>()),
                    Dj(std::vector<double>()),
                    omega(std::vector<double>()),
                    psi(std::vector<double>()),
                    k(std::vector<double>()),
                    phase(std::vector<std::vector<double> >()),
                    domega(1),
                    dpsi(1)

{
}

