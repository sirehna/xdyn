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
depth(0)
{}

YamlWaveOutput::YamlWaveOutput() : frame_of_reference(""),
         xmin(0),
             xmax(0),
             nx(0),
             ymin(0),
             ymax(0),
             ny(0)
{}

YamlWaveModel::YamlWaveModel():
discretization(YamlDiscretization()),
spectra(std::vector<YamlSpectra>()),
output(YamlWaveOutput()),
compute_froude_krylov(false)
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
