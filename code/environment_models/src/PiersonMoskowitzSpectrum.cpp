/*
 * PiersonMoskowitzSpectrum.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: cady
 */

#include "PiersonMoskowitzSpectrum.hpp"

PiersonMoskowitzSpectrum::PiersonMoskowitzSpectrum(const double Hs_, const double Tp_) : JonswapSpectrum(Hs_, Tp_, 1)
{
}

WaveSpectralDensity* PiersonMoskowitzSpectrum::clone() const
{
    return new PiersonMoskowitzSpectrum(*this);
}
