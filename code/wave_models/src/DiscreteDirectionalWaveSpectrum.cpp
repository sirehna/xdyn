/*
 * DiscreteDirectionalWaveSpectrum.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include "DiscreteDirectionalWaveSpectrum.hpp"

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

