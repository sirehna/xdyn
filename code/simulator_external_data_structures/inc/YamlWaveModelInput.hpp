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

struct YamlWaveOutput
{
    YamlWaveOutput();
    std::string full_filename;       //!< Output file name, complete with path & file extension
    std::string format;              //!< Format of the output (eg. YAML, OBJ, HDF5...)
    std::string frame_of_reference;  //!< Name of the frame of reference the x & y coordinates are expressed in
    std::vector<double> x;           //!< x-coordinate of the points we wish to observe (in frame 'frame_of_reference')
    std::vector<double> y;           //!< y-coordinate of the points we wish to observe (in frame 'frame_of_reference')
};

struct YamlWaveModel
{
    YamlWaveModel();
    YamlDiscretization discretization; //!< Spectral discretization parameters
    std::vector<YamlSpectra> spectra;  //!< Wave spectra to generate
    YamlWaveOutput output;             //!< Defines what wave data is outputted during the simulation & how it is generated
};

#endif /* YAMLWAVEMODELINPUT_HPP_ */
