#ifndef TRIMESHBUILDER_HPP
#define TRIMESHBUILDER_HPP

#include <map>
#include "GeometricTypes3d.hpp"
#include "MeshNumeric.hpp"
#include "TriMesh.hpp"

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

class TriMeshBuilder
{
	public:
		TriMeshBuilder(const VectorOfVectorOfPoints& v_);
		TriMeshBuilder(const VectorOfPoints& tri);
		TriMesh build();
		void operator()(const VectorOfPoints& Tri);
		Eigen::Matrix<double,3,Eigen::Dynamic> get_nodes() const;
		std::vector<Facet> get_facets() const;

	private:
		VectorOfVectorOfPoints v;
		Vector3dMap xyzMap;
		size_t index;
		Eigen::Matrix<double,3,Eigen::Dynamic> nodes;
		std::vector<Facet> facets;

		size_t build_one_point(const EPoint& xyz);
		bool point_is_in_map(const EPoint& xyz);
		bool add_point_if_missing(const EPoint& xyz);
};

#endif
