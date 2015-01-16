#ifndef HDF5OBSERVER_HPP_
#define HDF5OBSERVER_HPP_

#include <map>
#include <string>
#include <vector>
#include "H5Cpp.h"
#include "Observer.hpp"

struct Hdf5Addressing
{
    std::string address;
    Hdf5Addressing():address(){};
    Hdf5Addressing(
            const DataAddressing& addressing,
            const std::string& basename=""
            );
};

struct H5Element
{
    H5::DataSpace dataspace;
    H5::DataSet dataset;
    H5Element(): dataspace(), dataset(){}
};

class Hdf5Observer : public Observer
{
    public:
        Hdf5Observer(const std::string& filename, const std::vector<std::string>& data);

    private:
        void flush_after_initialization();
        void flush_after_write();
        void flush_value();

        using Observer::get_serializer;
        using Observer::get_initializer;

        std::function<void()> get_serializer(const double val, const DataAddressing& address);
        std::function<void()> get_initializer(const double val, const DataAddressing& address);

        std::function<void()> get_serializer(const SurfaceElevationGrid& val, const DataAddressing& address);
        std::function<void()> get_initializer(const SurfaceElevationGrid& val, const DataAddressing& address);

        H5::H5File h5File;
        std::string basename;
        std::map<std::string, std::string > name2address;
        std::map<std::string, H5::DataSet> name2dataset;
        std::map<std::string, H5::DataType> name2datatype;
        std::map<std::string, H5::DataSpace> name2dataspace;

        H5::Group group;        /**< Hdf5 group where all wave elevation data will be exported*/
        H5Element h5ElementT;   /**< Hdf5 dataspace and dataset for time values*/
        H5Element h5ElementX;   /**< Hdf5 dataspace and dataset for X vector values*/
        H5Element h5ElementY;   /**< Hdf5 dataspace and dataset for Y vector values*/
        H5Element h5ElementZ;   /**< Hdf5 dataspace and dataset for Z matrice values*/
        hsize_t n;              /**< Counter for wave elevation field exported. This counter is used for offset purpose*/

        H5Element get_h5ElementT(const size_t nx, const size_t ny) const;
        H5Element get_h5ElementX(const size_t nx, const size_t ny) const;
        H5Element get_h5ElementY(const size_t nx, const size_t ny) const;
        H5Element get_h5ElementZ(const size_t nx, const size_t ny) const;
        void write_T(const SurfaceElevationGrid& waveElevationGrid) const;
        void write_X(const SurfaceElevationGrid& waveElevationGrid) const;
        void write_Y(const SurfaceElevationGrid& waveElevationGrid) const;
        void write_Z(const SurfaceElevationGrid& waveElevationGrid) const;
};

#endif
