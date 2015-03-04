#include <algorithm> // std::copy_if
#include <iterator> // std::back_inserter
#include <set>
#include <sstream>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "ClosingFacetComputer.hpp"
#include "ClosingFacetComputerException.hpp"

#include <ssc/macros.hpp>
#include TR1INC(unordered_map)


void check_edge_index(const size_t idx, const ListOfEdges& edges, const std::string& function, const size_t line);
void check_edge_index(const size_t idx, const ListOfEdges& edges, const std::string& function, const size_t line)
{
    if (idx>=edges.size())
    {
        std::stringstream ss;
        ss << "In file " << __FILE__ << ", line " << line << ", function "
           << function
           << ": "
           << "Unable to find edge #" << idx;
        if (idx>0) ss << " (starting from 0)";
        ss << ": mesh only contains " << edges.size() << " edge";
        if (edges.size() > 1) ss << "s";
        ss << ".";
        throw ClosingFacetComputerException(ss.str().c_str());
    }
}

ClosingFacetComputer::ClosingFacetComputer(const Eigen::Matrix3Xd& mesh_, const ListOfEdges& edges_) :
        mesh(mesh_),
        edges(edges_),
        node_idx_in_mesh(extract_nodes()),
        node_to_connected_edges()
{
    for (size_t i = 0 ; i < edges.size() ; ++i)
    {
        auto it = node_to_connected_edges.find(edges.at(i).first);
        if (it == node_to_connected_edges.end()) node_to_connected_edges[edges.at(i).first] = std::set<size_t>({i});
        else                                     it->second.insert(i);
        it = node_to_connected_edges.find(edges.at(i).second);
        if (it == node_to_connected_edges.end()) node_to_connected_edges[edges.at(i).second] = std::set<size_t>({i});
        else                                     it->second.insert(i);
    }
}

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

