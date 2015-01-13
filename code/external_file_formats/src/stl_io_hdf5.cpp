#include "h5_tools.hpp"
#include "stl_io_hdf5.hpp"
#include <utility>

size_t getNumberOfPoints(const VectorOfVectorOfPoints& vvP);
size_t getNumberOfTriangles(const VectorOfVectorOfPoints& vvP);

size_t getNumberOfPoints(const VectorOfVectorOfPoints& vvP)
{
    size_t nPoints = 0;
    for (const auto vP:vvP)
    {
        const size_t nbPointForCurrentPolygones = vP.size();
        nPoints+=nbPointForCurrentPolygones;
    }
    return nPoints;
}

size_t getNumberOfTriangles(const VectorOfVectorOfPoints& vvP)
{
    size_t nTriangles = 0;
    for (const auto vP:vvP)
    {
        const size_t nbPointForCurrentPolygones = vP.size();
        nTriangles+=(nbPointForCurrentPolygones-2);
    }
    return nTriangles;
}

SVectorOfVectorOfPoints::SVectorOfVectorOfPoints(const VectorOfVectorOfPoints& vvP_):
    vvP(vvP_),nPoints(getNumberOfPoints(vvP_)),nTriangles(getNumberOfTriangles(vvP_))
{
}

std::vector<double> SVectorOfVectorOfPoints::concatenatePoints() const
{
    std::vector<double> res;
    res.reserve(3*nPoints);
    for (const auto vP:vvP)
    {
        for (const auto p:vP)
        {
            for (size_t k = 0; k<3 ; ++k)
            {
                res.push_back(p(k));
            }
        }
    }
    return res;
}


std::vector<uint64_t> SVectorOfVectorOfPoints::concatenateTriangles() const
{
    std::vector<uint64_t> res;
    res.reserve(3*nTriangles);
    uint64_t i = 0;
    for (const auto vP:vvP)
    {
        const uint64_t c = ++i;
        for (size_t k=0;k<vP.size()-2;++k)
        {
            res.push_back(c);
            res.push_back(c+k+1);
            res.push_back(c+k+2);
        }
    }
    return res;
}


void writeMeshToHdf5File(
        const std::string& file,
        const std::string& datasetName,
        const VectorOfVectorOfPoints& v)
{
    writeMeshToHdf5File(H5::H5File(file,H5F_ACC_TRUNC), datasetName,v);
}

void writeMeshToHdf5File(
        const H5::H5File& file,
        const std::string& datasetName,
        const VectorOfVectorOfPoints& v)
{
    const SVectorOfVectorOfPoints sv(v);
    H5::DataType doubleType(H5::PredType::NATIVE_DOUBLE);
    const hsize_t sPoints[2] = {sv.nPoints,3};
    H5::DataSpace pointsSpace(2, sPoints);
    H5::DataSet dP = H5_Tools::createDataSet(file, datasetName+"/points", doubleType, pointsSpace);
    dP.write((void*)sv.concatenatePoints().data(), doubleType);

    H5::DataType uint64Type(H5::PredType::NATIVE_UINT64);
    const hsize_t sTriangles[2] = {sv.nTriangles,3};
    H5::DataSpace trianglesSpace(2, sTriangles);
    H5::DataSet dT = H5_Tools::createDataSet(file, datasetName+"/triangles", uint64Type, trianglesSpace);
    dT.write((void*)sv.concatenateTriangles().data(), uint64Type);
}
/*
void readMeshFromHdf5File(
        const std::string& file,
        const std::string& datasetName,
        const VectorOfVectorOfPoints& v)
{
    readMeshFromHdf5File(H5::H5File(file,H5F_ACC_RDONLY), datasetName,v);
}

void readMeshFromHdf5File(
        const H5::H5File& file,
        const std::string& datasetName,
        const VectorOfVectorOfPoints& v)
{

}
*/
