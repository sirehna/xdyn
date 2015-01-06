#include "SimHdf5WaveObserver.hpp"

#include <vector>
#include <memory>
#include "H5Cpp.h"
#include "eigen3-hdf5.hpp"

class SimHdf5WaveObserver::Impl
{
    public:
        void write(const WaveElevationGrid& waveElevationGrid);
        Impl():
            h5File(),
            group(),
            dataspaceT(),
            datasetT(),
            dataspaceX(),
            datasetX(),
            dataspaceY(),
            datasetY(),
            dataspaceZ(),
            datasetZ(),
            n(0){}
    private:
        H5::H5File h5File;
        H5::Group group;
        H5::DataSpace dataspaceT;
        H5::DataSet datasetT;
        H5::DataSpace dataspaceX;
        H5::DataSet datasetX;
        H5::DataSpace dataspaceY;
        H5::DataSet datasetY;
        H5::DataSpace dataspaceZ;
        H5::DataSet datasetZ;
        hsize_t n;                      /**< Counter*/

        void write_T(const WaveElevationGrid& waveElevationGrid) const;
        void write_X(const WaveElevationGrid& waveElevationGrid) const;
        void write_Y(const WaveElevationGrid& waveElevationGrid) const;
        void write_Z(const WaveElevationGrid& waveElevationGrid) const;
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
        TR1(shared_ptr)<SimHdf5WaveObserver::Impl> build();
    private:
        H5::H5File h5File;
        std::string datasetName;
        size_t nx;
        size_t ny;
        void create_dataset_T(SimHdf5WaveObserver::Impl& obs);
        void create_dataset_X(SimHdf5WaveObserver::Impl& obs, const size_t nx);
        void create_dataset_Y(SimHdf5WaveObserver::Impl& obs, const size_t ny);
        void create_dataset_Z(SimHdf5WaveObserver::Impl& obs, const size_t nx, const size_t ny);
};

SimHdf5WaveObserverBuilder::SimHdf5WaveObserverBuilder(
    const std::string& fileName,
    const std::string& datasetName_,
    const size_t nx_,
    const size_t ny_):h5File(H5::H5File(fileName, H5F_ACC_TRUNC)),datasetName(datasetName_),nx(nx_),ny(ny_)
{
}

SimHdf5WaveObserverBuilder::SimHdf5WaveObserverBuilder(
    const H5::H5File& h5File_,
    const std::string& datasetName_,
    const size_t nx_,
    const size_t ny_):h5File(h5File_),datasetName(datasetName_),nx(nx_),ny(ny_)
{
}

TR1(shared_ptr)<SimHdf5WaveObserver::Impl> SimHdf5WaveObserverBuilder::build()
{
    TR1(shared_ptr)<SimHdf5WaveObserver::Impl> obs(new SimHdf5WaveObserver::Impl);
    obs->h5File.setId(h5File.getLocId());
    obs->group = obs->h5File.createGroup("/"+datasetName);
    create_dataset_T(*obs);
    create_dataset_X(*obs,nx);
    create_dataset_Y(*obs,ny);
    create_dataset_Z(*obs,nx,ny);
    return obs;
}

void SimHdf5WaveObserverBuilder::create_dataset_T(SimHdf5WaveObserver::Impl& obs)
{
    hsize_t dimsT[1] = {1};
    const hsize_t maxdimsT[1] = {H5S_UNLIMITED};
    const hsize_t chunk_dims1[1] = {1};
    H5::DSetCreatPropList cparms1;
    cparms1.setChunk(1, chunk_dims1);
    obs.dataspaceT = H5::DataSpace(1, dimsT, maxdimsT);
    obs.datasetT = obs.group.createDataSet("/"+datasetName+"/t",H5::PredType::NATIVE_DOUBLE, obs.dataspaceT, cparms1);
}

