#include "Hdf5WaveObserver.hpp"
#include "Hdf5WaveObserverBuilder.hpp"
#include <vector>
#include "eigen3-hdf5.hpp"

class Hdf5WaveObserver::Impl
{
    public:
        Impl(const Hdf5WaveObserverBuilder& builder):
            h5File(builder.get_h5File()),
            group(builder.get_group()),
            h5ElementT(builder.get_h5ElementT()),
            h5ElementX(builder.get_h5ElementX()),
            h5ElementY(builder.get_h5ElementY()),
            h5ElementZ(builder.get_h5ElementZ()),
            n((hsize_t)0){}
        void write(const SurfaceElevationGrid& waveElevationGrid);
    private:
        H5::H5File h5File;      /**< Hdf5 file pointer*/
        H5::Group group;        /**< Hdf5 group where all wave elevation data will be exported*/
        H5Element h5ElementT;   /**< Hdf5 dataspace and dataset for time values*/
        H5Element h5ElementX;   /**< Hdf5 dataspace and dataset for X vector values*/
        H5Element h5ElementY;   /**< Hdf5 dataspace and dataset for Y vector values*/
        H5Element h5ElementZ;   /**< Hdf5 dataspace and dataset for Z matrice values*/
        hsize_t n;              /**< Counter for wave elevation field exported. This counter is used for offset purpose*/

        Impl();
        void write_T(const SurfaceElevationGrid& waveElevationGrid) const;
        void write_X(const SurfaceElevationGrid& waveElevationGrid) const;
        void write_Y(const SurfaceElevationGrid& waveElevationGrid) const;
        void write_Z(const SurfaceElevationGrid& waveElevationGrid) const;
};

void Hdf5WaveObserver::Impl::write_T(const SurfaceElevationGrid& waveElevationGrid) const
{
    const hsize_t nt = n+1;
    hsize_t dims1[1] = {1};
    hsize_t offsetT[1] = {0};
    hsize_t sizeT[1] = {0};
    offsetT[0] = n;
    sizeT[0] = nt;
    dims1[0] = (hsize_t)1;
    h5ElementT.dataset.extend(sizeT);
    H5::DataSpace fspaceT = h5ElementT.dataset.getSpace();
    fspaceT.selectHyperslab(H5S_SELECT_SET, dims1, offsetT);
    if (n==0)
    {
        h5ElementT.dataspace.setExtentSimple(1, sizeT, sizeT);
    }
    h5ElementT.dataset.write(&waveElevationGrid.t, H5::PredType::NATIVE_DOUBLE, h5ElementT.dataspace, fspaceT);
}

void Hdf5WaveObserver::Impl::write_X(const SurfaceElevationGrid& waveElevationGrid) const
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
    h5ElementX.dataset.extend(sizeX);
    H5::DataSpace fspaceX = h5ElementX.dataset.getSpace();
    fspaceX.selectHyperslab(H5S_SELECT_SET, dims2, offsetX);
    if (n==0)
    {
        h5ElementX.dataspace.setExtentSimple(2,sizeX,sizeX);
    }
    h5ElementX.dataset.write(waveElevationGrid.x.data(), H5::PredType::NATIVE_DOUBLE, h5ElementX.dataspace, fspaceX);
}

void Hdf5WaveObserver::Impl::write_Y(const SurfaceElevationGrid& waveElevationGrid) const
{
    const hsize_t nt = n+1;
    hsize_t dims2[2] = {1, 1};
    hsize_t offsetY[2] = {0,0};
    hsize_t sizeY[2] = {0,0};
    offsetY[0] = n;
    offsetY[1] = (hsize_t)0;
    sizeY[0] = nt;
    sizeY[1] = (hsize_t)waveElevationGrid.y.size();
    dims2[0] = (hsize_t)1;
    dims2[1] = (hsize_t)waveElevationGrid.y.size();
    h5ElementY.dataset.extend(sizeY);
    H5::DataSpace fspaceY = h5ElementY.dataset.getSpace();
    fspaceY.selectHyperslab(H5S_SELECT_SET, dims2, offsetY);
    if (n==0)
    {
        h5ElementY.dataspace.setExtentSimple(2, sizeY, sizeY);
    }
    h5ElementY.dataset.write(waveElevationGrid.y.data(), H5::PredType::NATIVE_DOUBLE, h5ElementY.dataspace, fspaceY);
}

void Hdf5WaveObserver::Impl::write_Z(const SurfaceElevationGrid& waveElevationGrid) const
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
    h5ElementZ.dataset.extend(sizeZ);
    H5::DataSpace fspaceZ = h5ElementZ.dataset.getSpace();
    fspaceZ.selectHyperslab(H5S_SELECT_SET, dims3, offsetZ);
    if (n==0)
    {
        h5ElementZ.dataspace.setExtentSimple(3,sizeZ,sizeZ);
    }
    if (not(waveElevationGrid.z.IsRowMajor))
    {
        const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> row_major_mat(waveElevationGrid.z);
        h5ElementZ.dataset.write(row_major_mat.data(), H5::PredType::NATIVE_DOUBLE, h5ElementZ.dataspace, fspaceZ);
    }
    else
    {
        h5ElementZ.dataset.write(waveElevationGrid.z.data(), H5::PredType::NATIVE_DOUBLE, h5ElementZ.dataspace, fspaceZ);
    }
}

void Hdf5WaveObserver::Impl::write(const SurfaceElevationGrid& waveElevationGrid)
{
    write_T(waveElevationGrid);
    write_X(waveElevationGrid);
    write_Y(waveElevationGrid);
    write_Z(waveElevationGrid);
    n++;
}

Hdf5WaveObserver::Hdf5WaveObserver(
        const H5::H5File& h5File, const std::string& datasetName, const size_t nx, const size_t ny):
                pimpl(TR1(shared_ptr)<Hdf5WaveObserver::Impl>(new Hdf5WaveObserver::Impl(Hdf5WaveObserverBuilder(h5File, datasetName, nx, ny))))
{
}

Hdf5WaveObserver::Hdf5WaveObserver(
        const std::string& fileName, const std::string& datasetName,
        const std::size_t nx, const std::size_t ny):
                pimpl(TR1(shared_ptr)<Hdf5WaveObserver::Impl>(new Hdf5WaveObserver::Impl(Hdf5WaveObserverBuilder(fileName, datasetName, nx, ny))))
{
}

Hdf5WaveObserver& Hdf5WaveObserver::operator<<(const SurfaceElevationGrid& waveElevationGrid)
{
    this->pimpl->write(waveElevationGrid);
    return *this;
}
