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

class ClosingFacetComputer
{
    public:
        typedef std::pair<size_t,size_t> Edge;
        typedef std::vector<Edge> ListOfEdges;
        ClosingFacetComputer(const Eigen::Matrix3Xd& mesh, const ListOfEdges& edges);

        /**  \brief Takes a list of edges on the surface & groups connected ones together
          *  \returns A list of facets containing their constituting edges
          */
        static std::vector<std::vector<size_t> > group_connected_edges(const ListOfEdges& edges);

        static std::map<size_t,std::set<size_t> > get_node_to_connected_edges(const ListOfEdges& edges);

        /**  \brief Takes a list of edges on the surface & returns the list of unique nodes appearing in them
          *  \returns A list of node indexes, in the order in which they are use in the edges
          */
        std::vector<size_t> extract_nodes() const;

        /**  \returns Index of node guaranteed to be on the frontier
         *   \details Uses the first node at minimal x value
         */
        size_t find_extreme_node() const;

        /**  \brief Computes the angle between two edges
         */
        double angle_between_edges(const size_t idx_of_first_edge, const size_t idx_of_second_edge) const;

        /**  \returns All edges connected to the second point of an edge (except that edge, of course)
         */
        std::vector<size_t> edges_connected_to_second_node_of_edge(const size_t edge_idx) const;

        /**  \returns Next edge in contour
         */
        size_t next_edge(const size_t edge_idx) const;

        /**  \returns Index of edge guaranteed to be on the contour
         */
        size_t first_extreme_edge() const;

        /**  \returns Outside edges, in order
         */
        std::vector<size_t> contour() const;

    private:
        Eigen::Matrix3Xd mesh;
        ListOfEdges edges;
        std::vector<size_t> node_idx_in_mesh;
        std::map<size_t,std::set<size_t> > node_to_connected_edges;
};

#endif  /* CLOSINGFACETCOMPUTER_HPP_ */
