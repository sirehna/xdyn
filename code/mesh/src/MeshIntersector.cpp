#include <set>

#include "MeshBuilder.hpp"
#include "MeshIntersector.hpp"
#include "mesh_manipulations.hpp"

MeshIntersector::MeshIntersector(const VectorOfVectorOfPoints& mesh_) : mesh(MeshPtr(new Mesh(MeshBuilder(mesh_).build())))
,all_immersions()
,index_of_emerged_facets()
,index_of_immersed_facets()
{}

MeshIntersector::MeshIntersector(const MeshPtr mesh_)
:mesh(mesh_)
,all_immersions()
,index_of_emerged_facets()
,index_of_immersed_facets()
{}

std::vector<size_t > MeshIntersector::find_intersection_with_free_surface(
        std::vector<size_t>& split_edges,
        std::vector<int>& edges_immersion_status,
        std::vector<bool>& facet_crosses_free_surface)
{
    for (size_t edge_index = 0; edge_index < mesh->static_edges;
            ++edge_index)
    {
        double z0 = all_immersions[mesh->edges[0][edge_index]];
        double z1 = all_immersions[mesh->edges[1][edge_index]];
        int status = get_edge_immersion_status(z0, z1);
        edges_immersion_status[edge_index] = status;
        if (crosses_free_surface(status))
            split_edges[edge_index] = split_partially_immersed_edge(edge_index,
                    edges_immersion_status);

        if (crosses_free_surface(status) || just_touches_free_surface(status))
            for (std::vector<size_t>::const_iterator that_facet =
                    mesh->facetsPerEdge[edge_index].begin();
                    that_facet != mesh->facetsPerEdge[edge_index].end();
                    ++that_facet)
                facet_crosses_free_surface[*that_facet] = true;
    }
    return split_edges;
}

void MeshIntersector::classify_or_split(
        const std::vector<size_t>& split_edges,
        std::vector<bool>& facet_crosses_free_surface,
        std::vector<int>& edges_immersion_status)
{
    // iterate on each facet to classify and/or split
    for (size_t facet_index = 0; facet_index < mesh->static_facets;
            ++facet_index)
    {
        if (facet_crosses_free_surface[facet_index])
        {
            split_partially_immersed_facet(facet_index, edges_immersion_status,
                    split_edges);
        }
        else
        {
            if (is_emerged(
                    edges_immersion_status[mesh->orientedEdgesPerFacet[facet_index][0]
                            >> 1]))
                index_of_emerged_facets.push_back(facet_index);
            else
                index_of_immersed_facets.push_back(facet_index);
        }
    }

}

void MeshIntersector::reset_dynamic_members()
{
    mesh->reset_dynamic_data();
    index_of_emerged_facets.clear();
    index_of_immersed_facets.clear();
    index_of_emerged_facets.reserve(mesh->facets.size());
    index_of_immersed_facets.reserve(mesh->facets.size());
}

void MeshIntersector::update_intersection_with_free_surface(const std::vector<double>& immersions)
{
    all_immersions = immersions;
    reset_dynamic_members();
    std::vector<bool> facet_crosses_free_surface(mesh->static_facets,false);
    std::vector<int> edges_immersion_status(mesh->static_edges,0); // the immersion status of each edge
    std::vector<size_t > split_edges(mesh->static_edges,0);  //!< a table indicating the index of replacing edge for each edge that is split (there are two consecutive edges per split edge, the table only gives the first one)
    find_intersection_with_free_surface(split_edges, edges_immersion_status, facet_crosses_free_surface);
    classify_or_split(split_edges, facet_crosses_free_surface, edges_immersion_status);
}

