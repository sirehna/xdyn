/*
 * DiscreteDirectionalWaveSpectrum.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include "DiscreteDirectionalWaveSpectrum.hpp"


DiscreteDirectionalWaveDensity::DiscreteDirectionalWaveDensity() :
    a(0),
    omega(0),
    psi(0),
    k(0),
    phase(0)
{
}

FlatDiscreteDirectionalWaveSpectrum::FlatDiscreteDirectionalWaveSpectrum() :
    spectrum() //,
    //domega(0),
    //dpsi(0)
{

}

FlatDiscreteDirectionalWaveSpectrum2::FlatDiscreteDirectionalWaveSpectrum2() :
                    a(),
                    omega(),
                    psi(),
                    cos_psi(),
                    sin_psi(),
                    k(),
                    phase(),
                    pdyn_factor(),
                    pdyn_factor_sh()
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

