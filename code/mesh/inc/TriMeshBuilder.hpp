#ifndef TRIMESHBUILDER_HPP
#define TRIMESHBUILDER_HPP

#include <map>
#include "GeometricTypes3d.hpp"
#include "MeshNumeric.hpp"

struct XyzComparator
{
    bool operator() (const Eigen::Vector3d& lhs, const Eigen::Vector3d& rhs)
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

typedef std::map< Eigen::Vector3d , size_t, XyzComparator > XyzMap;

class TriMesh;

class TriMeshBuilder
{
	public:
		TriMeshBuilder(const VectorOfPoint3dTriplet& v_):v(v_),xyzMap(XyzMap()), index(0),nodes(std::vector<Eigen::Vector3d>()),facets(std::vector<Facet>()){}
		void build();
		void operator()(const Point3dTriplet& Tri);
		std::vector<Eigen::Vector3d> get_nodes() const;
		std::vector<Facet> get_facets() const;

		VectorOfPoint3dTriplet v;
		XyzMap xyzMap;
		size_t index;
		std::vector<Eigen::Vector3d> nodes;
		std::vector<Facet> facets;
	private:
		Eigen::Vector3d evaluate_normal(const Point3dTriplet& tri) const;
		Eigen::Vector3d evaluate_barycenter(const Point3dTriplet& tri) const;
		double evaluate_area(const Point3dTriplet& tri) const;
		size_t build_one_point(const Eigen::Vector3d& xyz);
		bool point_is_in_map(const Eigen::Vector3d& xyz);
		bool add_point_if_missing(const Eigen::Vector3d& xyz);
};

#endif
