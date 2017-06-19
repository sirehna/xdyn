#ifndef HDF5WAVEOBSERVERBUILDER_HPP_
#define HDF5WAVEOBSERVERBUILDER_HPP_

#include <string>
#include "H5Cpp.h"
#include "h5_element.hpp"

class Hdf5WaveObserverBuilder
{
    public:
        Hdf5WaveObserverBuilder(
            const std::string& fileName,
            const std::string& datasetName,
            const size_t nx,
            const size_t ny);

        Hdf5WaveObserverBuilder(
            const H5::H5File& h5File,
            const std::string& datasetName,
            const size_t nx,
            const size_t ny);

        H5::H5File get_h5File() const;
        H5::Group get_group() const;
        H5Element get_h5ElementT() const;
        H5Element get_h5ElementX() const;
        H5Element get_h5ElementY() const;
        H5Element get_h5ElementZ() const;
        size_t get_nx() const {return nx;};
        size_t get_ny() const {return ny;};
    private:
        H5::H5File h5File;
        std::string datasetName;
        H5::Group group;
        size_t nx;
        size_t ny;
};

#endif
