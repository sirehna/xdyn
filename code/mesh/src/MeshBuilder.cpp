#include <algorithm>
#include <iostream>

#include "MeshBuilder.hpp"
#include "mesh_manipulations.hpp"

Matrix3x MeshBuilder::get_nodes() const
{
    return nodes;
}

std::vector<Edge> MeshBuilder::get_edges() const
{
    return edges;
}

std::vector<Facet> MeshBuilder::get_facets() const
{
    return facets;
}

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
    return Mesh(resize(nodes), edges , facets, facetsPerEdge , edgesPerFacet , edgesRunningDirection , clockwise);
}

void MeshBuilder::operator()(const VectorOfPoints& list_of_points)
{
    if (not(list_of_points.empty()))
    {
        size_t facet_index=facets.size();
        std::vector<size_t> edges_of_this_facet;
        std::vector<bool> running_directions;
        Facet facet;
        const Matrix3x M = convert(list_of_points);
        facet.unit_normal = unit_normal(M);
        facet.area = area(M);
        facet.barycenter = barycenter(M);
        for (VectorOfPoints::const_iterator it = list_of_points.begin() ; it != list_of_points.end() ; )
        {
            size_t vertex_index = build_one_point(*it);
            facet.vertex_index.push_back(vertex_index);
            ++it;
            size_t edge_index = (it != list_of_points.end())
                ? build_one_edge(Edge(vertex_index,build_one_point(*it)))
                : build_one_edge(Edge(vertex_index,build_one_point(*(list_of_points.begin()))));
            edges_of_this_facet.push_back(edge_index);
            facetsPerEdge.at(edge_index).push_back(facet_index);
            running_directions.push_back( edges.at(edge_index).first_vertex(0) == vertex_index);
        }
        facets.push_back(facet);
        edgesPerFacet.push_back(edges_of_this_facet);
        edgesRunningDirection.push_back(running_directions);
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
    for (size_t i = 0 ; i < (size_t)tri.cols() ; ++i)
    {
        vv.push_back(tri.col(i));
    }
    v.push_back(vv);
    clockwise = oriented_clockwise(v,barycenter(v));
    if (not(v.empty())) nodes = Eigen::MatrixXd::Zero(3,(int)(v.size()*v.front().size()));
}
