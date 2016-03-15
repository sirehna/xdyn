#ifndef STL_IO_HDF5_CPP_
#define STL_IO_HDF5_CPP_

#include "H5Cpp.h"
#include "GeometricTypes3d.hpp"

struct SVectorOfVectorOfPoints
{
    VectorOfVectorOfPoints vvP;
    size_t nPoints;
    size_t nTriangles;
    SVectorOfVectorOfPoints(const VectorOfVectorOfPoints& vvP_);
    std::vector<double> concatenatePoints() const;
    std::vector<uint64_t> concatenateTriangles() const;
};

void writeMeshToHdf5File(
        const std::string& file,
        const std::string& datasetName,
        const VectorOfVectorOfPoints& v);

void writeMeshToHdf5File(
        const H5::H5File& file,
        const std::string& datasetName,
        const VectorOfVectorOfPoints& v);

VectorOfVectorOfPoints readMeshFromHdf5File(
        const std::string& file,
        const std::string& datasetName);

VectorOfVectorOfPoints readMeshFromHdf5File(
        const H5::H5File& file,
        const std::string& datasetName);

void writeMeshToHdf5File(
        const std::string& file,
        const std::string& datasetName,
        const Matrix3x& nodes,
        const std::vector<Facet>& facets);

void writeMeshToHdf5File(
        const H5::H5File& file,
        const std::string& datasetName,
        const Matrix3x& nodes,
        const std::vector<Facet>& facets);

#endif
