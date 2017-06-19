#include "Hdf5WaveSpectrumObserver.hpp"
#include "h5_tools.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

void hdf5WaveSpectrumObserver(
        const std::string& fileName,
        const std::string& datasetName,
        const std::vector<DiscreteDirectionalWaveSpectrum>& s)
{
    H5::H5File h5File = H5_Tools::openOrCreateAHdf5File(fileName);
    hdf5WaveSpectrumObserver(h5File, datasetName, s);
    h5File.close();
}

void hdf5WaveSpectrumObserver(
        const H5::H5File& h5File,
        const std::string& datasetName,
        const std::vector<DiscreteDirectionalWaveSpectrum>& s)
{
    const std::string dn(H5_Tools::ensureStringStartsAndEndsWithAPattern(datasetName,"/"));
    H5::Group group = H5_Tools::createMissingGroups(h5File, dn);
    size_t index = 0;
    for (const auto& ss: s)
    {
        std::stringstream ssindex;
        ssindex << std::setfill('0') << std::setw(2) << index;
        hdf5WaveSpectrumObserver(h5File, dn + "/" + ssindex.str(), ss);
        index++;
    }
}

void hdf5WaveSpectrumObserver(
        const H5::H5File& h5File,
        const std::string& datasetName,
        const DiscreteDirectionalWaveSpectrum& s)
{
    const std::string dn(H5_Tools::ensureStringStartsAndEndsWithAPattern(datasetName,"/"));
    H5::Group group = H5_Tools::createMissingGroups(h5File, dn);

    H5_Tools::write(group, "omega", s.omega);
    H5_Tools::write(group, "Si", s.Si);
    H5_Tools::write(group, "Dj", s.Dj);
    H5_Tools::write(group, "psi", s.psi);
    H5_Tools::write(group, "k", s.k);
    H5_Tools::write(group, "phase", s.phase);
}

void hdf5WaveSpectrumObserver(
        const std::string& fileName, const std::string& datasetName,
        const DiscreteDirectionalWaveSpectrum& s)
{
    H5::H5File h5File = H5_Tools::openOrCreateAHdf5File(fileName);
    hdf5WaveSpectrumObserver(h5File, datasetName, s);
    h5File.close();
}

