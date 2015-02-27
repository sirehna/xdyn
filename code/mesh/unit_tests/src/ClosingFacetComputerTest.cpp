/*
 * ClosingFacetComputerTest.cpp
 *
 *  Created on: Feb 27, 2015
 *      Author: cady
 */


#include "ClosingFacetComputerTest.hpp"
#include "ClosingFacetComputer.hpp"

ClosingFacetComputerTest::ClosingFacetComputerTest() : a(ssc::random_data_generator::DataGenerator(21233))
{
}

ClosingFacetComputerTest::~ClosingFacetComputerTest()
{
}

void ClosingFacetComputerTest::SetUp()
{
}

void ClosingFacetComputerTest::TearDown()
{
}


TEST_F(ClosingFacetComputerTest, can_cluster_edges_into_independent_facets_to_compute_closing_facet)
{
    const std::vector<size_t> edges_index = {1,2,3,5,8,17};
    const std::vector<std::pair<size_t,size_t> > edges_on_free_surface = {{1,2},{2,3},{3,1},{8,10},{10,12},{12,8}};
    const std::vector<std::vector<size_t> > closing_facets=ClosingFacetComputer::group_connected_edges_into_facets(edges_index, edges_on_free_surface);
    ASSERT_EQ(2,  closing_facets.size());
    ASSERT_EQ(3,  closing_facets.at(0).size());
    ASSERT_EQ(3,  closing_facets.at(1).size());
    ASSERT_EQ(1,  closing_facets.at(0).at(0));
    ASSERT_EQ(2,  closing_facets.at(0).at(1));
    ASSERT_EQ(3,  closing_facets.at(0).at(2));
    ASSERT_EQ(5,  closing_facets.at(1).at(0));
    ASSERT_EQ(8,  closing_facets.at(1).at(1));
    ASSERT_EQ(17, closing_facets.at(1).at(2));
}

