#include <algorithm>
#include <iostream>
#include "TriMeshBuilder.hpp"

std::vector<Eigen::Vector3d> TriMeshBuilder::get_nodes() const
{
	return nodes;
}

std::vector<Facet> TriMeshBuilder::get_facets() const
{
	return facets;
}

void TriMeshBuilder::build()
{
	*this = std::for_each(v.begin(), v.end(), *this);
}

void TriMeshBuilder::operator()(const Point3dTriplet& tri)
{
	Facet facet;
	if (evaluate_unit_normal(tri,facet.unit_normal))
	{
		facet.area = evaluate_area(tri);
		facet.barycenter = evaluate_barycenter(tri);
		facet.index[0] = build_one_point(tri.p1);
		facet.index[1] = build_one_point(tri.p2);
		facet.index[2] = build_one_point(tri.p3);
		facets.push_back(facet);
	}
}

Eigen::Vector3d TriMeshBuilder::evaluate_barycenter(const Point3dTriplet& tri) const
{
	Eigen::Vector3d xyz;
	xyz = (tri.p1+tri.p2+tri.p3)/3.0;
	return xyz;
}

bool TriMeshBuilder::evaluate_unit_normal(const Point3dTriplet& tri, Eigen::Vector3d& unit_normal) const
{
	const Eigen::Vector3d normal = evaluate_normal(tri);
	const double norm = normal.norm();
	if (norm<1000*std::numeric_limits<double>::epsilon())
	{
		std::cerr<<__PRETTY_FUNCTION__<<"Input triangle is degenerated"<<std::endl;
		return false;
	}
	unit_normal  = normal/norm;
	return true;
}

Eigen::Vector3d TriMeshBuilder::evaluate_normal(const Point3dTriplet& tri) const
{
	const Eigen::Vector3d n1(tri.p2-tri.p1);
	const Eigen::Vector3d n2(tri.p3-tri.p1);
	return n1.cross(n2);
}

double TriMeshBuilder::evaluate_area(const Point3dTriplet& tri) const
{
	const Eigen::Vector3d n1(tri.p2-tri.p1);
	const Eigen::Vector3d n2(tri.p3-tri.p1);
	return 0.5*fabs((n1.cross(n2)).norm());
}

size_t TriMeshBuilder::build_one_point(const Eigen::Vector3d& xyz)
{
	const bool point_has_been_added = add_point_if_missing(xyz);
	const size_t ret = index;
	if (point_has_been_added) index++;
	return ret;
}

bool TriMeshBuilder::add_point_if_missing(const Eigen::Vector3d& xyz)
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

bool TriMeshBuilder::point_is_in_map(const Eigen::Vector3d& xyz)
{
	const Vector3dMap::const_iterator itMap = xyzMap.find(xyz);
	return itMap != xyzMap.end();
}
