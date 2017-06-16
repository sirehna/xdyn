#include "Hdf5WaveSpectrumObserver.hpp"
#include "eigen3-hdf5.hpp"
#include "h5_tools.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

void hdf5WaveSpectrumObserver(
        const std::string& fileName,
        const std::string& datasetName,
        const SurfaceElevationFromWaves& s)
{
    H5::H5File h5File = H5_Tools::openOrCreateAHdf5File(fileName);
    hdf5WaveSpectrumObserver(h5File, datasetName, s);
    h5File.close();
}

void hdf5WaveSpectrumObserver(
        const H5::H5File& h5File,
        const std::string& datasetName,
        const SurfaceElevationFromWaves& s)
{
    const std::string dn(H5_Tools::ensureStringStartsAndEndsWithAPattern(datasetName,"/"));
    H5::Group group = H5_Tools::createMissingGroups(h5File, dn);
    size_t index = 0;
    for (const auto& ss: s.get_models())
    {
        std::stringstream ssindex;
        ssindex << std::setfill('0') << std::setw(2) << index;
        hdf5WaveSpectrumObserver(h5File, dn + "/" + ssindex.str(), ss->get_spectrum());
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

    // dataset dimensions
    hsize_t dimsf[2];
    dimsf[0] = (hsize_t)s.omega.size();
    dimsf[1] = 1;
    H5::DataSpace dataspace(2, dimsf);

    H5::DataType datatype(H5::PredType::NATIVE_DOUBLE);
    H5::DataSet dataset = group.createDataSet("omega", datatype, dataspace);
    dataset.write(&s.omega[0], H5::PredType::NATIVE_DOUBLE);
    dataset.close();
    dataspace.close();
}

void hdf5WaveSpectrumObserver(
        const std::string& fileName, const std::string& datasetName,
        const DiscreteDirectionalWaveSpectrum& s)
{
    H5::H5File h5File = H5_Tools::openOrCreateAHdf5File(fileName);
    hdf5WaveSpectrumObserver(h5File, datasetName, s);
    h5File.close();
}

