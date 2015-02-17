#include <algorithm>
#include <iostream>

#include "MeshBuilder.hpp"
#include "mesh_manipulations.hpp"

MeshBuilder::MeshBuilder(const VectorOfVectorOfPoints& v_) : v(v_),
                                                                   xyzMap(Vector3dMap()),
                                                                   edgeMap(EdgeMap()),
                                                                   index(0),
                                                                   edgeIndex(0),
                                                                   nodes(Matrix3x()),
                                                                   edges(std::vector<Edge>()),
                                                                   facets(std::vector<Facet>()),
                                                                   clockwise(oriented_clockwise(v,barycenter(v)))
{
    if (not(v.empty())) nodes = Eigen::MatrixXd::Zero(3,(int)(v.size()*v.front().size()));
}

MeshBuilder::MeshBuilder(const VectorOfPoints& tri) : v(VectorOfVectorOfPoints(1,tri)),
                                                            xyzMap(Vector3dMap()),
                                                            edgeMap(EdgeMap()),
                                                            index(0),
                                                            edgeIndex(0),
                                                            nodes(Matrix3x()),
                                                            edges(std::vector<Edge>()),
                                                            facets(std::vector<Facet>()),
                                                            clockwise(false)
{
    if (not(tri.empty())) nodes = Eigen::MatrixXd::Zero(3,(int)(v.size()*v.front().size()));
}

Matrix3x MeshBuilder::resize(const Matrix3x& M) const
{
    Matrix3x resized(Eigen::MatrixXd::Zero(3,(int)index));
    for (size_t j = 0 ; j < index ; ++j)
    {
        resized.col((int)j) = M.col((int)j);
    }
    return resized;
}

Mesh MeshBuilder::build()
{
    *this = std::for_each(v.begin(), v.end(), *this);
    std::array<std::vector<size_t>,2> edges_for_mesh;
    edges_for_mesh[0].reserve(edges.size());
    edges_for_mesh[1].reserve(edges.size());
    for(size_t i=0;i<edges.size();i++) {
        edges_for_mesh[0].push_back(edges[i].vertex_index[0]);
        edges_for_mesh[1].push_back(edges[i].vertex_index[1]);
    }
    return Mesh(resize(nodes), edges_for_mesh , facets, facetsPerEdge , orientedEdgesPerFacet , clockwise);
}

void MeshBuilder::operator()(const VectorOfPoints& list_of_points)
{
    if (not(list_of_points.empty()))
    {
        size_t facet_index=facets.size();
        std::vector<size_t> oriented_edges_of_this_facet;
        Facet facet;
        const Matrix3x M = convert(list_of_points);
        facet.unit_normal = unit_normal(M);
        facet.area = area(M);
        facet.centre_of_gravity = centre_of_gravity(M);
        for (VectorOfPoints::const_iterator it = list_of_points.begin() ; it != list_of_points.end() ; )
        {
            size_t vertex_index = build_one_point(*it);
            facet.vertex_index.push_back(vertex_index);
            ++it;
            size_t edge_index = (it != list_of_points.end())
                ? build_one_edge(Edge(vertex_index,build_one_point(*it)))
                : build_one_edge(Edge(vertex_index,build_one_point(*(list_of_points.begin()))));
            bool reverse_direction = edges.at(edge_index).vertex_index[1] == vertex_index;
            oriented_edges_of_this_facet.push_back(Mesh::make_oriented_edge(edge_index,reverse_direction));
            facetsPerEdge.at(edge_index).push_back(facet_index);
        }
        facets.push_back(facet);
        orientedEdgesPerFacet.push_back(oriented_edges_of_this_facet);
    }
}

size_t MeshBuilder::build_one_edge(const Edge& e)
{
    const bool edge_has_been_added = add_edge_if_missing(e);
    if (edge_has_been_added) {
        std::vector<size_t> listOfFacets;
        facetsPerEdge.push_back(listOfFacets);
        edgeIndex++;
    }
    return edgeMap[e];
}

size_t MeshBuilder::build_one_point(const EPoint& xyz)
{
    const bool point_has_been_added = add_point_if_missing(xyz);
    if (point_has_been_added) index++;
    return xyzMap[xyz];
}

bool MeshBuilder::add_edge_if_missing(const Edge& e)
{
    bool edge_has_been_added = false;
    if (not(edge_is_in_map(e)))
    {
        edgeMap.insert(std::make_pair(e,edgeIndex));
        edges.push_back(e);
        edge_has_been_added = true;
    }
    return edge_has_been_added;
}

bool MeshBuilder::add_point_if_missing(const EPoint& xyz)
{
    bool point_has_been_added = false;
    if (not(point_is_in_map(xyz)))
    {
        xyzMap.insert(std::make_pair(xyz,index));
        nodes.col((int)index) = xyz;
        point_has_been_added = true;
    }
    return point_has_been_added;
}

bool MeshBuilder::edge_is_in_map(const Edge& e)
{
    const EdgeMap::const_iterator itMap = edgeMap.find(e);
    return itMap != edgeMap.end();
}

bool MeshBuilder::point_is_in_map(const EPoint& xyz)
{
    const Vector3dMap::const_iterator itMap = xyzMap.find(xyz);
    return itMap != xyzMap.end();
}

MeshBuilder::MeshBuilder(const Matrix3x& tri) : v(VectorOfVectorOfPoints()),
                                                xyzMap(Vector3dMap()),
                                                edgeMap(EdgeMap()),
                                                index(0),
                                                edgeIndex(0),
                                                nodes(Matrix3x()),
                                                edges(std::vector<Edge>()),
                                                facets(std::vector<Facet>()),
                                                clockwise(false)
{
    VectorOfPoints vv;
    for (int i = 0 ; i < tri.cols() ; ++i)
    {
        vv.push_back(tri.col(i));
    }
    v.push_back(vv);
    clockwise = oriented_clockwise(v,barycenter(v));
    if (not(v.empty())) nodes = Eigen::MatrixXd::Zero(3,(int)(v.size()*v.front().size()));
}