void MeshIntersector::split_partially_immersed_facet(
        size_t facet_index,                             //!< the index of facet to be split
        const std::vector<int>& edges_immersion_status, //!< the immersion status of each edge
        const std::vector<size_t>& split_edges          //!< the replacement map for split edges
        )
{
    const std::vector<size_t> oriented_edges_of_this_facet = mesh->orientedEdgesPerFacet[facet_index];
    std::vector<size_t> emerged_edges;
    std::vector<size_t> immersed_edges;
    int status=-1;
    size_t first_emerged  = 0;
    size_t first_immersed = 0;

    for(size_t i=0 ; i < oriented_edges_of_this_facet.size() ; ++i)
    {
        size_t oriented_edge = oriented_edges_of_this_facet[i];
        size_t edge_index=Mesh::get_oriented_edge_index(oriented_edge);
        if (is_emerged(edges_immersion_status[edge_index]))
        {
            if(status==3) first_emerged = emerged_edges.size();
            emerged_edges.push_back(oriented_edge);
            status = 0;
        }
        else if (is_immersed(edges_immersion_status[edge_index]))
        {
            if(status==0) first_immersed = immersed_edges.size();
            immersed_edges.push_back(oriented_edge);
            status = 3;
        }
        else
        {
            bool reverse_direction=Mesh::get_oriented_edge_direction(oriented_edge);
            size_t edge1 = split_edges[edge_index];
            size_t edge2 = edge1 + 1; // because edges are always added by pair...
            if(reverse_direction) std::swap(edge1,edge2);
            if(is_emerged(edges_immersion_status[edge1]))
            {
                emerged_edges.push_back(Mesh::make_oriented_edge(edge1,reverse_direction));
                first_immersed = immersed_edges.size();
                immersed_edges.push_back(Mesh::make_oriented_edge(edge2,reverse_direction));
                status = 3;
            }
            else
            {
                immersed_edges.push_back(Mesh::make_oriented_edge(edge1,reverse_direction));
                first_emerged = emerged_edges.size();
                emerged_edges.push_back(Mesh::make_oriented_edge(edge2,reverse_direction));
                status = 0;
            }
        }
    }

    // handle the degenerated cases, when the facet is tangent to free surface
    if(emerged_edges.size() <= 1)
    {
        index_of_immersed_facets.push_back(facet_index);
        return;
    }
    if(immersed_edges.size() <= 1)
    {
        index_of_emerged_facets.push_back(facet_index);
        return;
    }

    // insert the closing edge
    size_t closing_edge_index = mesh->add_edge(
            mesh->first_vertex_of_oriented_edge(immersed_edges[first_immersed]),
            mesh->first_vertex_of_oriented_edge( emerged_edges[ first_emerged]));
    immersed_edges.insert(immersed_edges.begin()+first_immersed,Mesh::make_oriented_edge(closing_edge_index,true));
    emerged_edges.insert( emerged_edges.begin()+ first_emerged,Mesh::make_oriented_edge(closing_edge_index,false));

    // create the Facets
    EPoint unit_normal=(mesh->facets.begin()+facet_index)->unit_normal;
    index_of_emerged_facets.push_back(mesh->create_facet_from_edges(emerged_edges,unit_normal));
    index_of_immersed_facets.push_back(mesh->create_facet_from_edges(immersed_edges,unit_normal));
}

