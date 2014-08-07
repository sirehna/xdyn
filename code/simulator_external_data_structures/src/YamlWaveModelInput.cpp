/*
 * YamlWaveModelInput.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#include "YamlWaveModelInput.hpp"

YamlDiscretization::YamlDiscretization() : n(0), omega_min(0), omega_max(0){}

YamlSpectra::YamlSpectra():
model(std::string()),
model_yaml(std::string()),
directional_spreading_type(std::string()),
directional_spreading_yaml(std::string()),
spectral_density_type(std::string()),
spectral_density_yaml(std::string())
{}

YamlWaveModel::YamlWaveModel():
discretization(YamlDiscretization()),
spectra(std::vector<YamlSpectra>())
{}

