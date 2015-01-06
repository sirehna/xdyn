#include "SimHdf5WaveObserverBuilder.hpp"


SimHdf5WaveObserverBuilder::SimHdf5WaveObserverBuilder(
    const std::string& fileName,
    const std::string& datasetName_,
    const size_t nx_,
    const size_t ny_):h5File(H5::H5File(fileName,H5F_ACC_TRUNC)),group(h5File.createGroup("/"+datasetName_)),datasetName(datasetName_),nx(nx_),ny(ny_)
{
}

SimHdf5WaveObserverBuilder::SimHdf5WaveObserverBuilder(
    const H5::H5File& h5File_,
    const std::string& datasetName_,
    const size_t nx_,
    const size_t ny_):h5File(h5File_),group(h5File.createGroup("/"+datasetName_)),datasetName(datasetName_),nx(nx_),ny(ny_)
{
}

H5::H5File SimHdf5WaveObserverBuilder::get_h5File() const
{
    return h5File;
}
H5::Group SimHdf5WaveObserverBuilder::get_group() const
{
    return group;
}

H5Element SimHdf5WaveObserverBuilder::get_h5ElementT() const
{
    H5Element h5ElementT;
    hsize_t dimsT[1] = {1};
    const hsize_t maxdimsT[1] = {H5S_UNLIMITED};
    const hsize_t chunk_dims1[1] = {1};
    H5::DSetCreatPropList cparms1;
    cparms1.setChunk(1, chunk_dims1);
    h5ElementT.dataspace = H5::DataSpace(1, dimsT, maxdimsT);
    h5ElementT.dataset = group.createDataSet("/"+datasetName+"/t",H5::PredType::NATIVE_DOUBLE, h5ElementT.dataspace, cparms1);
    return h5ElementT;
}

H5Element SimHdf5WaveObserverBuilder::get_h5ElementX() const
{
    H5Element h5ElementX;
    hsize_t dimsX[2] = {1, 1};
    hsize_t maxdimsX[2] = {H5S_UNLIMITED, H5S_UNLIMITED};
    const hsize_t chunk_dims2[2] = {1,10};
    H5::DSetCreatPropList cparms2;
    cparms2.setChunk(2, chunk_dims2);
    dimsX[1] = (hsize_t)nx;
    maxdimsX[1] = (hsize_t)nx;
    h5ElementX.dataspace = H5::DataSpace(2, dimsX, maxdimsX);
    h5ElementX.dataset = group.createDataSet("/"+datasetName+"/x",H5::PredType::NATIVE_DOUBLE, h5ElementX.dataspace, cparms2);
    return h5ElementX;
}

H5Element SimHdf5WaveObserverBuilder::get_h5ElementY() const
{
    H5Element h5ElementY;
    hsize_t dimsY[2] = {1, 1};
    hsize_t maxdimsY[2] = {H5S_UNLIMITED, H5S_UNLIMITED};
    const hsize_t chunk_dims2[2] = {1,10};
    H5::DSetCreatPropList cparms2;
    cparms2.setChunk(2, chunk_dims2);
    dimsY[1] = (hsize_t)ny;
    maxdimsY[1] = (hsize_t)ny;
    h5ElementY.dataspace = H5::DataSpace(2, dimsY, maxdimsY);
    h5ElementY.dataset = group.createDataSet("/"+datasetName+"/y",H5::PredType::NATIVE_DOUBLE, h5ElementY.dataspace, cparms2);
    return h5ElementY;
}

H5Element SimHdf5WaveObserverBuilder::get_h5ElementZ() const
{
    H5Element h5ElementZ;
    hsize_t dimsZ[3] = {1, 1, 1};
    hsize_t maxdimsZ[3] = {H5S_UNLIMITED, H5S_UNLIMITED, H5S_UNLIMITED};
    const hsize_t chunk_dims3[3] = {20,10,1};
    H5::DSetCreatPropList cparms3;
    cparms3.setChunk(3, chunk_dims3);
    dimsZ[0] = (hsize_t)nx;
    maxdimsZ[0] = (hsize_t)nx;
    dimsZ[1] = (hsize_t)ny;
    maxdimsZ[1] = (hsize_t)ny;
    h5ElementZ.dataspace = H5::DataSpace(3, dimsZ, maxdimsZ);
    h5ElementZ.dataset = group.createDataSet("/"+datasetName+"/z",H5::PredType::NATIVE_DOUBLE, h5ElementZ.dataspace, cparms3);
    return h5ElementZ;
}
