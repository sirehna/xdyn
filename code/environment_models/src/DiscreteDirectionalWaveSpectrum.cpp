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
    spectrum(),
    domega(0),
    dpsi(0)
{

}

DiscreteDirectionalWaveSpectrum::DiscreteDirectionalWaveSpectrum() :
                    Si(),
                    Dj(),
                    omega(),
                    psi(),
                    k(),
                    phase(),
                    domega(1),
                    dpsi(1),
                    pdyn_factor(),
                    pdyn_factor_sh()
{
}

