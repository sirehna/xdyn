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

template <typename T>
std::vector<T> flatten(const std::vector<std::vector<T> >& v);

template <typename T>
std::vector<T> flatten(const std::vector<std::vector<T> >& v)
{
    std::size_t total_size = 0;
    for (const auto& sub : v)
        total_size += sub.size();
    std::vector<T> result;
    result.reserve(total_size);
    for (const auto& sub : v)
        result.insert(result.end(), sub.begin(), sub.end());
    return result;
}

void _writeHdf5Data(const std::string& name, const double& v, H5::Group& group);
void _writeHdf5Data(const std::string& name, const double& v, H5::Group& group)
{
    H5::DataType datatype(H5::PredType::NATIVE_DOUBLE);
    hsize_t dimsf[2];
    dimsf[0] = 1;
    dimsf[1] = 1;
    H5::DataSpace dataspace(2, dimsf);
    H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
    dataset.write(&v, H5::PredType::NATIVE_DOUBLE);
    dataset.close();
    dataspace.close();
}

void _writeHdf5Data(const std::string& name, const std::vector<double>& v, H5::Group& group);
void _writeHdf5Data(const std::string& name, const std::vector<double>& v, H5::Group& group)
{
    H5::DataType datatype(H5::PredType::NATIVE_DOUBLE);
    hsize_t dimsf[2];
    dimsf[0] = (hsize_t)v.size();
    dimsf[1] = 1;
    H5::DataSpace dataspace(2, dimsf);
    H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
    dataset.write(&v[0], H5::PredType::NATIVE_DOUBLE);
    dataset.close();
    dataspace.close();
}

void _writeHdf5Data(const std::string& name, const std::vector<std::vector<double> >& v, H5::Group& group);
void _writeHdf5Data(const std::string& name, const std::vector<std::vector<double> >& v, H5::Group& group)
{
    H5::DataType datatype(H5::PredType::NATIVE_DOUBLE);
    hsize_t dimsf[2];
    dimsf[0] = (hsize_t)v.size();
    dimsf[1] = (hsize_t)v.at(0).size();
    H5::DataSpace dataspace(2, dimsf);
    H5::DataSet dataset = group.createDataSet(name, datatype, dataspace);
    const auto vflat = flatten(v);
    dataset.write(&vflat[0], H5::PredType::NATIVE_DOUBLE);
    dataset.close();
    dataspace.close();
}

void hdf5WaveSpectrumObserver(
        const H5::H5File& h5File,
        const std::string& datasetName,
        const DiscreteDirectionalWaveSpectrum& s)
{
    const std::string dn(H5_Tools::ensureStringStartsAndEndsWithAPattern(datasetName,"/"));
    H5::Group group = H5_Tools::createMissingGroups(h5File, dn);

    _writeHdf5Data("omega", s.omega, group);
    _writeHdf5Data("Si", s.Si, group);
    _writeHdf5Data("Dj", s.Dj, group);
    _writeHdf5Data("psi", s.psi, group);
    _writeHdf5Data("k", s.k, group);
    _writeHdf5Data("phase", s.phase, group);
}

void hdf5WaveSpectrumObserver(
        const std::string& fileName, const std::string& datasetName,
        const DiscreteDirectionalWaveSpectrum& s)
{
    H5::H5File h5File = H5_Tools::openOrCreateAHdf5File(fileName);
    hdf5WaveSpectrumObserver(h5File, datasetName, s);
    h5File.close();
}

