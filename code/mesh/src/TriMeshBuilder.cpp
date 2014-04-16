#include <algorithm>

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
	std::for_each(v.begin(), v.end(), *this);
	/*return TriMesh(get_nodes(), get_facets());*/
}

void TriMeshBuilder::operator()(const Point3dTriplet& Tri)
{
	Facet facet;
	facet.index[0] = build_one_point(Tri.p1);
	facet.index[1] = build_one_point(Tri.p2);
	facet.index[2] = build_one_point(Tri.p3);
	facets.push_back(facet);
}

size_t TriMeshBuilder::build_one_point(const Xyz& xyz)
{
	add_point_if_missing(xyz);
	const size_t ret = index;
	get_next_point(xyz);
	return ret;
}

bool TriMeshBuilder::point_is_in_map(const Xyz& xyz)
{
	XyzMap::const_iterator itMap = xyzMap.find(xyz);
	return itMap != xyzMap.end();
}

void TriMeshBuilder::add_point_if_missing(Xyz xyz)
{
	if (not(point_is_in_map(xyz)))
	{
		xyzMap.insert(std::make_pair(xyz,index));
		nodes.push_back(xyz);
	}
}

void TriMeshBuilder::get_next_point(const Xyz& xyz)
{
	if (point_is_in_map(xyz)) index++;
}
