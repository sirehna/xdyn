#ifndef MESH_INTERSECTOR_HPP
#define MESH_INTERSECTOR_HPP

#include "Mesh.hpp"

class FacetIterator
{
    public:
        FacetIterator(const VectorOfFacet::const_iterator &begin_, std::vector<size_t>::const_iterator &here_) : begin(begin_), here(here_)
        {
        }

        const Facet& operator*() const
        {
            return *(begin+*here);
        }

        const FacetIterator& operator++()
        {
            here++;
            return *this;
        }

        const Facet* operator->() const
        {
            return (begin+*here).operator ->();
        }

        bool operator!=(const FacetIterator& rhs) const
        {
            return (begin != rhs.begin) or (here != rhs.here);
        }

    private:
        VectorOfFacet::const_iterator begin;
        std::vector<size_t>::const_iterator here;
};

class MeshIntersector
{
public:
    MeshIntersector(
            const MeshPtr mesh_,                  //!< the mesh to intersect
            const std::vector<double> &immersions //!< the relative immersion of each static vertex of the mesh
            );

    /* \brief Update the intersection of the mesh with free surface
     * \details the intersection requires new Vertices/Edges/Facets stored as dynamic data in the end of container members */
    void update_intersection_with_free_surface();

    FacetIterator begin_immersed() const
    {
        const std::vector<Facet>::const_iterator target=mesh->facets.begin();
        std::vector<size_t>::const_iterator here = index_of_immersed_facets.begin();
        return FacetIterator(target, here);
    }

    FacetIterator end_immersed() const
    {
        const std::vector<Facet>::const_iterator target=mesh->facets.begin();
        std::vector<size_t>::const_iterator here = index_of_immersed_facets.end();
        return FacetIterator(target, here);
    }

    FacetIterator begin_emerged() const
    {
        std::vector<Facet>::const_iterator target=mesh->facets.begin();
        std::vector<size_t>::const_iterator here = index_of_emerged_facets.begin();
        return FacetIterator(target, here);
    }

    FacetIterator end_emerged() const
    {
        std::vector<Facet>::const_iterator target=mesh->facets.begin();
        std::vector<size_t>::const_iterator here = index_of_emerged_facets.end();
        return FacetIterator(target, here);
    }

    /* \brief Compute the point of intersection with free surface between two vertices
     * \details One of the vertices must be emerged and the other immersed */
    static EPoint edge_intersection(const EPoint& A, const double dzA, const EPoint& B, const double dzB);

    /* \brief split an edge into an emerged and an immersed part */
    size_t split_partially_immersed_edge(
            const size_t edge_index ,                //!< the index of edge to be split
            std::vector<int> &edges_immersion_status //!< the immersion status of each edge
            );

    /* \brief split a Facet into an emerged and an immersed part */
    void split_partially_immersed_facet(
            size_t facet_index,                             //!< the index of facet to be split
            const std::vector<int> &edges_immersion_status, //!< the immersion status of each edge
            const std::vector<size_t> &split_edges          //!< the replacement map for split edges
            );

    /* \brief Extract the coordinates of a specific facet */
    Matrix3x coordinates_of_facet(size_t facet_index) const;

    /* \brief Extract the relative immersions of a specific facet */
    std::vector<double> immersions_of_facet(size_t facet_index) const;

    /* \brief Answer the immersion status corresponding to an edge, knowing relative immersion of its vertices
     * \details The immersion status is composed of three bits
     * - the low bit (status & 1) is immersion status of first vertex (0=emerged, 1=immersed)
     * - the second bit (status & 2) is immersion status of second vertex (0=emerged, 2=immersed)
     * - the high bit (status & 4) is indicating if the edge just touches the free surface (0=not touching 4=touching)
     *
     * The case of totally emerged is thus two low bits = 00
     * The case of totally immersed is when two low bits == 11
     * The other cases 01 or 10 indicate that the edge is crossing the free surface.
     * If an Edge crosses the free surface, then the facets containing this edge do cross the free surface.
     *
     * The edge just touches the free surface if at least one of its vertex has exactly 0 as relative immersion
     * If the edge touches the free surface, it does not cross the free surface;
     * but the facets containing this edge may cross the free surface (if free surface crosses exactly 2 non consecutive vertices)
     */
    static int get_edge_immersion_status(
             const double z0, //!< the relative immersion of first vertex
             const double z1  //!< the relative immersion of second vertex
             );

    /* \brief answer whether this edge crosses the free surface
     */
    static bool crosses_free_surface(int status);

    /* \brief answer whether this edge is totally emerged
     */
    static bool is_emerged(int status);

    /* \brief answer whether this edge is totally immersed
     */
    static bool is_immersed(int status);

    /* \brief answer whether this edge touches the free surface
     */
    static bool just_touches_free_surface(int status);

    MeshPtr mesh;

    std::vector<double> all_immersions; //<! the immersions of all nodes (including the dynamically added ones)
    std::vector<size_t> index_of_emerged_facets;  //!< list of all emerged facets (included the dynamically ones created by split)
    std::vector<size_t> index_of_immersed_facets; //!< list of all immersed facets (included the dynamically ones created by split)
    std::vector<size_t > split_edges;  //!< a table indicating the index of replacing edge for each edge that is split (there are two consecutive edges per split edge, the table only gives the first one)

    friend class ImmersedFacetIterator;
    friend class EmergedFacetIterator;
};

typedef TR1(shared_ptr)<MeshIntersector> MeshIntersectorPtr;
typedef TR1(shared_ptr)<const MeshIntersector> const_MeshIntersectorPtr;

#endif
