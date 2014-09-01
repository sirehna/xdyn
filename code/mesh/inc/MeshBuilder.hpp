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

struct EdgeComparator
{
    bool operator() (const Edge& lhs, const Edge& rhs)
    {
        size_t i1 = std::min(lhs.vertex_index[0],lhs.vertex_index[1]);
        size_t i2 = std::max(lhs.vertex_index[0],lhs.vertex_index[1]);
        size_t j1 = std::min(rhs.vertex_index[0],rhs.vertex_index[1]);
        size_t j2 = std::max(rhs.vertex_index[0],rhs.vertex_index[1]);
        if ( i1 < j1) {return true;}
        if ( i1 == j1 )
        {
            if (i2 < j2) {return true;}
        }
        return false;
    }
};

typedef std::map<Edge, size_t, EdgeComparator> EdgeMap;

class MeshBuilder
{
    public:
        MeshBuilder(const VectorOfVectorOfPoints& v_);
        MeshBuilder(const VectorOfPoints& tri);
        MeshBuilder(const Matrix3x& tri);
        Mesh build();
        void operator()(const VectorOfPoints& Tri);
        Matrix3x get_nodes() const;
        std::vector<Edge> get_edges() const;
        std::vector<Facet> get_facets() const;

    private:
        VectorOfVectorOfPoints v;
        Vector3dMap xyzMap;
        EdgeMap edgeMap;
        size_t index;
        size_t edgeIndex;
        Matrix3x nodes;
        std::vector<Edge> edges;
        std::vector<Facet> facets;

        Matrix3x resize(const Matrix3x& M) const;
        size_t build_one_edge(const Edge& e);
        size_t build_one_point(const EPoint& xyz);
        bool edge_is_in_map(const Edge& e);
        bool point_is_in_map(const EPoint& xyz);
        bool add_edge_if_missing(const Edge& e);
        bool add_point_if_missing(const EPoint& xyz);

        bool clockwise;
};

#endif
