#ifndef MESHBUILDER_HPP
#define MESHBUILDER_HPP

#include <map>
#include "GeometricTypes3d.hpp"
#include "MeshNumeric.hpp"
#include "Mesh.hpp"

struct Vector3dComparator
{
    bool operator() (const EPoint& lhs, const EPoint& rhs)
    {
        if (MESH_LT(lhs(0),rhs(0))) {return true;}
        if (MESH_EQ(lhs(0),rhs(0)))
        {
            if (MESH_LT(lhs(1),rhs(1))) {return true;}
            if (MESH_EQ(lhs(1),rhs(1)) && MESH_LT(lhs(2),rhs(2))) {return true;}
        }
        return false;
    }
};

typedef std::map<EPoint, size_t, Vector3dComparator> Vector3dMap;

class MeshBuilder
{
    public:
        MeshBuilder(const VectorOfVectorOfPoints& v_);
        MeshBuilder(const VectorOfPoints& tri);
        Mesh build();
        void operator()(const VectorOfPoints& Tri);
        Eigen::Matrix<double,3,Eigen::Dynamic> get_nodes() const;
        std::vector<Facet> get_facets() const;

    private:
        VectorOfVectorOfPoints v;
        Vector3dMap xyzMap;
        size_t index;
        Eigen::Matrix<double,3,Eigen::Dynamic> nodes;
        std::vector<Facet> facets;

        Eigen::Matrix<double,3,Eigen::Dynamic> resize(const Eigen::Matrix<double,3,Eigen::Dynamic>& M) const;
        size_t build_one_point(const EPoint& xyz);
        bool point_is_in_map(const EPoint& xyz);
        bool add_point_if_missing(const EPoint& xyz);
        Matrix3x convert(const VectorOfPoints& v) const;
};

#endif
