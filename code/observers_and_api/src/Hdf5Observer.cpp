#include "h5_version.hpp"
#include "h5_tools.hpp"
#include "Hdf5Observer.hpp"

#include "demoMatLab.hpp"
#include "demoPython.hpp"

#include <ssc/exception_handling.hpp>

#define CHUNK_SIZE (hsize_t)10
#define MIN(i1,i2) (i1 < i2 ? i1 : i2)

class Hdf5ObserverException: public ::ssc::exception_handling::Exception
{
    public:
        Hdf5ObserverException(const char* s) :
            ::ssc::exception_handling::Exception(s)
        {
        }
};

Hdf5Addressing::Hdf5Addressing(
        const DataAddressing& addressing,
        const std::string& basename) :
            address(H5_Tools::ensureStringStartsWithAPattern(basename,"/") +
                    H5_Tools::ensureStringStartsWithAPattern(H5_Tools::join(addressing.address,"/"),"/"))
{
}

Hdf5Observer::Hdf5Observer(
        const std::string& filename,
        const std::vector<std::string>& d) :
            Observer(d),
            h5File(H5::H5File(filename,H5F_ACC_TRUNC)),
            basename("outputs"),
            name2address(),
            name2dataset(),
            name2datatype(),
            name2dataspace(),
            group(),
            h5ElementT(),
            h5ElementX(),
            h5ElementY(),
            h5ElementZ(),
            n(0)
{
    h5_writeFileDescription(h5File);
    exportMatLabScripts(h5File, filename, basename, "/scripts/MatLab");
    exportPythonScripts(h5File, filename, basename, "/scripts/Python");
}

std::function<void()> Hdf5Observer::get_serializer(const double val, const DataAddressing& addressing)
{
    return [this,val,addressing]()
           {
                H5::DataSet dataset = name2dataset[addressing.name];
                H5::DataSpace dataspace = dataset.getSpace();
                hsize_t offset[1];
                hsize_t size[1];
                if (dataspace.getSimpleExtentDims(size)!=1)
                {
                    std::stringstream ss;
                    ss << "Rank mismatch -> Should be one, not " << dataspace.getSimpleExtentNdims();
                    THROW(__PRETTY_FUNCTION__, Hdf5ObserverException, ss.str());
                }
                const hsize_t dims[1] = {(hsize_t)1};
                offset[0] = size[0];
                size[0] += 1;
                dataset.extend(size);
                H5::DataSpace fspace = dataset.getSpace();
                fspace.selectHyperslab(H5S_SELECT_SET, dims, offset);
                dataset.write(&val, name2datatype[addressing.name], H5_Tools::createDataSpace1DUnlimited(), fspace);
           };
}

std::function<void()> Hdf5Observer::get_initializer(const double , const DataAddressing& addressing)
{
    return [this,addressing]()
           {
                name2address[addressing.name] = Hdf5Addressing(addressing,this->basename).address;
                name2datatype[addressing.name] = H5::DataType(H5::PredType::NATIVE_DOUBLE);
                name2dataspace[addressing.name] = H5_Tools::createDataSpace1DUnlimited();
           };
}

std::function<void()> Hdf5Observer::get_serializer(const SurfaceElevationGrid& waveElevationGrid, const DataAddressing&)
{
    return [this,waveElevationGrid](){write_T(waveElevationGrid);
                                      write_X(waveElevationGrid);
                                      write_Y(waveElevationGrid);
                                      write_Z(waveElevationGrid);
                                      n++;};
}

std::function<void()> Hdf5Observer::get_initializer(const SurfaceElevationGrid& waveElevationGrid, const DataAddressing&)
{
    return [this,waveElevationGrid]()
           {
               const size_t nx = waveElevationGrid.x.size();
               const size_t ny = waveElevationGrid.y.size();
               group = ((nx*ny)>0)?(H5_Tools::createMissingGroups(h5File, "waves")):H5::Group();
               h5ElementT = get_h5ElementT(nx,ny);
               h5ElementX = get_h5ElementX(nx,ny);
               h5ElementY = get_h5ElementY(nx,ny);
               h5ElementZ = get_h5ElementZ(nx,ny);
           };
}

H5Element Hdf5Observer::get_h5ElementT(const size_t nx, const size_t ny) const
{
    H5Element h5ElementT;
    if ((nx*ny)==0) return h5ElementT;
    hsize_t dimsT[1] = {1};
    const hsize_t maxdimsT[1] = {H5S_UNLIMITED};
    const hsize_t chunk_dims1[1] = {1};
    H5::DSetCreatPropList cparms1;
    cparms1.setChunk(1, chunk_dims1);
    h5ElementT.dataspace = H5::DataSpace(1, dimsT, maxdimsT);
    h5ElementT.dataset = group.createDataSet("wavest",H5::PredType::NATIVE_DOUBLE, h5ElementT.dataspace, cparms1);
    return h5ElementT;
}

