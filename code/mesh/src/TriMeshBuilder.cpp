#include <algorithm>
#include <iostream>

#include "TriMeshBuilder.hpp"
#include "mesh_manipulations.hpp"

Eigen::Matrix<double,3,Eigen::Dynamic> TriMeshBuilder::get_nodes() const
{
	return nodes;
}

std::vector<Facet> TriMeshBuilder::get_facets() const
{
	return facets;
}

TriMeshBuilder::TriMeshBuilder(const VectorOfVectorOfPoints& v_) : v(v_),
                                                                   xyzMap(Vector3dMap()),
                                                                   index(0),
                                                                   nodes(Eigen::Matrix<double,3,Eigen::Dynamic>(Eigen::MatrixXd::Zero(3,3*v.size()))),
                                                                   facets(std::vector<Facet>())
{
}

TriMeshBuilder::TriMeshBuilder(const VectorOfPoints& tri) : v(VectorOfVectorOfPoints(1,tri)),
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

void TriMeshBuilder::operator()(const VectorOfPoints& tri)
{
	Facet facet;
    facet.unit_normal = unit_normal(tri);
    facet.area = area(tri);
    facet.barycenter = barycenter(tri);
    for (VectorOfPoints::const_iterator it = tri.begin() ; it != tri.end() ; ++it)
    {
        facet.index.push_back(build_one_point(*it));
    }
    facets.push_back(facet);
}

size_t TriMeshBuilder::build_one_point(const EPoint& xyz)
{
	const bool point_has_been_added = add_point_if_missing(xyz);
	const size_t ret = index;
	if (point_has_been_added) index++;
	return ret;
}

bool TriMeshBuilder::add_point_if_missing(const EPoint& xyz)
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

bool TriMeshBuilder::point_is_in_map(const EPoint& xyz)
{
	const Vector3dMap::const_iterator itMap = xyzMap.find(xyz);
	return itMap != xyzMap.end();
}