template <typename T,typename U> bool has(const U& map_or_set, const T idx)
{
    if (map_or_set.find(idx) != map_or_set.end()) return true;
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
    const bool node_appears_in_two_different_edges = has(node2edge,node);
    if (node_appears_in_two_different_edges)
    {
        // The two different edges in question belong to the same facet, as they are linked by the first node in the current edge
        const size_t idx_of_first_edge = current_edge_idx;
        const size_t idx_of_second_edge = node2edge.find(node)->second;
        const bool first_edge_already_in_a_facet = has(edge2facet,idx_of_first_edge);
        const bool second_edge_already_in_a_facet = has(edge2facet,idx_of_second_edge);
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

std::vector<size_t> ClosingFacetComputer::extract_nodes() const
{
    std::set<size_t> used_nodes;
    std::vector<size_t> ret;
    for (const auto edge:edges)
    {
        if (not(has(used_nodes, edge.first)))
        {
            used_nodes.insert(edge.first);
            ret.push_back(edge.first);
        }
        if (not(has(used_nodes, edge.second)))
        {
            used_nodes.insert(edge.second);
            ret.push_back(edge.second);
        }
    }
    return ret;
}

size_t ClosingFacetComputer::find_extreme_node() const
{
    size_t idx_xmin = 0;
    double xmin = mesh(0,0);
    for (size_t i = 1 ; i < node_idx_in_mesh.size() ; ++i)
    {
        const double val = mesh(0,node_idx_in_mesh.at(i));
        if (val<xmin)
        {
            idx_xmin = i;
            xmin = val;
        }
    }
    return node_idx_in_mesh.at(idx_xmin);
}

std::vector<size_t> ClosingFacetComputer::sort_edges(const std::vector<size_t>& indexes_of_edges_to_sort) const
{
    std::vector<size_t> ret;
    if (indexes_of_edges_to_sort.empty()) return ret;
    TR1(unordered_map)<size_t,size_t> idx_of_first_node_in_edge_to_edge_idx;
    for (const auto idx:indexes_of_edges_to_sort)
    {
        check_edge_index(idx, edges, __PRETTY_FUNCTION__, __LINE__);
        idx_of_first_node_in_edge_to_edge_idx[edges.at(idx).first] = idx;
    }

    const auto first_edge = edges.at(indexes_of_edges_to_sort.front());
    auto iterator_to_next_edge = idx_of_first_node_in_edge_to_edge_idx.find(first_edge.first);

    while ((iterator_to_next_edge != idx_of_first_node_in_edge_to_edge_idx.end()) and ret.size() < indexes_of_edges_to_sort.size())
    {
        ret.push_back(iterator_to_next_edge->second);
        const size_t idx_of_first_node_in_next_edge = edges.at(iterator_to_next_edge->second).second;
        iterator_to_next_edge = idx_of_first_node_in_edge_to_edge_idx.find(idx_of_first_node_in_next_edge);
    }
    if (ret.size() != indexes_of_edges_to_sort.size())
    {
        THROW(__PRETTY_FUNCTION__, ClosingFacetComputerException, "Unable to sort edges");
    }
    return ret;
}

struct TwoEdges
{
    TwoEdges(const size_t idx_of_edge_AB, const size_t idx_of_edge_BC, const ListOfEdges& edges) :
        idx_A(),
        idx_B(),
        idx_C(),
        idx_D()
    {
        check_edge_index(idx_of_edge_AB, edges, __PRETTY_FUNCTION__, __LINE__);
        check_edge_index(idx_of_edge_BC, edges, __PRETTY_FUNCTION__, __LINE__);
        idx_A = edges.at(idx_of_edge_AB).first;
        idx_B = edges.at(idx_of_edge_AB).second;
        idx_C = edges.at(idx_of_edge_BC).first;
        idx_D = edges.at(idx_of_edge_BC).second;
        const size_t common_node = get_common_node();
        if (idx_C == common_node) idx_C = idx_D;
    }

    size_t get_idx_A() const
    {
        return idx_A;
    }

    size_t get_idx_B() const
    {
        return idx_B;
    }

    size_t get_idx_C() const
    {
        return idx_C;
    }

    private:
        TwoEdges();

        std::vector<size_t> common_nodes() const
        {
            std::vector<size_t> ret;
            if (idx_A == idx_C) ret.push_back(idx_A);
            if (idx_A == idx_D) ret.push_back(idx_A);
            if (idx_B == idx_C) ret.push_back(idx_B);
            if (idx_B == idx_D) ret.push_back(idx_B);
            return ret;
        }

        size_t get_common_node() const
        {
            const std::vector<size_t> nodes = common_nodes();
            std::stringstream err;
            if (nodes.empty())              err << "Edges have no common' middle nodes should be the same. ";
            else
            {
                if (nodes.size() > 1)       err << "Edges have more than one common node. ";
                if (idx_B != nodes.front()) err << "The common node should be the second node of the first edge. ";

            }
            if (not(err.str().empty()))
            {
                err << "First edge has nodes " << idx_A
                    << " & " << idx_B << ", and second edge has nodes "<< idx_C
                    << " & " << idx_D << ".";
                THROW(__PRETTY_FUNCTION__, ClosingFacetComputerException, err.str());
            }
            return nodes.front();
        }

        size_t idx_A;
        size_t idx_B;
        size_t idx_C;
        size_t idx_D;
};

double wrap_2pi(const double theta); // We know the result of atan2 is between -pi & pi
double wrap_2pi(const double theta)
{
    return theta > 0 ? theta : theta+2*PI;
}

double ClosingFacetComputer::edge_angle(const size_t idx_of_edge_AB, const size_t idx_of_edge_BC) const
{
    const TwoEdges AB_BC(idx_of_edge_AB, idx_of_edge_BC, edges);
    const auto BA = mesh.col(AB_BC.get_idx_A()) - mesh.col(AB_BC.get_idx_B());
    const auto BC = mesh.col(AB_BC.get_idx_C()) - mesh.col(AB_BC.get_idx_B());
    const double BA_angle = wrap_2pi(std::atan2(BA(1),BA(0)));
    const double BC_angle = wrap_2pi(std::atan2(BC(1),BC(0)));
    return BA_angle - BC_angle;
}

std::vector<size_t> ClosingFacetComputer::edges_connected_to_second_node_of_edge(const size_t edge_idx) const
{
    check_edge_index(edge_idx, edges, __PRETTY_FUNCTION__, __LINE__);
    const size_t second_node = edges.at(edge_idx).second;
    const auto it = node_to_connected_edges.find(second_node);
    if (it == node_to_connected_edges.end())
    {
        std::stringstream ss;
        ss << "Unable to find edges connected to second node of edge #" << edge_idx << " (starting at 0)";
        THROW(__PRETTY_FUNCTION__, ClosingFacetComputerException, ss.str());
    }
    const auto edges = it->second;
    std::vector<size_t> ret;
    std::copy_if(edges.begin(), edges.end(), std::back_inserter(ret),[edge_idx](const size_t i) { return i != edge_idx; });
    return ret;
}

size_t ClosingFacetComputer::next_edge(const size_t edge_idx) const
{
    check_edge_index(edge_idx, edges, __PRETTY_FUNCTION__, __LINE__);
    const std::vector<size_t> connected_edges = edges_connected_to_second_node_of_edge(edge_idx);
    if (connected_edges.empty())
    {
        std::stringstream ss;
        ss << "Second node of edge #" << edge_idx;;
        if (edge_idx>0) ss << " (starting from 0)";
        ss << " is not connected to anything: cannot find following edge.";
        THROW(__PRETTY_FUNCTION__, ClosingFacetComputerException, ss.str());
    }
    size_t idx = 0;
    double angle = edge_angle(edge_idx, connected_edges.front());
    for (size_t i = 1 ; i < connected_edges.size() ; ++i)
    {
        const double new_angle = edge_angle(edge_idx, connected_edges.at(i));
        if ((angle < 0) and (new_angle > 0))
        {
            if (new_angle < angle)
            {
                idx = i;
                angle = new_angle;
            }
        }
        else
        {
            if (new_angle > angle)
            {
                idx = i;
                angle = new_angle;
            }
        }
    }
    return connected_edges.at(idx);
}
