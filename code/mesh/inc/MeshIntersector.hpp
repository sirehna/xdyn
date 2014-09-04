#ifndef MESH_INTERSECTOR_HPP
#define MESH_INTERSECTOR_HPP

#include "Mesh.hpp"

class EdgeImmersionStatus
{
    EdgeImmersionStatus():status(0) {}
public:
    EdgeImmersionStatus(int status_):status(status_) {}

    EdgeImmersionStatus(
             const double z0, //!< the relative immersion of first vertex
             const double z1  //!< the relative immersion of second vertex
             );

    /* \brief answer whether this edge crosses the free surface
     */
    bool crosses_free_surface() const;

    /* \brief answer whether this edge is totally emerged
     */
    bool is_emerged() const;

    /* \brief answer whether this edge is totally immersed
     */
    bool is_immersed() const;

    /* \brief answer whether this edge touches the free surface
     */
    bool touches_free_surface() const;

    int status; //!< 2 bits of immersion status: 00=totally emerged, 01=second emerged,first immersed, 10=first emerged,second immersed, 11=totally immersed
                // + third bit = 1 for potentially crossing (if any of the vertices is exactly on free surface)
};

class FacetIterator
{
    public:
        FacetIterator(const VectorOfFacet::const_iterator& begin_, const std::vector<size_t>& list_of_facets_, const size_t p) : begin(begin_), list_of_facets(list_of_facets_), pos(p)
        {
        }

        size_t index() const
        {
            return list_of_facets[pos];
        }

        const Facet& operator*() const
        {
            return *(begin+list_of_facets[pos]);
        }

        const FacetIterator& operator++()
        {
            pos++;
            return *this;
        }

        const Facet* operator->() const
        {
            return (begin+list_of_facets[pos]).operator ->();
        }

        bool operator!=(const FacetIterator& rhs) const
        {
            return (begin!=rhs.begin) or (pos != rhs.pos);
        }

    private:
        VectorOfFacet::const_iterator begin;
        std::vector<size_t> list_of_facets;
        size_t pos;
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
        return FacetIterator(mesh->facets.begin(), index_of_immersed_facets, 0);
    }

    FacetIterator end_immersed() const
    {
        return FacetIterator(mesh->facets.begin(), index_of_immersed_facets, index_of_immersed_facets.size());
    }

    FacetIterator begin_emerged() const
    {
        return FacetIterator(mesh->facets.begin(), index_of_emerged_facets, 0);
    }

    FacetIterator end_emerged() const
    {
        return FacetIterator(mesh->facets.begin(), index_of_emerged_facets, index_of_emerged_facets.size());
    }

    /* \brief Compute the point of intersection with free surface between two vertices
     * \details One of the vertices must be emerged and the other immersed */
    static EPoint edge_intersection(const EPoint& A, const double dzA, const EPoint& B, const double dzB);

    /* \brief split an edge into an emerged and an immersed part */
    size_t split_partially_immersed_edge(
            const size_t edge_index ,                                //!< the index of edge to be split
            std::vector<EdgeImmersionStatus> &edges_immersion_status //!< the immersion status of each edge
            );

    /* \brief split a Facet into an emerged and an immersed part */
    void split_partially_immersed_facet(
            size_t facet_index,                                             //!< the index of facet to be split
            const std::vector<EdgeImmersionStatus> &edges_immersion_status, //!< the immersion status of each edge
            const std::map<size_t,size_t >& added_edges                     //!< the map of split edges
            );

    /* \brief Extract the coordinates of a specific facet */
    Matrix3x coordinates_of_facet(size_t facet_index) const;

    /* \brief Extract the relative immersions of a specific facet */
    std::vector<double> immersions_of_facet(size_t facet_index) const;

    MeshPtr mesh;

    std::vector<double> all_immersions; //<! the immersions of all nodes (including the dynamically added ones)
    std::vector<size_t> index_of_emerged_facets;  //!< list of all emerged facets (included the dynamically ones created by split)
    std::vector<size_t> index_of_immersed_facets; //!< list of all immersed facets (included the dynamically ones created by split)

    friend class ImmersedFacetIterator;
    friend class EmergedFacetIterator;
};

typedef TR1(shared_ptr)<MeshIntersector> MeshIntersectorPtr;
typedef TR1(shared_ptr)<const MeshIntersector> const_MeshIntersectorPtr;

#endif
