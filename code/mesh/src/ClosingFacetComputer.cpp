#include <algorithm> // std::copy_if
#include <iterator> // std::back_inserter
#include <set>
#include <sstream>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

#include "ClosingFacetComputer.hpp"
#include "ClosingFacetComputerException.hpp"

#include <ssc/macros.hpp>
#include TR1INC(unordered_map)

#include <ssc/macros/SerializeMapsSetsAndVectors.hpp>

void check_edge_index(const size_t idx, const ClosingFacetComputer::ListOfEdges& edges, const std::string& function, const size_t line);
void check_edge_index(const size_t idx, const ClosingFacetComputer::ListOfEdges& edges, const std::string& function, const size_t line)
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

ClosingFacetComputer::ClosingFacetComputer(const Eigen::Matrix3Xd& mesh_, const ListOfEdges& edges_, std::vector<size_t> index_of_relevant_edges) :
        mesh(&mesh_),
        edges(),
        node_idx_in_mesh(),
        node_to_connected_edges(),
        original_edge_index()
{
    if (index_of_relevant_edges.empty()) for (size_t i = 0 ; i < edges_.size() ; ++i) index_of_relevant_edges.push_back(i);
    for (const auto idx:index_of_relevant_edges)
    {
        edges.push_back(edges_.at(idx));
        original_edge_index.push_back(idx);
    }
    node_idx_in_mesh = extract_nodes(edges);
    node_to_connected_edges = get_node_to_connected_edges(edges);
}

ClosingFacetComputer::ClosingFacetComputer(const Eigen::Matrix3Xd* mesh_, const ListOfEdges& edges_, std::vector<size_t> index_of_relevant_edges) :
        mesh(mesh_),
        edges(),
        node_idx_in_mesh(),
        node_to_connected_edges(),
        original_edge_index()
{
    if (index_of_relevant_edges.empty()) for (size_t i = 0 ; i < edges_.size() ; ++i) index_of_relevant_edges.push_back(i);
    for (const auto idx:index_of_relevant_edges)
    {
        edges.push_back(edges_.at(idx));
        original_edge_index.push_back(idx);
    }
    node_idx_in_mesh = extract_nodes(edges);
    node_to_connected_edges = get_node_to_connected_edges(edges);
}

template <typename T,typename U> bool has(const U& map_or_set, const T idx)
{
    if (map_or_set.find(idx) != map_or_set.end()) return true;
                                                  return false;
}

std::vector<std::vector<size_t> > convert_sets_to_vectors(const std::vector<std::set<size_t> >& facets);
std::vector<std::vector<size_t> > convert_sets_to_vectors(const std::vector<std::set<size_t> >& facets)
{
    std::vector<std::vector<size_t> > ret;
    for (const auto facet : facets)
    {
        ret.push_back(std::vector<size_t>(facet.begin(), facet.end()));
    }
    return ret;
}

ClosingFacetComputer::ConnectedComponents ClosingFacetComputer::get_connected_components(const ListOfEdges& edges_)
{
    using namespace boost;
    typedef adjacency_list <vecS, vecS, undirectedS> Graph;
    Graph G;
    std::map<size_t,size_t> idx_in_mesh_to_node_idx;
    std::map<size_t,size_t> node_idx_to_idx_in_mesh;
    const auto idx_in_mesh = extract_nodes(edges_);
    size_t i = 0;
    size_t max_idx = 0;
    for (auto idx:idx_in_mesh)
    {
        idx_in_mesh_to_node_idx[idx] = i;
        node_idx_to_idx_in_mesh[i++] = idx;
        max_idx = std::max(max_idx, idx);
    }
    for (const auto edge:edges_) add_edge(idx_in_mesh_to_node_idx[edge.first], idx_in_mesh_to_node_idx[edge.second], G);

    ConnectedComponents ret;
    ret.component_idx_per_node = std::vector<int>(num_vertices(G));
    ret.nb_of_components = connected_components(G, &ret.component_idx_per_node[0]);
    std::vector<int> component_idx_per_node(max_idx+1);
    for (size_t i= 0 ; i <ret.component_idx_per_node.size() ; ++i)
    {
        component_idx_per_node[node_idx_to_idx_in_mesh[i]] = ret.component_idx_per_node[i];
    }
    ret.component_idx_per_node = component_idx_per_node;
    return ret;
}

