#ifndef SIMHDF5WAVEOBSERVERBUILDER_HPP_
#define SIMHDF5WAVEOBSERVERBUILDER_HPP_

#include <string>
#include "H5Cpp.h"

struct H5Element
{
    H5::DataSpace dataspace;
    H5::DataSet dataset;
    H5Element(): dataspace(), dataset(){}
};

class SimHdf5WaveObserverBuilder
{
    public:
        SimHdf5WaveObserverBuilder(
            const std::string& fileName,
            const std::string& datasetName,
            const size_t nx,
            const size_t ny);

        SimHdf5WaveObserverBuilder(
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
        H5::Group group;
        std::string datasetName;
        size_t nx;
        size_t ny;
};

#endif /* SIMHDF5WAVEOBSERVERBUILDER_HPP_ */
