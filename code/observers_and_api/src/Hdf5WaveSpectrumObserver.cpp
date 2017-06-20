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
    H5::Group group = H5_Tools::createMissingGroups(h5File, dn);
    H5_Tools::write(group,
            std::string("elevation"),
            std::string("zeta(x,y,t)=-\\sum_{i=1}^{nfreq}\\sum_{j=1}^{ndir}a_{ij}\\sin(k_i \\cdot(x \\cdot \\cos(\\psi_j)+ y \\cdot \\sin(\\psi_j)) - \\omega_i \\cdot t + \\phi_{i,j})"));
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
    H5::Group group = H5_Tools::createMissingGroups(h5File, dn);
    H5_Tools::write(group, "omega", s.omega);
    H5_Tools::write(group, "Si", s.Si);
    H5_Tools::write(group, "Dj", s.Dj);
    H5_Tools::write(group, "gamma", s.psi);
    H5_Tools::write(group, "k", s.k);
    H5_Tools::write(group, "phase", s.phase);
    H5_Tools::write(group, "amplitude",  create_amplitude_matrix(s));
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

