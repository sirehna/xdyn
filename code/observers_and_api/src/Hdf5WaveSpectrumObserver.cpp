#include "Hdf5WaveSpectrumObserver.hpp"
#include "h5_tools.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

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
    H5_Tools::write(h5File, dn+"/elevation",
            std::string("eta(x,y,t)=-\\sum_{i=1}^{nfreq}\\sum_{j=1}^{ndir}a_{ij}\\sin(k_i \\cdot(x \\cdot \\cos(\\gamma_j)+ y \\cdot \\sin(\\gamma_j)) - \\omega_i \\cdot t + \\phi_{i,j})"));
    size_t index = 0;
    for (const auto& ss: s)
    {
        std::stringstream ssindex;
        ssindex << std::setfill('0') << std::setw(2) << index;
        hdf5WaveSpectrumObserver(h5File, dn + "/" + ssindex.str(), ss);
        index++;
    }
}

std::vector<std::vector<double> > create_amplitude_matrix(const DiscreteDirectionalWaveSpectrum& s);
std::vector<std::vector<double> > create_amplitude_matrix(const DiscreteDirectionalWaveSpectrum& s)
{
    const size_t nOmega = s.omega.size();
    const size_t nPsi = s.psi.size();
    const double k = sqrt(2*s.domega*s.dpsi);
    std::vector<std::vector<double> > a(nOmega);
    for (size_t i = 0 ; i < nOmega ; ++i)
    {
        std::vector<double> p(nPsi,0.0);
        const double Ai = sqrt(s.Si[i]);
        for (size_t j = 0 ; j < nPsi ; ++j)
        {
            const double Dj = sqrt(s.Dj[j]);
            p[j] = + k * Ai * Dj;
        }
        a[i] = p;
    }
    return a;
}

void hdf5WaveSpectrumObserver(
        const H5::H5File& h5File,
        const std::string& datasetName,
        const DiscreteDirectionalWaveSpectrum& s)
{
    const std::string dn(H5_Tools::ensureStringStartsAndEndsWithAPattern(datasetName,"/"));
    H5_Tools::write(h5File, dn+"/omega", s.omega);
    H5_Tools::write(h5File, dn+"/Si", s.Si);
    H5_Tools::write(h5File, dn+"/Dj", s.Dj);
    H5_Tools::write(h5File, dn+"/gamma", s.psi);
    H5_Tools::write(h5File, dn+"/k", s.k);
    H5_Tools::write(h5File, dn+"/phase", s.phase);
    H5_Tools::write(h5File, dn+"/amplitude",  create_amplitude_matrix(s));
}

void hdf5WaveSpectrumObserver(
        const std::string& fileName,
        const std::string& datasetName,
        const DiscreteDirectionalWaveSpectrum& s)
{
    H5::H5File h5File = H5_Tools::openOrCreateAHdf5File(fileName);
    hdf5WaveSpectrumObserver(h5File, datasetName, s);
    h5File.close();
}

////////////////////////////////////////////////////////////////////////////////
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

