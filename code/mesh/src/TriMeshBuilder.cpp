#include <algorithm>
#include <iostream>
#include "test_macros.hpp"

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
	std::cout<<v.size()<<std::endl;
	std::cout<<v.at(0).p1.x<<std::endl;
	*this = std::for_each(v.begin(), v.end(), *this);
}

void TriMeshBuilder::operator()(const Point3dTriplet& tri)
{
	Facet facet;
	facet.index[0] = build_one_point(tri.p1);
	facet.index[1] = build_one_point(tri.p2);
	facet.index[2] = build_one_point(tri.p3);
	facets.push_back(facet);
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
