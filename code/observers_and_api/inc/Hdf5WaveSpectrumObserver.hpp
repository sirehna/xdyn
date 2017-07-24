#ifndef HDF5WAVESPECTRUMOBSERVER_HPP_
#define HDF5WAVESPECTRUMOBSERVER_HPP_

#include "DiscreteDirectionalWaveSpectrum.hpp"

#include <string>
#include <vector>
#include "H5Cpp.h"

void hdf5WaveSpectrumObserver(
        const H5::H5File& h5File,
        const std::string& datasetName,
        const DiscreteDirectionalWaveSpectrum& s);

void hdf5WaveSpectrumObserver(
        const std::string& fileName,
        const std::string& datasetName,
        const DiscreteDirectionalWaveSpectrum& s);

void hdf5WaveSpectrumObserver(
        const H5::H5File& h5File,
        const std::string& datasetName,
        const std::vector<DiscreteDirectionalWaveSpectrum>& s);

void hdf5WaveSpectrumObserver(
        const std::string& fileName,
        const std::string& datasetName,
        const std::vector<DiscreteDirectionalWaveSpectrum>& s);

////////////////////////////////////////////////////////////////////////////////
void hdf5WaveSpectrumObserver(
        const std::string& fileName,
        const std::string& datasetName,
        const std::vector<FlatDiscreteDirectionalWaveSpectrum2>& s);
void hdf5WaveSpectrumObserver(
        const H5::H5File& h5File,
        const std::string& datasetName,
        const std::vector<FlatDiscreteDirectionalWaveSpectrum2>& s);
void hdf5WaveSpectrumObserver(
        const H5::H5File& h5File,
        const std::string& datasetName,
        const FlatDiscreteDirectionalWaveSpectrum2& s);
void hdf5WaveSpectrumObserver(
        const std::string& fileName,
        const std::string& datasetName,
        const FlatDiscreteDirectionalWaveSpectrum2& s);

#endif
