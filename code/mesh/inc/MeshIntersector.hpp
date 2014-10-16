#ifndef MESH_INTERSECTOR_HPP
#define MESH_INTERSECTOR_HPP

#include "CenterOfMass.hpp"
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

        bool operator==(const FacetIterator& rhs) const
        {
            return not(rhs != *this);
        }

    private:
        VectorOfFacet::const_iterator begin;
        std::vector<size_t>::const_iterator here;
};

class MeshIntersector
{
    public:
        MeshIntersector(const VectorOfVectorOfPoints& mesh);
        MeshIntersector(
                const MeshPtr mesh_                  //!< the mesh to intersect
                );

        /* \brief Update the intersection of the mesh with free surface
         * \details the intersection requires new Vertices/Edges/Facets stored as dynamic data in the end of container members */
        void update_intersection_with_free_surface(const std::vector<double> &immersions //!< the relative immersion of each static vertex of the mesh
                                                   );

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

        std::vector<double> all_relative_immersions; //<! the immersions of all nodes (including the dynamically added ones)
        std::vector<size_t> index_of_emerged_facets;  //!< list of all emerged facets (included the dynamically ones created by split)
        std::vector<size_t> index_of_immersed_facets; //!< list of all immersed facets (included the dynamically ones created by split)

        friend class ImmersedFacetIterator;
        friend class EmergedFacetIterator;

        /**  \brief Computes the volume inside a closed mesh, for the points under the free surface.
          *  \returns Volume of the STL file (in m^3 if the unit in the STL data is m)
          *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest volume_example
          *  \see admesh-0.95
          *  \see Efficient feature extraction for 2D/3D objects in mesh representation, Cha Zhang and Tsuhan Chen, Dept. of Electrical and Computer Engineering, Carnegie Mellon University
          */
        double immersed_volume() const;

        /**  \brief Computes the volume inside a closed mesh, for the points above the free surface.
          *  \returns Volume of the STL file (in m^3 if the unit in the STL data is m)
          *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest volume_example
          *  \see admesh-0.95
          *  \see Efficient feature extraction for 2D/3D objects in mesh representation, Cha Zhang and Tsuhan Chen, Dept. of Electrical and Computer Engineering, Carnegie Mellon University
          */
        double emerged_volume() const;
        CenterOfMass center_of_mass(const FacetIterator& begin, const FacetIterator& end, const Facet& closing_facet) const;
        CenterOfMass center_of_mass(const FacetIterator& begin, const FacetIterator& end) const;
        CenterOfMass center_of_mass(const Facet& f) const;

        /**  \brief used by the 'volume' method to close the mesh.
          *  \details When computing the intersection with free surface, the
          *           algorithm does not close the two resulting meshes (emerged
          *           & immersed mesh).
          *  \returns Facet on free surface & closing the mesh
          *  \snippet mesh/unit_tests/src/MeshIntersectorTest.cpp MeshIntersectorTest compute_closing_facet_example
          */
        Facet compute_closing_facet() const;

        /**  \brief Detect if a facet already exists in mesh.
          *  \details Only compares the indices (not the barycenter or unit normal or area).
          *  \returns True if facet exists, 0 otherwise.
          *  \snippet mesh/unit_tests/src/MeshIntersectorTest.cpp MeshIntersectorTest has_example
          */
        bool has(const Facet& f //!< Facet to check
                 ) const;
        bool has(const Facet& f, //!< Facet to check
                                  const FacetIterator& begin,
                                  const FacetIterator& end
                                 ) const;

        Eigen::MatrixXd convert(const Facet& f) const;
        double facet_volume(const Facet& f) const;
    private:
    /**  \brief Iterate on each edge to find intersection with free surface
      */
        std::vector<size_t> find_intersection_with_free_surface(
                std::vector<size_t>& split_edges,
                std::vector<int>& edges_immersion_status,
                std::vector<bool>& facet_crosses_free_surface);
        /** \brief Iterate on each facet to classify and/or split
         */
        void classify_or_split(const std::vector<size_t>& split_edges,
                               std::vector<bool>& facet_crosses_free_surface,
                               std::vector<int>& edges_immersion_status);
        void reset_dynamic_members();

        double volume(const FacetIterator& begin, const FacetIterator& end) const;
        Facet make(const Facet& f, const size_t i1, const size_t i2, const size_t i3) const;
};

typedef TR1(shared_ptr)<MeshIntersector> MeshIntersectorPtr;
typedef TR1(shared_ptr)<const MeshIntersector> const_MeshIntersectorPtr;

#endif
