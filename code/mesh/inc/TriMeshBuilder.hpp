#ifndef TRIMESHBUILDER_HPP
#define TRIMESHBUILDER_HPP

#include <map>
#include <Eigen/Dense>
#include "GeometricTypes3d.hpp"
#include "MeshNumeric.hpp"

struct XyzComparator
{
    bool operator() (const Xyz& lhs, const Xyz& rhs)
    {
        if (MESH_LT(lhs.x,rhs.x)) {return true;}
        if (MESH_EQ(lhs.x,rhs.x))
        {
            if (MESH_LT(lhs.y,rhs.y)) {return true;}
            if (MESH_EQ(lhs.y,rhs.y) && MESH_LT(lhs.z,rhs.z)) {return true;}
        }
        return false;
    }
};

typedef std::map< Xyz , size_t, XyzComparator > XyzMap;

class TriMesh;

class TriMeshBuilder
{
	public:
		TriMeshBuilder(const VectorOfPoint3dTriplet& v_):v(v_),xyzMap(XyzMap()), index(0),nodes(std::vector<Xyz>()),facets(std::vector<Facet>()){}
		void build();
		void operator()(const Point3dTriplet& Tri);
		std::vector<Xyz> get_nodes() const;
		std::vector<Facet> get_facets() const;

		VectorOfPoint3dTriplet v;
		XyzMap xyzMap;
		size_t index;
		std::vector<Xyz> nodes;
		std::vector<Facet> facets;
	private:
		Eigen::Vector3d evaluate_normal(const Point3dTriplet& tri) const;
		Xyz evaluate_barycenter(const Point3dTriplet& tri) const;
		double evaluate_area(const Point3dTriplet& tri) const;
		size_t build_one_point(const Xyz& xyz);
		bool point_is_in_map(const Xyz& xyz);
		bool add_point_if_missing(const Xyz& xyz);
};

#endif
