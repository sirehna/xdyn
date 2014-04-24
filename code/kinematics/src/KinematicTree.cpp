/*
 * KinematicsTree.cpp
 *
 *  Created on: 23 avr. 2014
 *      Author: cady
 */

#include "KinematicTree.hpp"
#include "KinematicsException.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <map>
#include <algorithm> // std::reverse

typedef float Weight;
typedef boost::property<boost::edge_weight_t, Weight> WeightProperty;
typedef boost::property<boost::vertex_name_t, std::string> NameProperty;

typedef boost::adjacency_list < boost::listS, boost::vecS, boost::undirectedS,
NameProperty, WeightProperty > Graph;

typedef boost::graph_traits < Graph >::vertex_descriptor Vertex;

typedef boost::property_map < Graph, boost::vertex_index_t >::type IndexMap;
typedef boost::property_map < Graph, boost::vertex_name_t >::type NameMap;

typedef boost::iterator_property_map < Vertex*, IndexMap, Vertex, Vertex& > PredecessorMap;
typedef boost::iterator_property_map < Weight*, IndexMap, Weight, Weight& > DistanceMap;
typedef std::map<std::string,Vertex>::const_iterator VertexMapIter;

class KinematicTree::Impl
{
    public:
        Impl() : g(Graph()), name_to_vertex(std::map<std::string,Vertex>()) {}

        ~Impl(){}
        Impl& operator=(const Impl& rhs)
        {
            if (this!=&rhs)
            {
                g = rhs.g;
            }
            return *this;
        }

        Impl(const Impl& rhs) : g(rhs.g), name_to_vertex(rhs.name_to_vertex)
        {
        }

        void add(const std::string& frame_A, const std::string& frame_B)
        {
            const Vertex A = get_vertex(frame_A);
            const Vertex B = get_vertex(frame_B);
            boost::add_edge(A, B, 1, g);
        }

        PathType get_path(const std::string& frame_A, const std::string& frame_B)
        {
            const std::pair<Vertex,Vertex> edge = get_edge(frame_A, frame_B);
            std::vector<std::string> path = dijkstra(edge);
            if (path.size() < 2) return PathType(1, std::make_pair(frame_A,frame_B));
            PathType ret;
            for (size_t i = 0 ; i < path.size()-1 ; ++i)
            {
                ret.push_back(std::make_pair(path[i], path[i+1]));
            }
            ret.push_back(std::make_pair(path.back(), frame_B));
            return ret;
        }

    private:

        Vertex get_vertex(const std::string& vertex_name)
        {
            const VertexMapIter that_vertex = name_to_vertex.find(vertex_name);
            if (that_vertex == name_to_vertex.end())  return new_vertex(vertex_name);
                                                      return that_vertex->second;
        }

        std::pair<Vertex,Vertex> get_edge(const std::string& vertex1, const std::string& vertex2) const
        {
            const VertexMapIter itA = name_to_vertex.find(vertex1);
            if (itA == name_to_vertex.end()) {THROW(__PRETTY_FUNCTION__, KinematicsException, std::string("Unable to find '") + vertex1 + "' in map.");}
            const VertexMapIter itB = name_to_vertex.find(vertex2);
            if (itB == name_to_vertex.end()) {THROW(__PRETTY_FUNCTION__, KinematicsException, std::string("Unable to find '") + vertex2 + "' in map.");}
            return std::make_pair(itA->second, itB->second);
        }

        Vertex new_vertex(const std::string& vertex_name)
        {
            const Vertex V = boost::add_vertex(vertex_name, g);
            name_to_vertex.insert(std::make_pair(vertex_name, V));
            return V;
        }

        std::vector<std::string> get_path(const Vertex& destination, const PredecessorMap& predecessor_map)
        {
            std::vector<std::string> path;
            NameMap nameMap = boost::get(boost::vertex_name, g);
            Vertex v = destination; // We want to start at the destination and work our way back to the source
            for(Vertex u = predecessor_map[v]; // Start by setting 'u' to the destination node's predecessor
                u != v; // Keep tracking the path until we get to the source
                v = u, u = predecessor_map[v]) // Set the current vertex to the current predecessor, and the predecessor to one level up
            {
                std::pair<Graph::edge_descriptor, bool> edgePair = boost::edge(u, v, g);
                Graph::edge_descriptor edge = edgePair.first;
                path.push_back(nameMap[boost::source(edge, g)]);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        PredecessorMap get_predecessor_map(std::vector<Vertex>& predecessors, const Vertex& start)
        {
            std::vector<Weight> distances(boost::num_vertices(g)); // To store distances
            IndexMap indexMap = boost::get(boost::vertex_index, g);
            PredecessorMap predecessor_map(&predecessors[0], indexMap);
            DistanceMap distanceMap(&distances[0], indexMap);

            // Compute shortest paths from 'start' to all vertices, and store the output in predecessors and distances
            boost::dijkstra_shortest_paths(g, start, boost::distance_map(distanceMap).predecessor_map(predecessor_map));
            return predecessor_map;
        }

        std::vector<std::string> dijkstra(const std::pair<Vertex,Vertex>& edge)
        {
            std::vector<Vertex> predecessors(boost::num_vertices(g)); // To store parents
            PredecessorMap predecessor_map = get_predecessor_map(predecessors, edge.first);
            return get_path(edge.second, predecessor_map);
        }

        Graph g;
        std::map<std::string,Vertex> name_to_vertex;
};


KinematicTree::KinematicTree() : pimpl(new Impl())
{
}

void KinematicTree::add(const std::string& frame_A, const std::string& frame_B)
{
    pimpl->add(frame_A, frame_B);
}

PathType KinematicTree::get_path(const std::string& frame_A, const std::string& frame_B)
{
    return pimpl->get_path(frame_A, frame_B);
}
