#ifndef HDF5WAVEOBSERVER_HPP_
#define HDF5WAVEOBSERVER_HPP_

#include <string>
#include <ssc/macros.hpp>
#include TR1INC(memory)
#include "H5Cpp.h"
#include "SurfaceElevationGrid.hpp"

class SimHdf5WaveObserver
{
    public:
        SimHdf5WaveObserver(
                const H5::H5File& h5File,
                const std::string& datasetName = "WaveElevation",
                const std::size_t nx = 0,
                const std::size_t ny = 0);

        SimHdf5WaveObserver(
                const std::string& fileName,
                const std::string& datasetName = "WaveElevation",
                const std::size_t nx = 0,
                const std::size_t ny = 0);

        SimHdf5WaveObserver& operator<<(const SurfaceElevationGrid& waveElevationGrid);

    private:
        SimHdf5WaveObserver(); // Disabled
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;
};

typedef TR1(shared_ptr)<SimHdf5WaveObserver> SimHdf5WaveObserverPtr;

#endif
