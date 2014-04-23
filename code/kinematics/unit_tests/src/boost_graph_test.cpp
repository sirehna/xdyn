/*
 * boost_graph_test.cpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */


#include "boost_graph_test.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

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
typedef std::vector<Graph::edge_descriptor> PathType;




boost_graph_test::boost_graph_test() : a(DataGenerator(12222))
{
}

boost_graph_test::~boost_graph_test()
{
}

void boost_graph_test::SetUp()
{
}

void boost_graph_test::TearDown()
{
}

TEST_F(boost_graph_test, reimplementation_of_robwork)
{
    Graph g;

    Vertex A = boost::add_vertex(std::string("A"), g);
    Vertex B = boost::add_vertex(std::string("B"), g);
    Vertex C = boost::add_vertex(std::string("C"), g);
    Vertex D = boost::add_vertex(std::string("D"), g);
    Vertex E = boost::add_vertex(std::string("E"), g);
    Vertex F = boost::add_vertex(std::string("F"), g);
    Vertex G = boost::add_vertex(std::string("G"), g);
    Vertex H = boost::add_vertex(std::string("H"), g);
    Vertex I = boost::add_vertex(std::string("I"), g);


    boost::add_edge (A, B, 1, g);
    boost::add_edge (A, C, 1, g);
    boost::add_edge (B, D, 1, g);
    boost::add_edge (C, E, 1, g);
    boost::add_edge (C, F, 1, g);
    boost::add_edge (D, G, 1, g);
    boost::add_edge (D, H, 1, g);
    boost::add_edge (D, I, 1, g);

    std::vector<Vertex> predecessors(boost::num_vertices(g)); // To store parents
    std::vector<Weight> distances(boost::num_vertices(g)); // To store distances

    IndexMap indexMap = boost::get(boost::vertex_index, g);
    PredecessorMap predecessorMap(&predecessors[0], indexMap);
    DistanceMap distanceMap(&distances[0], indexMap);

    // Compute shortest paths from v0 to all vertices, and store the output in predecessors and distances
    // boost::dijkstra_shortest_paths(g, v0, boost::predecessor_map(predecessorMap).distance_map(distanceMap));
    // This is exactly the same as the above line - it is the idea of "named parameters" - you can pass the
    // prdecessor map and the distance map in any order.
    boost::dijkstra_shortest_paths(g, C, boost::distance_map(distanceMap).predecessor_map(predecessorMap));

    PathType path;
    NameMap nameMap = boost::get(boost::vertex_name, g);
    Vertex v = D; // We want to start at the destination and work our way back to the source
    for(Vertex u = predecessorMap[v]; // Start by setting 'u' to the destintaion node's predecessor
      u != v; // Keep tracking the path until we get to the source
      v = u, u = predecessorMap[v]) // Set the current vertex to the current predecessor, and the predecessor to one level up
    {
    std::pair<Graph::edge_descriptor, bool> edgePair = boost::edge(u, v, g);
    Graph::edge_descriptor edge = edgePair.first;

    path.push_back( edge );
    }

    std::cout << "Shortest path from v0 to v3:" << std::endl;
    for(PathType::reverse_iterator pathIterator = path.rbegin(); pathIterator != path.rend(); ++pathIterator)
    {
    std::cout << nameMap[boost::source(*pathIterator, g)] << " -> " << nameMap[boost::target(*pathIterator, g)]
              << " = " << boost::get( boost::edge_weight, g, *pathIterator ) << std::endl;

    }
}