std::vector<std::set<size_t> > ClosingFacetComputer::get_edges_per_component(const ConnectedComponents& connected_components, const ListOfEdges& edges_)
{
    std::vector<std::set<size_t> > facets(connected_components.nb_of_components);
    auto node2edges = get_node_to_connected_edges(edges_);
    for (size_t node_idx = 0 ; node_idx < connected_components.component_idx_per_node.size() ; ++node_idx)
    {
        const auto edges_connected_to_current_node = node2edges[node_idx];
        const auto facet_idx = connected_components.component_idx_per_node[node_idx];
        facets[facet_idx].insert(edges_connected_to_current_node.begin(),edges_connected_to_current_node.end());
    }
    return facets;
}

std::vector<std::vector<size_t> > convert_to_original_indexes(const std::vector<std::vector<size_t> >& idx_to_convert, const std::vector<size_t>& idx_of_relevant_edges);
std::vector<std::vector<size_t> > convert_to_original_indexes(const std::vector<std::vector<size_t> >& idx_to_convert, const std::vector<size_t>& idx_of_relevant_edges)
{
    std::vector<std::vector<size_t> > ret;
    for (auto v:idx_to_convert)
    {
        std::vector<size_t> l;
        for (auto idx:v)
        {
            l.push_back(idx_of_relevant_edges[idx]);
        }
        ret.push_back(l);
    }
    return ret;
}

std::vector<std::vector<size_t> > ClosingFacetComputer::group_connected_edges(const ListOfEdges& edges_, std::vector<size_t> idx_of_relevant_edges)
{
    ListOfEdges relevant_edges;
    if (idx_of_relevant_edges.empty()) for (size_t i = 0 ; i < edges_.size() ; ++i) idx_of_relevant_edges.push_back(i);
    for (auto idx:idx_of_relevant_edges) relevant_edges.push_back(edges_.at(idx));
    const auto c = get_connected_components(relevant_edges);
    const auto e = get_edges_per_component(c, relevant_edges);
    const auto v = convert_sets_to_vectors(e);
    const auto o = convert_to_original_indexes(v, idx_of_relevant_edges);
    return o;
}

