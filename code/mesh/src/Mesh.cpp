#include <map>
#include <set>
#include <vector>
#include <iostream>
#include "Mesh.hpp"
#include "mesh_manipulations.hpp"

Edge::Edge(size_t v1,size_t v2)
{
    vertex_index[0]=v1;
    vertex_index[1]=v2;
}

size_t Edge::first_vertex(bool reverse_direction) const
{
    return vertex_index[reverse_direction];
}

size_t Edge::second_vertex(bool reverse_direction) const
{
    return vertex_index[1-reverse_direction];
}

bool EdgeImmersionStatus::crosses_free_surface() const
{
    return ((status & 1) != 0) xor ((status & 2) != 0);
}
bool EdgeImmersionStatus::is_emerged() const
{
    return (status & 3) == 0;
}
bool EdgeImmersionStatus::is_immersed() const
{
    return (status & 3) == 3;
}
bool EdgeImmersionStatus::touches_free_surface() const
{
    return crosses_free_surface() or ((status & 4) != 0);
}

EdgeImmersionStatus::EdgeImmersionStatus(
        const double z0, //!< the relative immersion of first vertex
        const double z1  //!< the relative immersion of second vertex
        )
{
    bool first_is_immersed  = z0 > 0 or (z0 == 0 and z1 > 0);
    bool second_is_immersed = z1 > 0 or (z1 == 0 and z0 > 0);
    bool just_touches = z0 == 0 or z1 == 0;
    status = (unsigned char)((first_is_immersed?1:0) | (second_is_immersed?2:0) | (just_touches?4:0));
}

Mesh::Mesh():nodes(Matrix3x()),edges(std::vector<Edge>()),facets(std::vector<Facet>()), orientation_factor(1)
{
}

Mesh::Mesh(const Matrix3x& nodes_,
        const std::vector<Edge>& edges_,
        const std::vector<Facet>& facets_,
        const std::vector<std::vector<size_t> >& facetsPerEdge_ , //!< for each Edge (index), the list of Facet (indices) to which the edge belongs
        const std::vector<std::vector<OrientedEdge> >& edgesPerFacet_,  //!< for each Facet (index), the list of Edges (indices) composing the facet
        const bool clockwise)
:nodes(nodes_)
,edges(edges_)
,facets(facets_)
,facetsPerEdge(facetsPerEdge_)
,edgesPerFacet(edgesPerFacet_)
,static_nodes(nodes_.cols())
,static_edges(edges_.size())
,static_facets(facets_.size())
,all_nodes(3,static_nodes+static_edges)
,node_count(static_nodes)
,orientation_factor(clockwise ? -1 : 1)
{
    Matrix3x room_for_dynamic_vertices(3,all_nodes.cols()-nodes.cols());
    room_for_dynamic_vertices.fill(0);
    all_nodes << nodes , room_for_dynamic_vertices;
}

MeshIntersector::MeshIntersector(const MeshPtr mesh_,const std::vector<double> &immersions)
:mesh(mesh_)
,all_immersions(immersions)
,index_of_emerged_facets()
,index_of_immersed_facets()
{}

void Mesh::reset_dynamic_data()
{
    node_count = static_nodes;
    edges.erase( edges.begin() + static_edges , edges.end());
    facets.erase( facets.begin() + static_facets , facets.end());
}

