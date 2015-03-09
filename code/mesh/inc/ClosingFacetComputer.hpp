/*
 * ClosingFacetComputer.hpp
 *
 *  Created on: Feb 27, 2015
 *      Author: cady
 */


#ifndef CLOSINGFACETCOMPUTER_HPP_
#define CLOSINGFACETCOMPUTER_HPP_

#include <cstdlib> // size_t
#include <map>
#include <utility> // std::pair
#include <set>
#include <vector>

#include <Eigen/Dense>

template <typename T,typename U> bool has(const U& map_or_set, const T idx)
{
    if (map_or_set.find(idx) != map_or_set.end()) return true;
                                                  return false;
}

class ClosingFacetComputer
{
    public:
        struct Contour
        {
            std::vector<size_t> edge_idx;
            std::vector<bool> reversed;
        };

        typedef std::pair<size_t,size_t> Edge;
        typedef std::vector<Edge> ListOfEdges;
        ClosingFacetComputer(const Eigen::Matrix3Xd& mesh, const ListOfEdges& edges, std::vector<size_t> index_of_relevant_edges=std::vector<size_t>());
        ClosingFacetComputer(const Eigen::Matrix3Xd* mesh, const ListOfEdges& edges, std::vector<size_t> index_of_relevant_edges=std::vector<size_t>());

        /**  \brief Takes a list of edges on the surface & groups connected ones together
          *  \returns A list of facets containing their constituting edges
          */
        static std::vector<std::vector<size_t> > group_connected_edges(const ListOfEdges& edges, std::vector<size_t> idx_of_relevant_edges=std::vector<size_t>());

        static std::map<size_t,std::set<size_t> > get_node_to_connected_edges(const ListOfEdges& edges);

        /**  \brief Takes a list of edges on the surface & returns the list of unique nodes appearing in them
          *  \returns A list of node indexes, in the order in which they are use in the edges
          */
        static std::vector<size_t> extract_nodes(const ListOfEdges& edges);

        /**  \returns Index of two nodes guaranteed to be on the frontier
         *   \details Uses the first node at minimal x value & the first node at maximal y value
         *            Won't work in all cases (in particular, for case 17)
         */
        std::pair<size_t,size_t> find_extreme_nodes() const;

        /**  \brief Computes the angle between two edges
         */
        double angle_between_edges(const size_t idx_of_first_edge, const size_t idx_of_second_edge, const bool reverse=false) const;

        /**  \returns All edges connected to the second node of an edge (except that edge, of course)
         */
        std::vector<size_t> edges_connected_to_second_node_of_edge(const size_t edge_idx) const;

        /**  \returns All edges connected to the first node of an edge (except that edge, of course)
         */
        std::vector<size_t> edges_connected_to_first_node_of_edge(const size_t edge_idx) const;

        /**  \returns Next edge in contour
         */
        size_t next_edge(const size_t edge_idx, const bool reverse=false) const;

        /**  \returns Index of two edges guaranteed to be on the contour
         */
        std::pair<size_t,size_t> extreme_edges() const;

        /**  \returns Index of one edge containing given node & guaranteed to be on the contour
         */
        size_t extreme_edge(const size_t extreme_node) const;

        /**  \returns Outside edges, in order.
         */
        Contour contour() const;
        /**  \returns Outside edges, in order
         */
        Contour contour(size_t extreme_edge) const;

    private:
        const Eigen::Matrix3Xd* mesh;
        ListOfEdges edges;
        std::vector<size_t> node_idx_in_mesh;
        std::map<size_t,std::set<size_t> > node_to_connected_edges;
        std::vector<size_t> original_edge_index; //!< Edge index in original mesh
        double xmin;
        double xmax;
        double ymin;
        double ymax;

        struct ConnectedComponents
        {
            std::vector<int> component_idx_per_node;
            int nb_of_components;
        };
        static std::vector<std::set<size_t> > get_edges_per_component(const ConnectedComponents& connected_components, const ListOfEdges& edges_);
        static ClosingFacetComputer::ConnectedComponents get_connected_components(const ListOfEdges& edges);

        bool need_to_reverse(const size_t first_edge, const size_t second_edge, const bool) const;
};

#endif  /* CLOSINGFACETCOMPUTER_HPP_ */
