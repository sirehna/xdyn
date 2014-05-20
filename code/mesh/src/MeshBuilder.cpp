#include <algorithm>
#include <iostream>

#include "MeshBuilder.hpp"
#include "mesh_manipulations.hpp"

Eigen::Matrix<double,3,Eigen::Dynamic> MeshBuilder::get_nodes() const
{
	return nodes;
}

std::vector<Facet> MeshBuilder::get_facets() const
{
	return facets;
}

MeshBuilder::MeshBuilder(const VectorOfVectorOfPoints& v_) : v(v_),
                                                                   xyzMap(Vector3dMap()),
                                                                   index(0),
                                                                   nodes(Eigen::Matrix<double,3,Eigen::Dynamic>(Eigen::MatrixXd::Zero(3,3*v.size()))),
                                                                   facets(std::vector<Facet>())
{
}

MeshBuilder::MeshBuilder(const VectorOfPoints& tri) : v(VectorOfVectorOfPoints(1,tri)),
                                                            xyzMap(Vector3dMap()),
                                                            index(0),
                                                            nodes(Eigen::Matrix<double,3,Eigen::Dynamic>(Eigen::MatrixXd::Zero(3,3))),
                                                            facets(std::vector<Facet>())
{
}

Eigen::Matrix<double,3,Eigen::Dynamic> MeshBuilder::resize(const Eigen::Matrix<double,3,Eigen::Dynamic>& M) const
{
    Eigen::Matrix<double,3,Eigen::Dynamic> resized(Eigen::MatrixXd::Zero(3,index));
    for (size_t j = 0 ; j < index ; ++j)
    {
        resized.col(j) = M.col(j);
    }
    return resized;
}

Mesh MeshBuilder::build()
{
	*this = std::for_each(v.begin(), v.end(), *this);
	return Mesh(resize(nodes), facets);
}

void MeshBuilder::operator()(const VectorOfPoints& tri)
{
	Facet facet;
    facet.unit_normal = unit_normal(convert(tri));
    facet.area = area(convert(tri));
    facet.barycenter = barycenter(convert(tri));
    for (VectorOfPoints::const_iterator it = tri.begin() ; it != tri.end() ; ++it)
    {
        facet.index.push_back(build_one_point(*it));
    }
    facets.push_back(facet);
}

size_t MeshBuilder::build_one_point(const EPoint& xyz)
{
	const bool point_has_been_added = add_point_if_missing(xyz);
	if (point_has_been_added) index++;
	return xyzMap[xyz];
}

bool MeshBuilder::add_point_if_missing(const EPoint& xyz)
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

bool MeshBuilder::point_is_in_map(const EPoint& xyz)
{
	const Vector3dMap::const_iterator itMap = xyzMap.find(xyz);
	return itMap != xyzMap.end();
}

Matrix3x MeshBuilder::convert(const VectorOfPoints& v) const
{
    Matrix3x ret(3,v.size());
    for (size_t j = 0 ; j < v.size() ; ++j)
    {
        ret.col(j) = v[j];
    }
    return ret;
}