void MeshIntersector::update_intersection_with_free_surface()
{
    mesh->reset_dynamic_data();
    index_of_emerged_facets.clear();
    index_of_immersed_facets.clear();

    std::map<size_t,size_t > added_edges;
    std::set<size_t> set_of_facets_crossing_free_surface; //!< list of facets to be split into an emerged and immersed parts
    std::vector<EdgeImmersionStatus> edges_immersion_status(mesh->static_edges,EdgeImmersionStatus(0)); //!< the immersion status of each edge

    // iterate on each edge to find intersection with free surface
    for( size_t edge_index=0 ; edge_index < mesh->static_edges ; ++edge_index ) {
        double z0 = all_immersions[mesh->edges[edge_index].vertex_index[0]];
        double z1 = all_immersions[mesh->edges[edge_index].vertex_index[1]];
        EdgeImmersionStatus status = EdgeImmersionStatus(z0,z1);
        edges_immersion_status[edge_index]=status;
        if (status.touches_free_surface())
            set_of_facets_crossing_free_surface.insert(mesh->facetsPerEdge[edge_index].begin(),mesh->facetsPerEdge[edge_index].end());
        if (status.crosses_free_surface())
            added_edges[edge_index] = split_partially_immersed_edge(edge_index,edges_immersion_status);
    }

    // iterate on each facet to classify and/or split
    for(size_t facet_index = 0 ; facet_index < mesh->static_facets ; ++facet_index) {
        if( set_of_facets_crossing_free_surface.find(facet_index) != set_of_facets_crossing_free_surface.end()) {
            split_partially_immersed_facet(facet_index,edges_immersion_status,added_edges);
        } else {
            if(edges_immersion_status[mesh->edgesPerFacet[facet_index][0].edge_index].is_emerged())
                index_of_emerged_facets.push_back(facet_index);
            else
                index_of_immersed_facets.push_back(facet_index);
        }
    }
}

Matrix3x MeshIntersector::coordinates_of_facet(size_t facet_index) const
{
    std::vector<Facet>::const_iterator facet=mesh->facets.begin()+facet_index;
    size_t n = facet->vertex_index.size();
    Matrix3x coord(3,n);
    for(size_t i=0;i<n;++i)
        coord.col(i) = mesh->all_nodes.col(facet->vertex_index[i]);
    return coord;
}

std::vector<double> MeshIntersector::immersions_of_facet(size_t facet_index) const
{
    std::vector<Facet>::const_iterator facet=mesh->facets.begin()+facet_index;
    size_t n = facet->vertex_index.size();
    std::vector<double> z;
    for(size_t i=0;i<n;++i)
        z.push_back( all_immersions[facet->vertex_index[i]]);
    return z;
}

void MeshIntersector::split_partially_immersed_facet(
        size_t facet_index,                                             //!< the index of facet to be split
        const std::vector<EdgeImmersionStatus> &edges_immersion_status, //!< the immersion status of each edge
        const std::map<size_t,size_t >& added_edges                     //!< the map of split edges
        )
{
    std::vector<OrientedEdge> edges_of_this_facet = mesh->edgesPerFacet[facet_index];
    std::vector<OrientedEdge> emerged_edges;
    std::vector<OrientedEdge> immersed_edges;
    int status=-1;
    size_t first_emerged  = 0;
    size_t first_immersed = 0;

    for( std::vector<OrientedEdge>::const_iterator edge=edges_of_this_facet.begin(); edge != edges_of_this_facet.end() ; ++edge) {
        size_t edge_index=edge->edge_index;
        bool reverse_direction=edge->reverse_direction;
        if(edges_immersion_status[edge_index].is_emerged()) {
            if(status==3) first_emerged = emerged_edges.size();
            emerged_edges.push_back(*edge);
            status = 0;
        } else if(edges_immersion_status[edge_index].is_immersed()) {
            if(status==0) first_immersed = immersed_edges.size();
            immersed_edges.push_back(*edge);
            status = 3;
        } else {
            size_t edge1 = added_edges.at(edge_index);
            size_t edge2 = edge1 + 1; // because edges are always added by pair...
            if(reverse_direction) {
                std::swap(edge1,edge2);
            }
            if(edges_immersion_status[edge1].is_emerged()) {
                emerged_edges.push_back(OrientedEdge(edge1,reverse_direction));
                first_immersed = immersed_edges.size();
                immersed_edges.push_back(OrientedEdge(edge2,reverse_direction));
                status = 3;
            } else {
                immersed_edges.push_back(OrientedEdge(edge1,reverse_direction));
                first_emerged = emerged_edges.size();
                emerged_edges.push_back(OrientedEdge(edge2,reverse_direction));
                status = 0;
            }
        }
    }

    // handle the degenerated cases, when the facet is tangent to free surface
    if(emerged_edges.size() <= 1) {
        index_of_immersed_facets.push_back(facet_index);
        return;
    }
    if(immersed_edges.size() <= 1) {
        index_of_emerged_facets.push_back(facet_index);
        return;
    }

    // insert the closing edge
    size_t closing_edge_index = mesh->add_edge(
            mesh->edges[immersed_edges[first_immersed].edge_index].first_vertex(immersed_edges[first_immersed].reverse_direction),
            mesh->edges[ emerged_edges[ first_emerged].edge_index].first_vertex( emerged_edges [first_emerged].reverse_direction));
    immersed_edges.insert(immersed_edges.begin()+first_immersed,OrientedEdge(closing_edge_index,true));
     emerged_edges.insert( emerged_edges.begin()+ first_emerged,OrientedEdge(closing_edge_index,false));

    // create the Facets
    EPoint unit_normal=(mesh->facets.begin()+facet_index)->unit_normal;
    index_of_emerged_facets.push_back(mesh->create_facet_from_edges(emerged_edges,unit_normal));
    index_of_immersed_facets.push_back(mesh->create_facet_from_edges(immersed_edges,unit_normal));
}

