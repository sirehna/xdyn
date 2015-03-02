/*
 * ClosingFacetComputer.hpp
 *
 *  Created on: Feb 27, 2015
 *      Author: cady
 */


#ifndef CLOSINGFACETCOMPUTER_HPP_
#define CLOSINGFACETCOMPUTER_HPP_

#include <cstdlib> // size_t
#include <utility> // std::pair
#include <vector>

typedef std::pair<size_t,size_t> Edge;
typedef std::vector<Edge> ListOfEdges;

class ClosingFacetComputer
{
    public:
        /**  \brief Takes a list of edges on the surface & groups connected ones together
          *  \returns A list of facets containing their constituting edges
          */
        static std::vector<std::vector<size_t> > group_connected_edges_into_facets(const std::vector<size_t>& edges_index, const std::vector<std::pair<size_t,size_t> >& edges);

        /**  \brief Takes a list of edges on the surface & returns the list of unique nodes appearing in them
          *  \returns A list of node indexes, in the order in which they are use in the edges
          */
        static std::vector<size_t> extract_nodes(const std::vector<std::pair<size_t,size_t> >& edges);
};

#endif  /* CLOSINGFACETCOMPUTER_HPP_ */
