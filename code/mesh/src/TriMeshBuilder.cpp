#include <algorithm>
#include <iostream>
#include "TriMeshBuilder.hpp"

Eigen::Matrix<double,3,Eigen::Dynamic> TriMeshBuilder::get_nodes() const
{
	return nodes;
}

std::vector<Facet> TriMeshBuilder::get_facets() const
{
	return facets;
}

TriMeshBuilder::TriMeshBuilder(const VectorOfPoint3dTriplet& v_) : v(v_),
                                                                   xyzMap(Vector3dMap()),
                                                                   index(0),
                                                                   nodes(Eigen::Matrix<double,3,Eigen::Dynamic>(Eigen::MatrixXd::Zero(3,3*v.size()))),
                                                                   facets(std::vector<Facet>())
{
}

TriMeshBuilder::TriMeshBuilder(const Point3dTriplet& tri) : v(VectorOfPoint3dTriplet(1,tri)),
                                                            xyzMap(Vector3dMap()),
                                                            index(0),
                                                            nodes(Eigen::Matrix<double,3,Eigen::Dynamic>(Eigen::MatrixXd::Zero(3,3))),
                                                            facets(std::vector<Facet>())
{
}

TriMesh TriMeshBuilder::build()
{
	*this = std::for_each(v.begin(), v.end(), *this);
	nodes.resize(3, index);
	return TriMesh(nodes, facets);
}

void TriMeshBuilder::operator()(const Point3dTriplet& tri)
{
	Facet facet;
	if (unit_normal(tri,facet.unit_normal))
	{
		facet.area = area(tri);
		facet.barycenter = barycenter(tri);
		facet.index[0] = build_one_point(tri.p1);
		facet.index[1] = build_one_point(tri.p2);
		facet.index[2] = build_one_point(tri.p3);
		facets.push_back(facet);
	}
}

Eigen::Vector3d TriMeshBuilder::barycenter(const Point3dTriplet& tri) const
{
	Eigen::Vector3d xyz;
	xyz = (tri.p1+tri.p2+tri.p3)/3.0;
	return xyz;
}

bool TriMeshBuilder::unit_normal(const Point3dTriplet& tri, Eigen::Vector3d& unit_normal) const
{
	const Eigen::Vector3d n = normal(tri);
	const double norm = n.norm();
	if (norm<1000*std::numeric_limits<double>::epsilon())
	{
		std::cout << "WARNING: in " << __PRETTY_FUNCTION__<< ": input triangle is degenerated" << std::endl;
		return false;
	}
	unit_normal  = n/norm;
	return true;
}

Eigen::Vector3d TriMeshBuilder::normal(const Point3dTriplet& tri) const
{
	const Eigen::Vector3d n1(tri.p2-tri.p1);
	const Eigen::Vector3d n2(tri.p3-tri.p1);
	return n1.cross(n2);
}

double TriMeshBuilder::area(const Point3dTriplet& tri) const
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
		nodes.col(index) = xyz;
		point_has_been_added = true;
	}
	return point_has_been_added;
}

bool TriMeshBuilder::point_is_in_map(const Eigen::Vector3d& xyz)
{
	const Vector3dMap::const_iterator itMap = xyzMap.find(xyz);
	return itMap != xyzMap.end();
}
