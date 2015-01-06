#ifndef SIMHDF5WAVEOBSERVER_HPP_
#define SIMHDF5WAVEOBSERVER_HPP_

#include <ssc/macros.hpp>
#include TR1INC(memory)
#include <ssc/kinematics.hpp>

#include <string>

#include <Eigen/Core>

struct WaveElevationGrid
{
    double t;
    Eigen::VectorXd x;
    Eigen::VectorXd y;
    Eigen::MatrixXd z;
    WaveElevationGrid():t(0.0),x(),y(),z(){};
};

class SimHdf5WaveObserver
{
    public:
        SimHdf5WaveObserver(
                const std::string& fileName,
                const std::string& datasetName = "WaveElevation",
                const size_t nx = 0,
                const size_t ny = 0);

        SimHdf5WaveObserver& operator<<(const WaveElevationGrid& waveElevationGrid);

    private:
        SimHdf5WaveObserver(); // Disabled
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;
};

#endif /* SIMHDF5WAVEOBSERVER_HPP_ */
