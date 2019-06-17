#include "Hdf5WaveSpectrumObserver.hpp"
#include "h5_tools.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

void hdf5WaveSpectrumObserver(
        const std::string& fileName,
        const std::string& datasetName,
        const std::vector<FlatDiscreteDirectionalWaveSpectrum>& s)
{
    H5::H5File h5File = H5_Tools::openOrCreateAHdf5File(fileName);
    hdf5WaveSpectrumObserver(h5File, datasetName, s);
    h5File.close();
}

void hdf5WaveSpectrumObserver(
        const H5::H5File& h5File,
        const std::string& datasetName,
        const std::vector<FlatDiscreteDirectionalWaveSpectrum>& s)
{
    const std::string dn(H5_Tools::ensureStringStartsAndEndsWithAPattern(datasetName,"/"));
    H5_Tools::write(h5File, dn+"/elevation",
            std::string("eta(x,y,t)=-\\sum_{i=1}^{nfreq}a_{i}\\sin(k_i \\cdot(x \\cdot \\cos(\\gamma_i)+ y \\cdot \\sin(\\gamma_i)) - \\omega_i \\cdot t + \\phi_{i})"));
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
        const FlatDiscreteDirectionalWaveSpectrum& s)
{
    const std::string dn(H5_Tools::ensureStringStartsAndEndsWithAPattern(datasetName,"/"));
    H5_Tools::write(h5File, dn+"/omega", s.omega);
    H5_Tools::write(h5File, dn+"/a", s.a);
    H5_Tools::write(h5File, dn+"/gamma", s.psi);
    H5_Tools::write(h5File, dn+"/k", s.k);
    H5_Tools::write(h5File, dn+"/phase", s.phase);
}

void hdf5WaveSpectrumObserver(
        const std::string& fileName,
        const std::string& datasetName,
        const FlatDiscreteDirectionalWaveSpectrum& s)
{
    H5::H5File h5File = H5_Tools::openOrCreateAHdf5File(fileName);
    hdf5WaveSpectrumObserver(h5File, datasetName, s);
    h5File.close();
}

