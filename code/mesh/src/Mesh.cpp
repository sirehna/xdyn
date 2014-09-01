#include <map>
#include <set>
#include <vector>
#include "Mesh.hpp"
#include "mesh_manipulations.hpp"

Edge::Edge(size_t v1,size_t v2)
{
    vertex_index[0]=v1;
    vertex_index[1]=v2;
}

size_t Edge::first_vertex(int direction) const
{
    return vertex_index[direction];
}

size_t Edge::second_vertex(int direction) const
{
    return vertex_index[1-direction];
}
bool Edge::crosses_free_surface() const
{
    return status == 1 || status == 2;
}
bool Edge::is_emerged() const
{
    return status == 0;
}
bool Edge::is_immerged() const
{
    return status == 3;
}

void Edge::update_intersection_with_free_surface(
        const std::vector<double>& relative_immersions //!< the relative immersion of each static vertex of the mesh
        )
{
    double z_0 = relative_immersions[vertex_index[0]];
    double z_1 = relative_immersions[vertex_index[1]];
    bool first_is_immerged  = z_0 < 0 or (z_0 == 0 and z_1 < 0);
    bool second_is_immerged = z_1 < 0 or (z_1 == 0 and z_0 < 0);
    status = (unsigned char)((first_is_immerged?1:0) | (second_is_immerged?2:0));
}

Mesh::Mesh():nodes(Matrix3x()),edges(std::vector<Edge>()),facets(std::vector<Facet>()), orientation_factor(1)
{
}

Mesh::Mesh(const Matrix3x& nodes_,
        const std::vector<Edge>& edges_,
        const std::vector<Facet>& facets_,
        const std::vector<std::vector<size_t> >& facetsPerEdge_ , //!< for each Edge (index), the list of Facet (indices) to which the edge belongs
        const std::vector<std::vector<size_t> >& edgesPerFacet_,  //!< for each Facet (index), the list of Edges (indices) composing the facet
        const std::vector<std::vector<bool> >& edgesRunningDirection_, //!< for each Facet (index), the running direction of each edge
        const bool clockwise)
:nodes(nodes_)
,edges(edges_)
,facets(facets_)
,facetsPerEdge(facetsPerEdge_)
,edgesPerFacet(edgesPerFacet_)
,edgesRunningDirection(edgesRunningDirection_)
,static_nodes(nodes_.cols())
,static_edges(edges_.size())
,static_facets(facets_.size())
,all_nodes(3,static_nodes+static_edges)
,node_count(static_nodes)
,orientation_factor(clockwise ? -1 : 1)
,all_immersions()
,set_of_facets_crossing_free_surface()
,set_of_facets_emerged()
,set_of_facets_immerged()
{
    Matrix3x room_for_dynamic_vertices(3,static_edges);
    room_for_dynamic_vertices.fill(0);
    all_nodes << nodes , room_for_dynamic_vertices;
}

void Mesh::reset_dynamic_data()
{
    node_count = static_nodes;
    edges.erase( edges.begin() + static_edges , edges.end());
    facets.erase( facets.begin() + static_facets , facets.end());
}

void Mesh::update_intersection_with_free_surface(
        const std::vector<double>& relative_immersions //!< the relative immersion of each static vertex of the mesh
        )
{
    reset_dynamic_data();

    all_immersions = relative_immersions;
    std::map<size_t,size_t > added_edges;

    // iterate on each edge to find intersection with free surface
    std::vector<Edge>::iterator edge = edges.begin();
    for( size_t edge_index=0 ; edge_index < static_edges ; ++edge , ++edge_index ) {
        edge->update_intersection_with_free_surface(relative_immersions);
        if (edge->crosses_free_surface()) {
            set_of_facets_crossing_free_surface.insert(facetsPerEdge[edge_index].begin(),facetsPerEdge[edge_index].end());
            added_edges[edge_index] = split_partially_immerged_edge(*edge,all_immersions);
        }
    }

    // iterate on each facet to classify and/or split
    for(size_t facet_index = 0 ; facet_index < static_facets ; ++facet_index) {
        if( set_of_facets_crossing_free_surface.find(facet_index) != set_of_facets_crossing_free_surface.end()) {
            split_partially_immerged_facet(facet_index,added_edges);
        } else {
            if(edges[edgesPerFacet[facet_index][0]].is_emerged())
                set_of_facets_emerged.insert(facet_index);
            else
                set_of_facets_immerged.insert(facet_index);
        }
    }
}

