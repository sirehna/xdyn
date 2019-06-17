/*
 * YamlWaveModelInput.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#include "YamlWaveModelInput.hpp"

YamlDiscretization::YamlDiscretization() : n(0), omega_min(0), omega_max(0), energy_fraction(0)
{}

YamlSpectra::YamlSpectra():
model(std::string()),
model_yaml(std::string()),
directional_spreading_type(std::string()),
directional_spreading_yaml(std::string()),
spectral_density_type(std::string()),
spectral_density_yaml(std::string()),
depth(0),
stretching()
{}

YamlStretching::YamlStretching():
        delta(0),
        h(0)
{}

YamlWaveModel::YamlWaveModel()
:discretization()
,spectra()
,output()
{}

YamlJonswap::YamlJonswap():
    Hs(0),
    Tp(0),
    gamma(0)
{}

YamlBretschneider::YamlBretschneider():
    Hs(0),
    Tp(0)
{}

YamlCos2s::YamlCos2s():
    psi0(0),
   s(0)
{}

YamlDiracDirection::YamlDiracDirection():
    psi0(0) //!< Primary wave direction in radians.
{}

YamlDiracSpectrum::YamlDiracSpectrum():
    omega0(0), //!< Angular frequency (\f$2\pi f\f$) in rad/s of the significant wave height
    Hs(0)     //!< Significant wave height (in meters)
{}

YamlPiersonMoskowitz::YamlPiersonMoskowitz() :
    Hs(0),     //!< Significant wave height (in meters)
    Tp(0)    //!< Mean wave period (in seconds)
{}

YamlDefaultWaveModel::YamlDefaultWaveModel():
    zwave(0),          //!< Constant free surface elevation (in meters)
    output(YamlWaveOutput())
{}