void SimHdf5WaveObserverBuilder::create_dataset_X(
        SimHdf5WaveObserver::Impl& obs,
        const size_t nx)
{
    hsize_t dimsX[2] = {1, 1};
    hsize_t maxdimsX[2] = {H5S_UNLIMITED, H5S_UNLIMITED};
    const hsize_t chunk_dims2[2] = {1,10};
    H5::DSetCreatPropList cparms2;
    cparms2.setChunk(2, chunk_dims2);
    dimsX[1] = (hsize_t)nx;
    maxdimsX[1] = (hsize_t)nx;
    obs.dataspaceX = H5::DataSpace(2, dimsX, maxdimsX);
    obs.datasetX = obs.group.createDataSet("/"+datasetName+"/x",H5::PredType::NATIVE_DOUBLE, obs.dataspaceX, cparms2);
}

void SimHdf5WaveObserverBuilder::create_dataset_Y(
        SimHdf5WaveObserver::Impl& obs,
        const size_t ny)
{
    hsize_t dimsY[2] = {1, 1};
    hsize_t maxdimsY[2] = {H5S_UNLIMITED, H5S_UNLIMITED};
    const hsize_t chunk_dims2[2] = {1,10};
    H5::DSetCreatPropList cparms2;
    cparms2.setChunk(2, chunk_dims2);
    dimsY[1] = (hsize_t)ny;
    maxdimsY[1] = (hsize_t)ny;
    obs.dataspaceY = H5::DataSpace(2, dimsY, maxdimsY);
    obs.datasetY = obs.group.createDataSet("/"+datasetName+"/y",H5::PredType::NATIVE_DOUBLE, obs.dataspaceY, cparms2);
}

void SimHdf5WaveObserverBuilder::create_dataset_Z(
        SimHdf5WaveObserver::Impl& obs,
        const size_t nx, const size_t ny)
{
    hsize_t dimsZ[3] = {1, 1, 1};
    hsize_t maxdimsZ[3] = {H5S_UNLIMITED, H5S_UNLIMITED, H5S_UNLIMITED};
    const hsize_t chunk_dims3[3] = {20,10,1};
    H5::DSetCreatPropList cparms3;
    cparms3.setChunk(3, chunk_dims3);
    dimsZ[0] = (hsize_t)nx;
    maxdimsZ[0] = (hsize_t)nx;
    dimsZ[1] = (hsize_t)ny;
    maxdimsZ[1] = (hsize_t)ny;
    obs.dataspaceZ = H5::DataSpace(3, dimsZ, maxdimsZ);
    obs.datasetZ = obs.group.createDataSet("/"+datasetName+"/z",H5::PredType::NATIVE_DOUBLE, obs.dataspaceZ, cparms3);
}

void SimHdf5WaveObserver::Impl::write_T(const WaveElevationGrid& waveElevationGrid) const
{
    const hsize_t nt = n+1;
    hsize_t dims1[1] = {1};
    hsize_t offsetT[1] = {0};
    hsize_t sizeT[1] = {0};
    offsetT[0] = n;
    sizeT[0] = nt;
    dims1[0] = (hsize_t)1;
    datasetT.extend(sizeT);
    H5::DataSpace fspaceT = datasetT.getSpace();
    fspaceT.selectHyperslab(H5S_SELECT_SET, dims1, offsetT);
    if (n==0)
    {
        dataspaceT.setExtentSimple(1, sizeT, sizeT);
    }
    datasetT.write(&waveElevationGrid.t, H5::PredType::NATIVE_DOUBLE, dataspaceT, fspaceT);
}

void SimHdf5WaveObserver::Impl::write_X(const WaveElevationGrid& waveElevationGrid) const
{
    const hsize_t nt = n+1;
    hsize_t dims2[2] = {1, 1};
    hsize_t offsetX[2] = {0,0};
    hsize_t sizeX[2] = {0,0};
    offsetX[0] = n;
    offsetX[1] = (hsize_t)0;
    sizeX[0] = nt;
    sizeX[1] = (hsize_t)waveElevationGrid.x.size();
    dims2[0] = (hsize_t)1;
    dims2[1] = (hsize_t)waveElevationGrid.x.size();
    datasetX.extend(sizeX);
    H5::DataSpace fspaceX = datasetX.getSpace();
    fspaceX.selectHyperslab(H5S_SELECT_SET, dims2, offsetX);
    if (n==0)
    {
        dataspaceX.setExtentSimple(2,sizeX,sizeX);
    }
    datasetX.write(waveElevationGrid.x.data(), H5::PredType::NATIVE_DOUBLE, dataspaceX, fspaceX);
}

