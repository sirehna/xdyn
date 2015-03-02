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
          *  \details Used to compute the closing facets
          *  \returns A list of facets containing their constituting edges
          */
        static std::vector<std::vector<size_t> > group_connected_edges_into_facets(const std::vector<size_t>& edges_index, const std::vector<std::pair<size_t,size_t> >& edges);
};

#endif  /* CLOSINGFACETCOMPUTER_HPP_ */
