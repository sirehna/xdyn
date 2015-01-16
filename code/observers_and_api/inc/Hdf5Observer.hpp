#ifndef HDF5OBSERVER_HPP_
#define HDF5OBSERVER_HPP_

#include <map>
#include <string>
#include <vector>
#include "H5Cpp.h"
#include "Observer.hpp"
#include "h5_element.hpp"

struct Hdf5Addressing
{
    std::string address;
    Hdf5Addressing():address(){};
    Hdf5Addressing(
            const DataAddressing& addressing,
            const std::string& basename=""
            );
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

        H5Element h5ElementT;   /**< Hdf5 dataspace and dataset for time values*/
        H5Element h5ElementX;   /**< Hdf5 dataspace and dataset for X vector values*/
        H5Element h5ElementY;   /**< Hdf5 dataspace and dataset for Y vector values*/
        H5Element h5ElementZ;   /**< Hdf5 dataspace and dataset for Z matrice values*/
        hsize_t n;              /**< Counter for wave elevation field exported. This counter is used for offset purpose*/

        void write_T(const SurfaceElevationGrid& waveElevationGrid) const;
        void write_X(const SurfaceElevationGrid& waveElevationGrid) const;
        void write_Y(const SurfaceElevationGrid& waveElevationGrid) const;
        void write_Z(const SurfaceElevationGrid& waveElevationGrid) const;
};

#endif
