#include <math.h>
#include <algorithm>
#include <limits>
#include <iostream>
#include "test_macros.hpp"

#include "MeshException.hpp"
#include "TriMeshBuilder.hpp"

std::vector<Xyz> TriMeshBuilder::get_nodes() const
{
	return nodes;
}

std::vector<Facet> TriMeshBuilder::get_facets() const
{
	return facets;
}

void TriMeshBuilder::build()
{
	COUT("size"<<v.size());
	*this = std::for_each(v.begin(), v.end(), *this);
}

void TriMeshBuilder::operator()(const Point3dTriplet& tri)
{
	Facet facet;
	Eigen::Vector3d normal = evaluate_normal(tri);
	const double norm = normal.norm();
	COUT("norm"<<norm);
	if (norm<1000*std::numeric_limits<double>::epsilon())
	{
		THROW(__PRETTY_FUNCTION__, MeshException,"Input triangle is degenerated");
	}
	normal/=norm;
	facet.unit_normal.x = normal(0);
	facet.unit_normal.y = normal(1);
	facet.unit_normal.z = normal(2);

	facet.area = evaluate_area(tri);
	facet.barycenter = evaluate_barycenter(tri);
	facet.index[0] = build_one_point(tri.p1);
	facet.index[1] = build_one_point(tri.p2);
	facet.index[2] = build_one_point(tri.p3);
	facets.push_back(facet);
}

Xyz TriMeshBuilder::evaluate_barycenter(const Point3dTriplet& tri) const
{
	Xyz xyz;
	xyz.x = (tri.p1.x+tri.p2.x+tri.p3.x)/3.0;
	xyz.y = (tri.p1.y+tri.p2.y+tri.p3.y)/3.0;
	xyz.z = (tri.p1.z+tri.p2.z+tri.p3.z)/3.0;
	return xyz;
}

Eigen::Vector3d TriMeshBuilder::evaluate_normal(const Point3dTriplet& tri) const
{
	const Eigen::Vector3d n1(tri.p2.x-tri.p1.x,tri.p2.y-tri.p1.y,tri.p2.z-tri.p1.z);
	const Eigen::Vector3d n2(tri.p3.x-tri.p1.x,tri.p3.y-tri.p1.y,tri.p3.z-tri.p1.z);
	return n1.cross(n2);
}

double TriMeshBuilder::evaluate_area(const Point3dTriplet& tri) const
{
	const Eigen::Vector3d n1(tri.p2.x-tri.p1.x,tri.p2.y-tri.p1.y,tri.p2.z-tri.p1.z);
	const Eigen::Vector3d n2(tri.p3.x-tri.p1.x,tri.p3.y-tri.p1.y,tri.p3.z-tri.p1.z);
	return 0.5*fabs((n1.cross(n2)).norm());
}

size_t TriMeshBuilder::build_one_point(const Xyz& xyz)
{
	const bool point_has_been_added = add_point_if_missing(xyz);
	const size_t ret = index;
	if (point_has_been_added) index++;
	return ret;
}

bool TriMeshBuilder::add_point_if_missing(const Xyz& xyz)
{
	bool point_has_been_added = false;
	if (not(point_is_in_map(xyz)))
	{
		xyzMap.insert(std::make_pair(xyz,index));
		nodes.push_back(xyz);
		point_has_been_added = true;
	}
	return point_has_been_added;
}

bool TriMeshBuilder::point_is_in_map(const Xyz& xyz)
{
	const XyzMap::const_iterator itMap = xyzMap.find(xyz);
	return itMap != xyzMap.end();
}
