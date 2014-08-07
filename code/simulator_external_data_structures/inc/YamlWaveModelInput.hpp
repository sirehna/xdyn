/*
 * YamlWaveModelInput.hpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#ifndef YAMLWAVEMODELINPUT_HPP_
#define YAMLWAVEMODELINPUT_HPP_

#include <cstdlib> // For size_t
#include <string>
#include <vector>

struct YamlDiscretization
{
    YamlDiscretization();
    size_t n;         //!< Nb of frequencies in the discretization
    double omega_min; //!< First angular frequency (in rad/s)
    double omega_max; //!< Last angular frequency (in rad/s)
};

struct YamlSpectra
{
    YamlSpectra();
    std::string model;                      //!< Wave model (eg. airy)
    std::string model_yaml;                 //!< Model data in YAML format
    std::string directional_spreading_type; //!< Spreading type (eg. cos2s)
    std::string directional_spreading_yaml; //!< Model data in YAML format
    std::string spectral_density_type;      //!< Type of spectrum (eg. jonswap)
    std::string spectral_density_yaml;      //!< Spectral model parameters in YAML format
};

struct YamlWaveModel
{
    YamlWaveModel();
    YamlDiscretization discretization; //!< Spectral discretization parameters
    std::vector<YamlSpectra> spectra;  //!< Wave spectra to generate
};

#endif /* YAMLWAVEMODELINPUT_HPP_ */
