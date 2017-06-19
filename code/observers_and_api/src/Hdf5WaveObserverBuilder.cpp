#include "Hdf5WaveObserverBuilder.hpp"

#include "h5_interface.hpp"

/** \def MIN(i1,i2) Define minimum of two scalar*/
#define MIN(i1,i2) (i1 < i2 ? i1 : i2)

/** \def CHUNK_SIZE Hdf5 chunk parameter used to buffer data before writing*/
#define CHUNK_SIZE (hsize_t)10

Hdf5WaveObserverBuilder::Hdf5WaveObserverBuilder(
    const std::string& fileName,
    const std::string& datasetName_,
    const size_t nx_,
    const size_t ny_):
        h5File(H5_Tools::openOrCreateAHdf5File(fileName)),
        datasetName(H5_Tools::ensureStringStartsAndEndsWithAPattern(datasetName_,"/")),
        group(((nx_*ny_)>0)?(H5_Tools::createMissingGroups(h5File, datasetName)):H5::Group()),
        nx(nx_),ny(ny_)
{
}

Hdf5WaveObserverBuilder::Hdf5WaveObserverBuilder(
    const H5::H5File& h5File_,
    const std::string& datasetName_,
    const size_t nx_,
    const size_t ny_):
        h5File(h5File_),
        datasetName(H5_Tools::ensureStringStartsAndEndsWithAPattern(datasetName_,"/")),
        group(((nx_*ny_)>0)?(H5_Tools::createMissingGroups(h5File, datasetName)):H5::Group()),
        nx(nx_),
        ny(ny_)
{
}

H5::H5File Hdf5WaveObserverBuilder::get_h5File() const
{
    return h5File;
}

H5::Group Hdf5WaveObserverBuilder::get_group() const
{
    return group;
}

H5Element Hdf5WaveObserverBuilder::get_h5ElementT() const
{
    H5Element h5ElementT;
    if ((nx*ny)==0) return h5ElementT;
    hsize_t dimsT[1] = {1};
    const hsize_t maxdimsT[1] = {H5S_UNLIMITED};
    const hsize_t chunk_dims1[1] = {1};
    H5::DSetCreatPropList cparms1;
    cparms1.setChunk(1, chunk_dims1);
    h5ElementT.datatype = H5::DataType(H5::PredType::NATIVE_DOUBLE);
    h5ElementT.dataspace = H5::DataSpace(1, dimsT, maxdimsT);
    h5ElementT.dataset = group.createDataSet(datasetName+"t",h5ElementT.datatype, h5ElementT.dataspace, cparms1);
    return h5ElementT;
}

H5Element Hdf5WaveObserverBuilder::get_h5ElementX() const
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
    h5ElementX.datatype = H5::DataType(H5::PredType::NATIVE_DOUBLE);
    h5ElementX.dataspace = H5::DataSpace(2, dimsX, maxdimsX);
    h5ElementX.dataset = group.createDataSet(datasetName+"x",h5ElementX.datatype, h5ElementX.dataspace, cparms2);
    return h5ElementX;
}

H5Element Hdf5WaveObserverBuilder::get_h5ElementY() const
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
    h5ElementY.datatype = H5::DataType(H5::PredType::NATIVE_DOUBLE);
    h5ElementY.dataspace = H5::DataSpace(2, dimsY, maxdimsY);
    h5ElementY.dataset = group.createDataSet(datasetName+"y",h5ElementY.datatype, h5ElementY.dataspace, cparms2);
    return h5ElementY;
}

H5Element Hdf5WaveObserverBuilder::get_h5ElementZ() const
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
    h5ElementZ.datatype = H5::DataType(H5::PredType::NATIVE_DOUBLE);
    h5ElementZ.dataspace = H5::DataSpace(3, dimsZ, maxdimsZ);
    h5ElementZ.dataset = group.createDataSet(datasetName+"z",h5ElementZ.datatype, h5ElementZ.dataspace, cparms3);
    return h5ElementZ;
}

