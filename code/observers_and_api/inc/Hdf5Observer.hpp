#ifndef HDF5OBSERVER_HPP_
#define HDF5OBSERVER_HPP_

#include "Observer.hpp"
#include "DiscreteDirectionalWaveSpectrum.hpp"
#include <ssc/macros.hpp>
#include TR1INC(memory)
#include "H5Cpp.h"

struct Hdf5Addressing
{
    std::string address;
    Hdf5Addressing():address(){};
    Hdf5Addressing(
            const DataAddressing& addressing,
            const std::string& basename=""
            );
};

class SimHdf5WaveObserver;
class Hdf5Observer : public Observer
{
    public:
        Hdf5Observer(const std::string& filename, const std::vector<std::string>& data);
        void write_before_simulation(const std::vector<DiscreteDirectionalWaveSpectrum>& val, const DataAddressing& address);
    private:
        void flush_after_initialization();
        void flush_after_write();
        void flush_value_during_write();

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

        TR1(shared_ptr)<SimHdf5WaveObserver> wave_serializer;
};

#endif
