#ifndef HDF5OBSERVER_HPP_
#define HDF5OBSERVER_HPP_

#include <map>
#include <string>
#include <vector>
#include "H5Cpp.h"
#include "Observer.hpp"

struct Hdf5Addressing
{
    std::string name;
    std::string address;
    Hdf5Addressing():name(),address(){};
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
        std::map<std::string, std::vector<Hdf5Addressing> > address2columns;
        std::map<std::string,H5::DataSet> address2dataset;
        std::map<std::string,H5::DataType> name2datatype;
};

#endif
