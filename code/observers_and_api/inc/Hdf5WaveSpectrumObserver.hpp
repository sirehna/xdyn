#ifndef HDF5WAVESPECTRUMOBSERVER_HPP_
#define HDF5WAVESPECTRUMOBSERVER_HPP_

#include "DiscreteDirectionalWaveSpectrum.hpp"

#include <string>
#include <vector>
#include "H5Cpp.h"

void hdf5WaveSpectrumObserver(
        const std::string& fileName,
        const std::string& datasetName,
        const std::vector<FlatDiscreteDirectionalWaveSpectrum>& s);
void hdf5WaveSpectrumObserver(
        const H5::H5File& h5File,
        const std::string& datasetName,
        const std::vector<FlatDiscreteDirectionalWaveSpectrum>& s);
void hdf5WaveSpectrumObserver(
        const H5::H5File& h5File,
        const std::string& datasetName,
        const FlatDiscreteDirectionalWaveSpectrum& s);
void hdf5WaveSpectrumObserver(
        const std::string& fileName,
        const std::string& datasetName,
        const FlatDiscreteDirectionalWaveSpectrum& s);

#endif
