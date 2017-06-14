/*
 * YamlSpectraInput.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */
#include "YamlSpectraInput.hpp"

YamlAiry::YamlAiry() :
seed(0)
{}

YamlDiracDirectionalSpreading::YamlDiracDirectionalSpreading():
psi0(0)
{}

YamlDiracSpectralDensity::YamlDiracSpectralDensity():
omega0(0),
Hs(0)
{}

YamlPiersonMoskowitzSpectralDensity::YamlPiersonMoskowitzSpectralDensity():
Hs(0),
Tp(0)
{}
