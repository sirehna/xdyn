#include <set>
#include <sstream>

#include <ssc/exception_handling.hpp>

class ClosingFacetComputerException: public ssc::exception_handling::Exception
{
    public:
        ClosingFacetComputerException(const char* s) :
               Exception(s)
        {
        }
};

#include <ssc/macros.hpp>
#include TR1INC(unordered_map)



#include "ClosingFacetComputer.hpp"


void check_nodes_appear_just_once_as_first_or_second_node_in_edge(const size_t current_edge_idx,
                                                                  const std::pair<size_t,size_t>& edge,
                                                                  const TR1(unordered_map)<size_t,size_t>& node_idx_to_idx_of_first_node_in_edge,
                                                                  const TR1(unordered_map)<size_t,size_t>& node_idx_to_idx_of_second_node_in_edge);
void check_nodes_appear_just_once_as_first_or_second_node_in_edge(const size_t current_edge_idx,
                                                                  const std::pair<size_t,size_t>& edge,
                                                                  const TR1(unordered_map)<size_t,size_t>& node_idx_to_idx_of_first_node_in_edge,
                                                                  const TR1(unordered_map)<size_t,size_t>& node_idx_to_idx_of_second_node_in_edge)
{
    std::stringstream ss;
    const auto it1 = node_idx_to_idx_of_first_node_in_edge.find(edge.first);
    if (it1 != node_idx_to_idx_of_first_node_in_edge.end())
    {
        ss << "Node " << edge.first << " appears twice as a first node (once for edge " << it1->second
           << ", and once for current edge " << current_edge_idx << " (all indexes start at 0)";
        THROW(__PRETTY_FUNCTION__, ClosingFacetComputerException, ss.str());
    }
    const auto it2 = node_idx_to_idx_of_second_node_in_edge.find(edge.second);
    if (it2 != node_idx_to_idx_of_second_node_in_edge.end())
    {
        ss << "Node " << edge.second << " appears twice as a second node (once for edge " << it2->second
           << ", and once for current edge " << current_edge_idx << " (all indexes start at 0)";
        THROW(__PRETTY_FUNCTION__, ClosingFacetComputerException, ss.str());
    }
}

template <typename T> bool contains(const TR1(unordered_map)<size_t,T>& map, const size_t idx)
{
    if (map.find(idx) != map.end()) return true;
                                    return false;
}

bool put_edges_in_facets(const TR1(unordered_map)<size_t,size_t>& node2edge,
                         TR1(unordered_map)<size_t,size_t>& edge2facet,
                         const size_t node,
                         const size_t current_edge_idx,
                         const size_t current_nb_of_facets,
                         TR1(unordered_map)<size_t,std::vector<size_t> >& facet_idx_to_facet
                         );
bool put_edges_in_facets(const TR1(unordered_map)<size_t,size_t>& node2edge,
                         TR1(unordered_map)<size_t,size_t>& edge2facet,
                         const size_t node,
                         const size_t current_edge_idx,
                         const size_t current_nb_of_facets,
                         TR1(unordered_map)<size_t,std::vector<size_t> >& facet_idx_to_facet
                         )
{
    const bool node_appears_in_two_different_edges = contains(node2edge,node);
    if (node_appears_in_two_different_edges)
    {
        // The two different edges in question belong to the same facet, as they are linked by the first node in the current edge
        const size_t idx_of_first_edge = current_edge_idx;
        const size_t idx_of_second_edge = node2edge.find(node)->second;
        const bool first_edge_already_in_a_facet = contains(edge2facet,idx_of_first_edge);
        const bool second_edge_already_in_a_facet = contains(edge2facet,idx_of_second_edge);
        if (first_edge_already_in_a_facet and not(second_edge_already_in_a_facet))
        {
            // Put second edge in a facet
            const size_t facet_idx = edge2facet[idx_of_first_edge];
            edge2facet[idx_of_second_edge] = facet_idx;
            facet_idx_to_facet[facet_idx].push_back(idx_of_second_edge);
        }
        if (second_edge_already_in_a_facet and not(first_edge_already_in_a_facet))
        {
            // Put first edge in a facet
            const size_t facet_idx = edge2facet[idx_of_second_edge];
            edge2facet[idx_of_first_edge] = facet_idx;
            facet_idx_to_facet[facet_idx].push_back(idx_of_first_edge);
        }
        if (not(first_edge_already_in_a_facet) and not(second_edge_already_in_a_facet))
        {
            // Create a new facet
            edge2facet[idx_of_second_edge] = current_nb_of_facets;
            edge2facet[idx_of_first_edge] = current_nb_of_facets;
            facet_idx_to_facet[current_nb_of_facets] = {idx_of_second_edge,idx_of_first_edge};
            return true;
        }
        // If both edges are already in a facet, do nothing
    }
    return false;
}

std::vector<std::vector<size_t> > ClosingFacetComputer::group_connected_edges_into_facets(const std::vector<size_t>& edges_index, const std::vector<std::pair<size_t,size_t> >& edges)
{
    std::vector<std::vector<size_t> > facets;
    TR1(unordered_map)<size_t,size_t> idx_of_first_node_in_edge_to_edge_idx;
    TR1(unordered_map)<size_t,size_t> idx_of_second_node_in_edge_to_edge_idx;
    TR1(unordered_map)<size_t,size_t> edge_idx_to_facet_idx;
    TR1(unordered_map)<size_t,std::vector<size_t> > facet_idx_to_facet;

    size_t current_nb_of_facets = 0;
    const size_t n = edges.size();
    for (size_t i = 0 ; i < n ; ++i)
    {
        const size_t current_edge_idx = edges_index.at(i);
        const auto edge = edges.at(i);
        check_nodes_appear_just_once_as_first_or_second_node_in_edge(current_edge_idx,edge,idx_of_first_node_in_edge_to_edge_idx,idx_of_second_node_in_edge_to_edge_idx);
        idx_of_first_node_in_edge_to_edge_idx[edge.first] = current_edge_idx;
        idx_of_second_node_in_edge_to_edge_idx[edge.second] = current_edge_idx;
        if (put_edges_in_facets(idx_of_second_node_in_edge_to_edge_idx,
                                edge_idx_to_facet_idx,
                                edge.first,
                                current_edge_idx,
                                current_nb_of_facets,
                                facet_idx_to_facet))
            current_nb_of_facets++;
        if (put_edges_in_facets(idx_of_first_node_in_edge_to_edge_idx,
                                edge_idx_to_facet_idx,
                                edge.second,
                                current_edge_idx,
                                current_nb_of_facets,
                                facet_idx_to_facet))
            current_nb_of_facets++;
    }
    for (auto facet:facet_idx_to_facet)
    {
        facets.push_back(facet.second);
    }
    return facets;
}


