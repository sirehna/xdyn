#include <utility>
#include "eigen3-hdf5.hpp"
#include "h5_tools.hpp"
#include "stl_io_hdf5.hpp"

size_t getNumberOfPoints(const VectorOfVectorOfPoints& vvP);
size_t getNumberOfTriangles(const VectorOfVectorOfPoints& vvP);

size_t getNumberOfTriangles(const std::vector<Facet>& vvP);

size_t getNumberOfPoints(const VectorOfVectorOfPoints& vvP)
{
    size_t nPoints = 0;
    for (const auto vP:vvP)
    {
        const size_t nbPointForCurrentPolygon = vP.size();
        nPoints+=nbPointForCurrentPolygon;
    }
    return nPoints;
}

size_t getNumberOfTriangles(const VectorOfVectorOfPoints& vvP)
{
    size_t nTriangles = 0;
    for (const auto vP:vvP)
    {
        const size_t nbPointForCurrentPolygon = vP.size();
        nTriangles+=(nbPointForCurrentPolygon-2);
    }
    return nTriangles;
}

size_t getNumberOfTriangles(const std::vector<Facet>& vvP)
{
    size_t nTriangles = 0;
    for (const auto vP:vvP)
    {
        const size_t nbPointForCurrentPolygon = vP.vertex_index.size();
        nTriangles+=(nbPointForCurrentPolygon-2);
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
    for (const auto& vP : vvP)
    {
        for (const auto& p:vP)
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
    uint64_t c = 1;
    for (const auto& vP : vvP)
    {
        for (size_t k=0;k<vP.size()-2;++k)
        {
            res.push_back(c);
            res.push_back(c+k+1);
            res.push_back(c+k+2);
        }
        c += vP.size();
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
    H5::DataSet dT = H5_Tools::createDataSet(file, datasetName+"/faces", uint64Type, trianglesSpace);
    dT.write((void*)sv.concatenateTriangles().data(), uint64Type);
}

void writeMeshToHdf5File(
        const std::string& file,
        const std::string& datasetName,
        const Matrix3x& nodes,
        const std::vector<Facet>& facets)
{
    writeMeshToHdf5File(H5::H5File(file,H5F_ACC_TRUNC), datasetName, nodes, facets);
}

void writeMeshToHdf5File(
        const H5::H5File& file,
        const std::string& datasetName,
        const Matrix3x& nodes,
        const std::vector<Facet>& facets)
{
    const size_t nPoints = nodes.cols();
    std::vector<double> points;
    points.reserve(nodes.rows()*3);
    for (size_t k = 0; k<nPoints ; ++k)
    {
        points.push_back(nodes(0,k));
        points.push_back(nodes(1,k));
        points.push_back(nodes(2,k));
    }
    H5::DataType doubleType(H5::PredType::NATIVE_DOUBLE);
    const hsize_t sPoints[2] = {nPoints,3};
    H5::DataSpace pointsSpace(2, sPoints);
    H5::DataSet dP = H5_Tools::createDataSet(file, datasetName+"/points", doubleType, pointsSpace);
    dP.write((void*)points.data(), doubleType);

    const size_t nTriangles = getNumberOfTriangles(facets);
    H5::DataType uint64Type(H5::PredType::NATIVE_UINT64);
    const hsize_t sTriangles[2] = {nTriangles,3};
    H5::DataSpace trianglesSpace(2, sTriangles);
    H5::DataSet dT = H5_Tools::createDataSet(file, datasetName+"/faces", uint64Type, trianglesSpace);
    std::vector<uint64_t> res;
    res.reserve(3*nTriangles);
    for (const auto& vP : facets)
    {
        for (size_t k=0;k<vP.vertex_index.size()-2;++k)
        {
            res.push_back(1+vP.vertex_index.at(0));
            res.push_back(1+vP.vertex_index.at(k+1));
            res.push_back(1+vP.vertex_index.at(k+2));
        }
    }
    dT.write((void*)res.data(), uint64Type);
}

VectorOfVectorOfPoints readMeshFromHdf5File(
        const std::string& file,
        const std::string& datasetName)
{
    return readMeshFromHdf5File(H5::H5File(file,H5F_ACC_RDONLY), datasetName);
}

VectorOfVectorOfPoints readMeshFromHdf5File(
        const H5::H5File& file,
        const std::string& datasetName)
{
    VectorOfVectorOfPoints v;
    Eigen::MatrixXd points;
    Eigen::Matrix<uint64_t, Eigen::Dynamic, Eigen::Dynamic> faces;
    EigenHDF5::load(file, datasetName+"/points", points);
    EigenHDF5::load(file, datasetName+"/faces", faces);
    for (size_t i=0;i<faces.rows();++i)
    {
        std::vector<EPoint> vv;
        for (size_t j=0;j<faces.cols();++j)
        {
            vv.push_back(EPoint(
                            points(faces(i,j)-1,0),
                            points(faces(i,j)-1,1),
                            points(faces(i,j)-1,2)));
        }
        v.push_back(vv);
    }
    return v;
}