void Mesh::split_partially_immerged_facet(
        size_t facet_index,
        const std::map<size_t,size_t >& added_edges)
{
    std::vector<size_t> edges_of_this_facet = edgesPerFacet[facet_index];
    std::vector<size_t> emerged_edges;
    std::vector<size_t> immerged_edges;
    std::vector<bool> emerged_dirs;
    std::vector<bool> immerged_dirs;
    int status=-1;
    size_t first_emerged = 0;
    size_t first_immerged = 0;
    for( size_t ei=0;ei<edges_of_this_facet.size();ei++) {
        size_t edge_index=edges_of_this_facet[ei];
        bool dir=edgesRunningDirection[facet_index][ei];
        if(edges[edge_index].is_emerged()) {
            if(status==3) first_emerged = emerged_edges.size();
            emerged_edges.push_back(edge_index);
            emerged_dirs.push_back(dir);
            status = 0;
        } else if(edges[edge_index].is_immerged()) {
            if(status==0) first_immerged = immerged_edges.size();
            immerged_edges.push_back(edge_index);
            immerged_dirs.push_back(dir);
            status = 3;
        } else {
            size_t edge1 = added_edges.at(edge_index);
            size_t edge2 = edge1 + 1; // because edges are always added by pair...
            if(dir) {
                std::swap(edge1,edge2);
            }
            if(edges[edge1].is_emerged()) {
                emerged_edges.push_back(edge1);
                emerged_dirs.push_back(dir);
                first_immerged = immerged_edges.size();
                immerged_edges.push_back(edge2);
                immerged_dirs.push_back(dir);
                status = 3;
            } else {
                immerged_edges.push_back(edge1);
                immerged_dirs.push_back(dir);
                first_emerged = emerged_edges.size();
                emerged_edges.push_back(edge2);
                emerged_dirs.push_back(dir);
                status = 0;
            }
        }
    }
    if(emerged_edges.size() == 1 or immerged_edges.size() == 1) return; // degenerated case...

    // insert the closing edge
    size_t closing_edge_index = edges.size();
    Edge closing_edge(
            edges[immerged_edges[first_immerged]].first_vertex(immerged_dirs[first_immerged]),
            edges[emerged_edges[first_emerged]].first_vertex(emerged_dirs[first_emerged]));
    edges.push_back(closing_edge);
    immerged_edges.insert(immerged_edges.begin()+first_immerged,closing_edge_index);
    immerged_dirs.insert(immerged_dirs.begin()+first_immerged,true);
    emerged_edges.insert(emerged_edges.begin()+first_emerged,closing_edge_index);
    emerged_dirs.insert(emerged_dirs.begin()+first_emerged,false);

    // create the Facets
    set_of_facets_emerged.insert(create_facet_from_edges(emerged_edges,emerged_dirs));
    set_of_facets_immerged.insert(create_facet_from_edges(immerged_edges,immerged_dirs));
}

size_t Mesh::create_facet_from_edges(const std::vector<size_t> edge_list,const std::vector<bool> dirs)
{
    Facet added_facet;
    std::vector<size_t> vertex_list;
    Matrix3x coords(3,edge_list.size());
    for( size_t ei=0;ei<edge_list.size();ei++) {
        vertex_list.push_back(edges[edge_list[ei]].first_vertex(dirs[ei]?1:0));
        coords.col(ei)=all_nodes.col(ei);
    }
    added_facet.vertex_index = vertex_list;
    added_facet.area         = ::area(coords);
    added_facet.unit_normal  = ::unit_normal(coords);
    added_facet.barycenter   = ::barycenter(coords);
    size_t facet_index = facets.size();
    facets.push_back(added_facet);
    return facet_index;
}

size_t Mesh::split_partially_immerged_edge(Edge &edge,std::vector<double>& all_immersions)
{
    EPoint A=nodes.col(edge.first_vertex(0));
    EPoint B=nodes.col(edge.second_vertex(0));
    double zA=all_immersions[edge.first_vertex(0)];
    double zB=all_immersions[edge.second_vertex(0)];
    all_nodes.col(node_count++) = edge_intersection(A,zA,B,zB);
    all_immersions.push_back(0);
    Edge first_sub_edge(edge.first_vertex(0),node_count);
    Edge second_sub_edge(node_count,edge.second_vertex(0));
    first_sub_edge.update_intersection_with_free_surface(all_immersions);
    second_sub_edge.update_intersection_with_free_surface(all_immersions);
    edges.push_back(first_sub_edge);
    edges.push_back(second_sub_edge);
    return edges.size()-2;
}

EPoint Mesh::edge_intersection(const EPoint& A, const double dzA, const EPoint& B, const double dzB) const
{
    //if (dzA*dzB>0)
    //{
    //    THROW(__PRETTY_FUNCTION__, HydrostaticException, "zB & zA must have different signs");
    //}
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