size_t MeshIntersector::split_partially_immersed_edge(
        const size_t edge_index ,                //!< the index of edge to be split
        std::vector<int> &edges_immersion_status //!< the immersion status of each edge
        )
{
    size_t first_vertex_index = mesh->edges[0][edge_index];
    size_t last_vertex_index  = mesh->edges[1][edge_index];
    EPoint A=mesh->all_nodes.col(first_vertex_index);
    EPoint B=mesh->all_nodes.col(last_vertex_index);
    double zA=all_immersions[first_vertex_index];
    double zB=all_immersions[last_vertex_index];
    size_t mid_vertex_index = mesh->add_vertex(edge_intersection(A,zA,B,zB));
    all_immersions.push_back(0);
    size_t first_sub_edge_index = mesh->add_edge(first_vertex_index,mid_vertex_index);
    /* second_sub_edge_index = */ mesh->add_edge(mid_vertex_index,last_vertex_index);
    edges_immersion_status.push_back(((edges_immersion_status[edge_index] & 1)  *3) | 4);
    edges_immersion_status.push_back(((edges_immersion_status[edge_index] & 2)/2*3) | 4);
    return first_sub_edge_index;
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
    std::vector<double> z(n,0.0);
    for(size_t i=0;i<n;++i)
        z[i] = all_immersions[facet->vertex_index[i]];
    return z;
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

int MeshIntersector::get_edge_immersion_status(
        const double z0, //!< Relative immersion of first vertex (in m)
        const double z1  //!< Relative immersion of second vertex (in m)
        )
{
    bool first_is_immersed  = z0 > 0 or (z0 == 0 and z1 > 0);
    bool second_is_immersed = z1 > 0 or (z1 == 0 and z0 > 0);
    bool just_touches = z0 == 0 or z1 == 0;
    return (first_is_immersed?1:0) | (second_is_immersed?2:0) | (just_touches?4:0);
}

bool MeshIntersector::crosses_free_surface(int status)
{
    return ((status & 1) != 0) xor ((status & 2) != 0);
}

bool MeshIntersector::is_emerged(int status)
{
    return (status & 3) == 0;
}

bool MeshIntersector::is_immersed(int status)
{
    return (status & 3) == 3;
}

bool MeshIntersector::just_touches_free_surface(int status)
{
    return ((status & 4) != 0);
}

Facet MeshIntersector::compute_closing_facet() const
{
    Facet ret;
    std::vector<size_t> vertex_index;
    vertex_index.reserve(mesh->node_count);
    EPoint bar(0,0,0);
    double A = 0;
    Eigen::Vector3d n(0,0,0);
    for (size_t i = 0 ; i < mesh->node_count ; ++i)
    {
        if (all_immersions[i] == 0)
        {
            vertex_index.push_back(i);
        }
    }
    if (vertex_index.size() > 2)
    {
        bar = ::barycenter(mesh->all_nodes, vertex_index);
        A = area(mesh->all_nodes, vertex_index);
        n = unit_normal(mesh->all_nodes, vertex_index);
    }
    return Facet(vertex_index, n, bar, A);
}

bool MeshIntersector::has(const Facet& f //!< Facet to check
                         )
{
    if (f.vertex_index.empty()) return false;
    std::set<size_t> s(f.vertex_index.begin(), f.vertex_index.end());
    for (auto that_facet = begin_immersed() ; that_facet != end_immersed() ; ++that_facet)
    {
        std::set<size_t> s_(that_facet->vertex_index.begin(), that_facet->vertex_index.end());
        if (s==s_) return true;
    }
    for (auto that_facet = begin_emerged() ; that_facet != end_emerged() ; ++that_facet)
    {
        std::set<size_t> s_(that_facet->vertex_index.begin(), that_facet->vertex_index.end());
        if (s==s_) return true;
    }
    return false;
}

EPoint MeshIntersector::barycenter(const FacetIterator& begin, const FacetIterator& end) const
{
    EPoint ret(0,0,0);
    size_t n = 0;
    for (auto that_facet = begin ; that_facet != end ; ++that_facet)
    {
        ret += that_facet->barycenter;
    }
    ret /= double(std::max(n, size_t(1)));
    return ret;
}

double MeshIntersector::volume(const FacetIterator& begin, const FacetIterator& end) const
{
    double volume = 0;
    // Choose a point (any point) as the reference
    const EPoint P0(0,0,0);// = begin->barycenter;//mesh->all_nodes.col(begin->vertex_index.front());
    size_t n = 0;
    for (auto that_facet = begin ; that_facet != end ; ++that_facet)
    {
        const auto P = mesh->all_nodes.col(that_facet->vertex_index.front());
        const auto dP = P-P0;
        // Dot product to get distance from point to plane
        const double height = that_facet->unit_normal.dot(dP);
        volume += (that_facet->area * height) / 3.0;
        ++n;
    }
    if (n < 3) return 0;
    return volume;
}

double MeshIntersector::immersed_volume() const
{
    return volume(begin_immersed(), end_immersed());
}

double MeshIntersector::emerged_volume() const
{
    return volume(begin_emerged(), end_emerged());
}
