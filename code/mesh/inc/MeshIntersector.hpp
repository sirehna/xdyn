#ifndef MESH_INTERSECTOR_HPP
#define MESH_INTERSECTOR_HPP

#include <set>

#include <ssc/kinematics.hpp>

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
            return *(begin+(long)(*here));
        }

        const FacetIterator& operator++()
        {
            here++;
            return *this;
        }

        const Facet* operator->() const
        {
            return (begin+(long)(*here)).operator ->();
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
        MeshIntersector(const VectorOfVectorOfPoints& mesh, const bool check_orientation=true);
        MeshIntersector(
                const MeshPtr mesh_                  //!< the mesh to intersect
                );

        /**
         * \brief Update the intersection of the mesh with free surface
         * \details the intersection requires new Vertices/Edges/Facets stored as dynamic data in the end of container members
         */
        void update_intersection_with_free_surface(
                const std::vector<double>& relative_immersions, //!< the relative immersion of each static vertex of the mesh
                const std::vector<double>& absolute_wave_elevations  //!< z coordinate in NED frame of each point in mesh
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

        FacetIterator begin_surface()
        {
            if (need_to_update_closing_facet) build_closing_edge();
            std::vector<Facet>::const_iterator target=mesh->facets.begin();
            std::vector<size_t>::const_iterator here = index_of_facets_exactly_on_the_surface.begin();
            return FacetIterator(target, here);
        }

        FacetIterator end_surface()
        {
            if (need_to_update_closing_facet) build_closing_edge();
            std::vector<Facet>::const_iterator target=mesh->facets.begin();
            std::vector<size_t>::const_iterator here = index_of_facets_exactly_on_the_surface.end();
            return FacetIterator(target, here);
        }

        /**
         * \brief Compute the point of intersection with free surface between two vertices
         * \details One of the vertices must be emerged and the other immersed
         */
        static EPoint edge_intersection(const EPoint& A, const double dzA, const EPoint& B, const double dzB);

        /**
         * \brief split an edge into an emerged and an immersed part
         */
        size_t split_partially_immersed_edge(
                const size_t edge_index ,                //!< the index of edge to be split
                std::vector<int> &edges_immersion_status //!< the immersion status of each edge
                );

        /**
         * \brief split a Facet into an emerged and an immersed part
         */
        void split_partially_immersed_facet_and_classify(
                size_t facet_index,                             //!< the index of facet to be split
                const std::vector<int> &edges_immersion_status, //!< the immersion status of each edge
                const std::vector<size_t> &split_edges          //!< the replacement map for split edges
                );

        /**
         * \brief Extract the coordinates of a specific facet
         */
        Matrix3x coordinates_of_facet(size_t facet_index) const;

        /**
         * \brief Extract the relative immersions of a specific facet
         */
        std::vector<double> immersions_of_facet(size_t facet_index) const;

        /**
         * \brief Answer the immersion status corresponding to an edge, knowing relative immersion of its vertices
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

        /**
         * \brief True if one end of the edge has z < 0 & the other has z > 0
         */
        static bool crosses_free_surface(int edge_immersion_status);

        /**
         * \brief True if edge is immersed (at least one edge with z < 0 the other with z <= 0)
         */
        static bool is_emerged(int edge_immersion_status);

        /**
         * \brief True if edge is immersed (at least one edge with z > 0 the other with z >= 0)
         */
        static bool is_immersed(int edge_immersion_status);

        /**
         * \brief True if one of the ends of the edge (but not both) just touch the free surface (z==0)
         */
        static bool one_of_the_ends_just_touches_free_surface(int edge_immersion_status);

        /**
         * \brief True if both ends of the edge just touch the free surface (z==0 for both ends)
         */
        static bool both_ends_just_touch_free_surface(int status);

        MeshPtr mesh;

        std::vector<double> all_relative_immersions;                //!< Relative immersions (z-zwave) of all nodes (including the dynamically added ones)
        std::vector<double> all_absolute_wave_elevations;           //!< Absolute wave elevation (z coordinate in NED frame) of all nodes (including the dynamically added ones)
        std::vector<double> all_absolute_immersions;                //!< Absolute immersion (z coordinate in NED frame) of all nodes (including the dynamically added ones)
        std::vector<size_t> index_of_emerged_facets;                //!< All emerged facets, including the ones dynamically created by split
        std::vector<size_t> index_of_immersed_facets;               //!< All immersed facets, including the ones dynamically created by split
        std::vector<size_t> index_of_facets_exactly_on_the_surface; //!< All facets exactly on the surface (z==0 for all points), including the ones dynamically created by split
        std::set<size_t>    index_of_edges_exactly_on_surface;      //!< Edges exactly on free surface (either generated or static)

        friend class ImmersedFacetIterator;
        friend class EmergedFacetIterator;

        /**  \brief Computes the volume inside a closed mesh, for the points under the free surface.
          *  \returns Volume of the STL file (in m^3 if the unit in the STL data is m)
          *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest volume_example
          *  \see admesh-0.95
          *  \see Efficient feature extraction for 2D/3D objects in mesh representation, Cha Zhang and Tsuhan Chen, Dept. of Electrical and Computer Engineering, Carnegie Mellon University
          */
        double immersed_volume();

        /**  \brief Computes the volume inside a closed mesh, for the points above the free surface.
          *  \returns Volume of the STL file (in m^3 if the unit in the STL data is m)
          *  \snippet mesh/unit_tests/src/mesh_manipulationsTest.cpp mesh_manipulationsTest volume_example
          *  \see admesh-0.95
          *  \see Efficient feature extraction for 2D/3D objects in mesh representation, Cha Zhang and Tsuhan Chen, Dept. of Electrical and Computer Engineering, Carnegie Mellon University
          */
        double emerged_volume();

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

        CenterOfMass center_of_mass_immersed();
        CenterOfMass center_of_mass_emerged();

        std::string display_facet_in_NED(const Facet& facet, const EPoint& mesh_center_in_NED_frame, const ssc::kinematics::RotationMatrix& R_from_ned_to_mesh) const;
        std::string display_edge_in_NED(const size_t idx, const EPoint& mesh_center_in_NED_frame, const ssc::kinematics::RotationMatrix& R_from_ned_to_mesh) const;

        VectorOfVectorOfPoints serialize(const FacetIterator& begin, const FacetIterator& end) const;

    private:
        CenterOfMass center_of_mass(const FacetIterator& begin, const FacetIterator& end, const bool immersed);
        CenterOfMass center_of_mass(const Facet& f) const;
        /**
         * \brief Iterate on each edge to find intersection with free surface
         */
        void find_intersection_with_free_surface(
                std::vector<size_t>& split_edges,
                std::vector<int>& edges_immersion_status,
                std::vector<bool>& facet_crosses_free_surface);
        /**
         * \brief Iterate on each facet to classify and/or split
         */
        void classify_or_split(const std::vector<size_t>& split_edges,
                               const std::vector<bool>& facet_crosses_free_surface,
                               std::vector<int>& edges_immersion_status);

        /**
         * \brief Classify facet based on immersion status
         */
        void classify_facet(const size_t facet_index, const std::vector<int>& edges_immersion_status);

        void reset_dynamic_members();

        double volume(const FacetIterator& begin, const FacetIterator& end) const;
        Facet make(const Facet& f, const size_t i1, const size_t i2, const size_t i3) const;

        void build_closing_edge();
        bool need_to_update_closing_facet;
};

typedef TR1(shared_ptr)<MeshIntersector> MeshIntersectorPtr;
typedef TR1(shared_ptr)<const MeshIntersector> const_MeshIntersectorPtr;

#endif
