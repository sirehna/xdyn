#include <algorithm>
#include <iostream>
#include "TriMeshBuilder.hpp"
#include "MeshException.hpp"

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

void TriMeshBuilder::operator()(const std::vector<Eigen::Vector3d>& tri)
{
	Facet facet;
    facet.unit_normal = unit_normal(tri);
    facet.area = area(tri);
    facet.barycenter = barycenter(tri);
    facet.index[0] = build_one_point(tri[0]);
    facet.index[1] = build_one_point(tri[1]);
    facet.index[2] = build_one_point(tri[2]);
    facets.push_back(facet);
}

Eigen::Vector3d TriMeshBuilder::barycenter(const std::vector<Eigen::Vector3d>& tri) const
{
	Eigen::Vector3d xyz;
	xyz = (tri[0]+tri[1]+tri[2])/3.0;
	return xyz;
}

Eigen::Vector3d TriMeshBuilder::unit_normal(const std::vector<Eigen::Vector3d>& tri) const
{
	const Eigen::Vector3d n = normal(tri);
	const double norm = n.norm();
	if (norm<1000*std::numeric_limits<double>::epsilon())
	{
	    std::stringstream ss;
	    ss << "Input is degenerated: cannot compute unit normal vector. The polygon is:" << std::endl;
	    for (size_t i = 0 ; i < tri.size() ; ++i)
	    {
	      ss << "p[" << i << "] = " << tri[i].transpose() << std::endl;
	    }
	    THROW(__PRETTY_FUNCTION__, MeshException, ss.str());
	}
	return n/norm;
}

Eigen::Vector3d TriMeshBuilder::normal(const std::vector<Eigen::Vector3d>& tri) const
{
    if (tri.size() < 3)
    {
        std::stringstream ss;
        ss << "Need at least three points to define a surface: cannot compute normal vector. Input has "
           << tri.size() << " points.";
       THROW(__PRETTY_FUNCTION__, MeshException, ss.str());
    }
	const Eigen::Vector3d n1(tri[1]-tri[0]);
	const Eigen::Vector3d n2(tri[2]-tri[0]);
	return n1.cross(n2);
}

double TriMeshBuilder::area(const std::vector<Eigen::Vector3d>& tri) const
{
	const Eigen::Vector3d n1(tri[1]-tri[0]);
	const Eigen::Vector3d n2(tri[2]-tri[0]);
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