std::vector<size_t> ClosingFacetComputer::extract_nodes(const ListOfEdges& edges_)
{
    std::set<size_t> used_nodes;
    std::vector<size_t> ret;
    for (const auto edge:edges_)
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

std::pair<size_t,size_t> ClosingFacetComputer::find_extreme_nodes() const
{
    size_t idx_xmin = 0;
    double xmin = mesh->operator()(0,0);
    size_t idx_ymax = 0;
    double ymax = mesh->operator()(0,0);
    for (size_t i = 1 ; i < node_idx_in_mesh.size() ; ++i)
    {
        const double xval = mesh->operator()(0,node_idx_in_mesh.at(i));
        const double yval = mesh->operator()(1,node_idx_in_mesh.at(i));
        if (xval<xmin)
        {
            idx_xmin = i;
            xmin = xval;
        }
        if (yval>ymax)
        {
            idx_ymax = i;
            ymax = yval;
        }
    }
    return std::make_pair(node_idx_in_mesh.at(idx_xmin),node_idx_in_mesh.at(idx_ymax));
}

struct TwoEdges
{
    TwoEdges(const size_t idx_of_edge_AB, const size_t idx_of_edge_BC, const ClosingFacetComputer::ListOfEdges& edges, const bool reverse) :
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
        const size_t common_node = get_common_node(reverse);
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

        size_t get_common_node(const bool reverse) const
        {
            const std::vector<size_t> nodes = common_nodes();
            std::stringstream err;
            if (nodes.empty())              err << "Edges have no common' middle nodes should be the same. ";
            else
            {
                if (nodes.size() > 1)       err << "Edges have more than one common node. ";
                if ((not(reverse)) and (idx_B != nodes.front())) err << "The common node should be the second node of the first edge. ";
                if (reverse and (idx_A != nodes.front())) err << "The common node should be the first node of the first edge because it is reversed. ";
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

double constrainAngle(double x);
double constrainAngle(double x)
{
    x = fmod(x,2*PI);
    if (x < 0)
        x += 2*PI;
    return x;// - PI;
}

double ClosingFacetComputer::angle_between_edges(const size_t idx_of_edge_AB, const size_t idx_of_edge_BC, const bool reverse) const
{
    const TwoEdges AB_BC(idx_of_edge_AB, idx_of_edge_BC, edges, reverse);
    const auto BA = mesh->col(AB_BC.get_idx_A()) - mesh->col(AB_BC.get_idx_B());
    const auto BC = mesh->col(AB_BC.get_idx_C()) - mesh->col(AB_BC.get_idx_B());
    const double BA_angle = (std::atan2(BA(1),BA(0)));
    const double BC_angle = (std::atan2(BC(1),BC(0)));
    const double angle = (BC_angle - BA_angle);
    const double sign = (-BA).cross(BC)(2) <0 ? -1 : 1;
    return sign*constrainAngle(angle);
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

std::vector<size_t> ClosingFacetComputer::edges_connected_to_first_node_of_edge(const size_t edge_idx) const
{
    check_edge_index(edge_idx, edges, __PRETTY_FUNCTION__, __LINE__);
    const size_t first_node = edges.at(edge_idx).first;
    const auto it = node_to_connected_edges.find(first_node);
    if (it == node_to_connected_edges.end())
    {
        std::stringstream ss;
        ss << "Unable to find edges connected to first node of edge #" << edge_idx << " (starting at 0)";
        THROW(__PRETTY_FUNCTION__, ClosingFacetComputerException, ss.str());
    }
    const auto edges = it->second;
    std::vector<size_t> ret;
    std::copy_if(edges.begin(), edges.end(), std::back_inserter(ret),[edge_idx](const size_t i) { return i != edge_idx; });
    return ret;
}

size_t ClosingFacetComputer::next_edge(const size_t edge_idx, const bool reverse) const
{
    check_edge_index(edge_idx, edges, __PRETTY_FUNCTION__, __LINE__);
    const std::vector<size_t> connected_edges = reverse ? edges_connected_to_first_node_of_edge(edge_idx) : edges_connected_to_second_node_of_edge(edge_idx);
    if (connected_edges.empty())
    {
        std::stringstream ss;
        ss << "Second node of edge #" << edge_idx;;
        if (edge_idx>0) ss << " (starting from 0)";
        ss << " is not connected to anything: cannot find following edge.";
        THROW(__PRETTY_FUNCTION__, ClosingFacetComputerException, ss.str());
    }
    size_t idx = 0;
    double angle = angle_between_edges(edge_idx, connected_edges.front(),reverse);
    for (size_t i = 1 ; i < connected_edges.size() ; ++i)
    {
        const size_t idx_of_connected_edge = connected_edges.at(i);

        const double new_angle = angle_between_edges(edge_idx, idx_of_connected_edge,reverse);
        if ((angle < 0) and (new_angle < 0))
        {
            if (new_angle < angle)
            {
                idx = i;
                angle = new_angle;
            }
        }
        else if ((angle < 0) and (new_angle > 0))
        {
            if (new_angle < angle)
            {
                idx = i;
                angle = new_angle;
            }
        }
        else
        {
            if (new_angle < angle)
            {
                idx = i;
                angle = new_angle;
            }
        }
    }
    return connected_edges.at(idx);
}

size_t ClosingFacetComputer::extreme_edge(const size_t extreme_node) const
{
    const auto it = node_to_connected_edges.find(extreme_node);
    if (it == node_to_connected_edges.end())
    {
        std::stringstream ss;
        ss << "Unable to find node " << extreme_node << " in map.";
        THROW(__PRETTY_FUNCTION__, ClosingFacetComputerException, ss.str());
    }
    const auto candidates = it->second;
    const auto A = mesh->col(extreme_node);
    const auto compute_angle = [this, A, extreme_node](const size_t candidate) -> double
                               {
                                   const auto idxB = edges.at(candidate).first==extreme_node ? edges.at(candidate).second : edges.at(candidate).first;
                                   const auto B = mesh->col(idxB);
                                   const auto AB = B-A;
                                   return wrap_2pi(std::atan2(AB(1),AB(0)));
                               };
    size_t ret = 0;
    double angle = compute_angle(ret);
    for (const auto candidate:candidates)
    {
        const double new_angle = compute_angle(candidate);
        if (new_angle > angle)
        {
            ret = candidate;
            angle = new_angle;
        }
    }
    return ret;
}

std::pair<size_t,size_t> ClosingFacetComputer::extreme_edges() const
{
    const auto extreme_node = find_extreme_nodes();
    return std::make_pair(extreme_edge(extreme_node.first), extreme_edge(extreme_node.second));
}

bool ClosingFacetComputer::need_to_reverse(const size_t first_edge, const size_t second_edge, const bool reverse) const
{
    return reverse ? edges.at(first_edge).first != edges.at(second_edge).first : edges.at(first_edge).second != edges.at(second_edge).first;
}

ClosingFacetComputer::Contour ClosingFacetComputer::contour(size_t edge) const
{
    Contour ret;
    ret.edge_idx.push_back(edge);
    ret.reversed.push_back(false);
    size_t previous_edge = edge;
    try
    {
        edge = next_edge(edge);
    }
    catch (const ClosingFacetComputerException&)
    {
        return Contour();
    }
    bool reverse = false;
    while ((edge != ret.edge_idx.front()) and (ret.edge_idx.size() < edges.size()))
    {
        try
        {
            ret.edge_idx.push_back(edge);
            reverse = need_to_reverse(previous_edge,edge,reverse);
            ret.reversed.push_back(reverse);
            previous_edge = edge;
            edge = next_edge(edge,reverse);
        }
        catch (const ClosingFacetComputerException&)
        {
            return Contour();
        }
    }
    return ret;
}

template <typename T> bool have_same_elements(const std::vector<T>& v1, const std::vector<T>& v2)
{
    const std::set<T> s1(v1.begin(), v1.end());
    const std::set<T> s2(v2.begin(), v2.end());
    for (const auto val:s1)
    {
        if (s2.find(val) == s2.end()) return false;
    }
    for (const auto val:s2)
    {
        if (s1.find(val) == s1.end()) return false;
    }
    return true;
}

ClosingFacetComputer::Contour ClosingFacetComputer::contour() const
{
    auto ext_edges = extreme_edges();
    const auto first_contour = contour(ext_edges.first);
    const auto second_contour = contour(ext_edges.second);
    if (not(have_same_elements(first_contour.edge_idx, second_contour.edge_idx)))
    {
        THROW(__PRETTY_FUNCTION__, ClosingFacetComputerException, "Not getting the same contour when starting from two different extreme edges");
    }
    std::vector<size_t> contour_with_original_indexes;
    for (const auto idx:first_contour.edge_idx) contour_with_original_indexes.push_back(original_edge_index[idx]);
    Contour ret;
    ret.edge_idx = contour_with_original_indexes;
    ret.reversed = first_contour.reversed;
    return ret;
}

std::map<size_t,std::set<size_t> > ClosingFacetComputer::get_node_to_connected_edges(const ListOfEdges& edges_)
{
    std::map<size_t,std::set<size_t> > ret;
    for (size_t i = 0 ; i < edges_.size() ; ++i)
    {
        auto it = ret.find(edges_.at(i).first);
        if (it == ret.end()) ret[edges_.at(i).first] = std::set<size_t>({i});
        else                 it->second.insert(i);
        it = ret.find(edges_.at(i).second);
        if (it == ret.end()) ret[edges_.at(i).second] = std::set<size_t>({i});
        else                 it->second.insert(i);
    }
    return ret;
}