H5Element Hdf5Observer::get_h5ElementX(const size_t nx, const size_t ny) const
{
    H5Element h5ElementX;
    if ((nx*ny)==0) return h5ElementX;
    hsize_t dimsX[2] = {1, 1};
    hsize_t maxdimsX[2] = {H5S_UNLIMITED, H5S_UNLIMITED};
    dimsX[1] = (hsize_t)nx;
    maxdimsX[1] = (hsize_t)nx;
    const hsize_t chunk_dims2[2] = {1, MIN(CHUNK_SIZE,(hsize_t)nx)};
    H5::DSetCreatPropList cparms2;
    cparms2.setChunk(2, chunk_dims2);
    h5ElementX.dataspace = H5::DataSpace(2, dimsX, maxdimsX);
    h5ElementX.dataset = group.createDataSet("wavesx",H5::PredType::NATIVE_DOUBLE, h5ElementX.dataspace, cparms2);
    return h5ElementX;
}

H5Element Hdf5Observer::get_h5ElementY(const size_t nx, const size_t ny) const
{
    H5Element h5ElementY;
    if ((nx*ny)==0) return h5ElementY;
    hsize_t dimsY[2] = {1, 1};
    hsize_t maxdimsY[2] = {H5S_UNLIMITED, H5S_UNLIMITED};
    dimsY[1] = (hsize_t)ny;
    maxdimsY[1] = (hsize_t)ny;
    const hsize_t chunk_dims2[2] = {1, MIN(CHUNK_SIZE,(hsize_t)ny)};
    H5::DSetCreatPropList cparms2;
    cparms2.setChunk(2, chunk_dims2);
    h5ElementY.dataspace = H5::DataSpace(2, dimsY, maxdimsY);
    h5ElementY.dataset = group.createDataSet("wavesy",H5::PredType::NATIVE_DOUBLE, h5ElementY.dataspace, cparms2);
    return h5ElementY;
}

H5Element Hdf5Observer::get_h5ElementZ(const size_t nx, const size_t ny) const
{
    H5Element h5ElementZ;
    if ((nx*ny)==0) return h5ElementZ;
    hsize_t dimsZ[3] = {1, 1, 1};
    hsize_t maxdimsZ[3] = {H5S_UNLIMITED, H5S_UNLIMITED, H5S_UNLIMITED};
    dimsZ[0] = (hsize_t)nx;
    maxdimsZ[0] = (hsize_t)nx;
    dimsZ[1] = (hsize_t)ny;
    maxdimsZ[1] = (hsize_t)ny;
    const hsize_t chunk_dims3[3] = {MIN(CHUNK_SIZE,(hsize_t)nx),MIN(CHUNK_SIZE,(hsize_t)ny),1};
    H5::DSetCreatPropList cparms3;
    cparms3.setChunk(3, chunk_dims3);
    h5ElementZ.dataspace = H5::DataSpace(3, dimsZ, maxdimsZ);
    h5ElementZ.dataset = group.createDataSet("wavesz",H5::PredType::NATIVE_DOUBLE, h5ElementZ.dataspace, cparms3);
    return h5ElementZ;
}

void Hdf5Observer::flush_after_initialization()
{
    for (const auto addressing:name2address)
    {
        name2dataset[addressing.first] =
                H5_Tools::createDataSet(h5File,
                                        addressing.second,
                                        name2datatype[addressing.first],
                                        name2dataspace[addressing.first]);
    }
}

void Hdf5Observer::write_T(const SurfaceElevationGrid& waveElevationGrid) const
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

void Hdf5Observer::write_X(const SurfaceElevationGrid& waveElevationGrid) const
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

void Hdf5Observer::write_Y(const SurfaceElevationGrid& waveElevationGrid) const
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

void Hdf5Observer::write_Z(const SurfaceElevationGrid& waveElevationGrid) const
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
    if (!waveElevationGrid.z.IsRowMajor)
    {
        const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> row_major_mat(waveElevationGrid.z);
        h5ElementZ.dataset.write(row_major_mat.data(), H5::PredType::NATIVE_DOUBLE, h5ElementZ.dataspace, fspaceZ);
    }
    else
    {
        h5ElementZ.dataset.write(waveElevationGrid.z.data(), H5::PredType::NATIVE_DOUBLE, h5ElementZ.dataspace, fspaceZ);
    }
}

void Hdf5Observer::flush_after_write()
{
}

void Hdf5Observer::flush_value()
{
}