size_t Mesh::create_facet_from_edges(const std::vector<OrientedEdge>& edge_list,const EPoint &unit_normal)
{
    Facet added_facet;
    std::vector<size_t> vertex_list;
    Matrix3x coords(3,edge_list.size());
    for( size_t ei=0;ei<edge_list.size();ei++) {
        size_t vertex_index = edges[edge_list[ei].edge_index].second_vertex(edge_list[ei].reverse_direction); // Note: use second vertex rather than first for compatibility with existing tests
        vertex_list.push_back(vertex_index);
        coords.col(ei)=all_nodes.col(vertex_index);
    }
    added_facet.vertex_index = vertex_list;
    added_facet.area         = ::area(coords);
    added_facet.unit_normal  = unit_normal;
    added_facet.barycenter   = ::barycenter(coords);
    size_t facet_index = facets.size();
    facets.push_back(added_facet);
    return facet_index;
}

size_t MeshIntersector::split_partially_immersed_edge(
        const size_t edge_index ,                                //!< the index of edge to be split
        std::vector<EdgeImmersionStatus> &edges_immersion_status //!< the immersion status of each edge
        )
{
    size_t first_vertex_index = mesh->edges[edge_index].vertex_index[0];
    size_t last_vertex_index  = mesh->edges[edge_index].vertex_index[1];
    EPoint A=mesh->all_nodes.col(first_vertex_index);
    EPoint B=mesh->all_nodes.col(last_vertex_index);
    double zA=all_immersions[first_vertex_index];
    double zB=all_immersions[last_vertex_index];
    size_t mid_vertex_index = mesh->add_vertex(edge_intersection(A,zA,B,zB));
    all_immersions.push_back(0);
    size_t first_sub_edge_index = mesh->add_edge(first_vertex_index,mid_vertex_index);
    /* second_sub_edge_index = */ mesh->add_edge(mid_vertex_index,last_vertex_index);
    edges_immersion_status.push_back(EdgeImmersionStatus((unsigned char)((edges_immersion_status[edge_index].status & 1) | 4)));
    edges_immersion_status.push_back(EdgeImmersionStatus((unsigned char)((edges_immersion_status[edge_index].status & 2) | 4)));
    return first_sub_edge_index;
}

size_t Mesh::add_edge(const size_t first_vertex_index,const size_t last_vertex_index)
{
    edges.push_back(Edge(first_vertex_index,last_vertex_index));
    return edges.size()-1; // that is the index of first_sub_edge added
}

size_t Mesh::add_vertex(const EPoint &vertex_coords)
{
    size_t node_index = node_count;
    all_nodes.col(node_index) = vertex_coords;
    ++node_count;
    return node_index;
}

EPoint MeshIntersector::edge_intersection(const EPoint& A, const double dzA, const EPoint& B, const double dzB)
{
    const double xA = A(0);
    const double xB = B(0);
    const double yA = A(1);
    const double yB = B(1);
    const double zA = A(2);
    const double zB = B(2);
    const double k = dzA/(dzA-dzB);
    return EPoint(xA + k*(xB-xA),
                  yA + k*(yB-yA),
                  zA + k*(zB-zA)
                  );
}