void SimHdf5WaveObserver::Impl::write_Y(const WaveElevationGrid& waveElevationGrid) const
{
    const hsize_t nt = n+1;
    hsize_t dims2[2] = {1, 1};
    hsize_t offsetY[2];
    hsize_t sizeY[2];
    offsetY[0] = n;
    offsetY[1] = (hsize_t)0;
    sizeY[0] = nt;
    sizeY[1] = (hsize_t)waveElevationGrid.y.size();
    dims2[0] = (hsize_t)1;
    dims2[1] = (hsize_t)waveElevationGrid.y.size();
    datasetY.extend(sizeY);
    H5::DataSpace fspaceY = datasetY.getSpace();
    fspaceY.selectHyperslab(H5S_SELECT_SET, dims2, offsetY);
    if (n==0)
    {
        dataspaceY.setExtentSimple(2, sizeY, sizeY);
    }
    datasetY.write(waveElevationGrid.y.data(), H5::PredType::NATIVE_DOUBLE, dataspaceY, fspaceY);
}

void SimHdf5WaveObserver::Impl::write_Z(const WaveElevationGrid& waveElevationGrid) const
{
    const hsize_t nt = n+1;
    hsize_t dims3[3] = {1, 1, 1};
    hsize_t offsetZ[3];
    offsetZ[0] = 0; offsetZ[1] = 0; offsetZ[2] = n;
    hsize_t sizeZ[3];
    sizeZ[0] = static_cast<hsize_t>(waveElevationGrid.z.rows());
    sizeZ[1] = static_cast<hsize_t>(waveElevationGrid.z.cols());
    sizeZ[2] = nt;
    dims3[0] = sizeZ[0];
    dims3[1] = sizeZ[1];
    dims3[2] = (hsize_t)1;
    datasetZ.extend(sizeZ);
    H5::DataSpace fspaceZ = datasetZ.getSpace();
    fspaceZ.selectHyperslab(H5S_SELECT_SET, dims3, offsetZ);
    if (n==0)
    {
        dataspaceZ.setExtentSimple(3,sizeZ,sizeZ);
    }
    if (!waveElevationGrid.z.IsRowMajor)
    {
        const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> row_major_mat(waveElevationGrid.z);
        datasetZ.write(row_major_mat.data(), H5::PredType::NATIVE_DOUBLE, dataspaceZ, fspaceZ);
    }
    else
    {
        datasetZ.write(waveElevationGrid.z.data(), H5::PredType::NATIVE_DOUBLE, dataspaceZ, fspaceZ);
    }
}

void SimHdf5WaveObserver::Impl::write(const WaveElevationGrid& waveElevationGrid)
{
    write_T(waveElevationGrid);
    write_X(waveElevationGrid);
    write_Y(waveElevationGrid);
    write_Z(waveElevationGrid);
    n++;
}

/*
SimHdf5WaveObserver::SimHdf5WaveObserver(
        const H5::H5File& h5File, const std::string& datasetName, const size_t nx, const size_t ny);
SimHdf5WaveObserver::SimHdf5WaveObserver(
        const H5::H5File& h5File, const std::string& datasetName, const size_t nx, const size_t ny):
                pimpl(SimHdf5WaveObserverBuilder(h5File, datasetName, nx, ny).build())
{}
*/
SimHdf5WaveObserver::SimHdf5WaveObserver(
        const std::string& fileName, const std::string& datasetName, const size_t nx, const size_t ny):
                pimpl(SimHdf5WaveObserverBuilder(fileName, datasetName, nx, ny).build())
{}

SimHdf5WaveObserver& SimHdf5WaveObserver::operator<<(const WaveElevationGrid& waveElevationGrid)
{
    this->pimpl->write(waveElevationGrid);
    return *this;
}
