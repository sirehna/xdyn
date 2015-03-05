/*
 * ClosingFacetComputerTest.cpp
 *
 *  Created on: Feb 27, 2015
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include <Eigen/Dense>

#include "gmock/gmock.h"

#include <ssc/macros.hpp>

#include "2DMeshDisplay.hpp"
#include "ClosingFacetComputerTest.hpp"
#include "ClosingFacetComputer.hpp"
#include "ClosingFacetComputerException.hpp"
#include "TestMeshes.hpp"

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

ClosingFacetComputer make(const TestMesh& mesh);
ClosingFacetComputer make(const TestMesh& mesh)
{
    return ClosingFacetComputer(mesh.all_nodes, mesh.edges);
}

using namespace testing; // So we can use 'ElementsAre' unqualified

TEST_F(ClosingFacetComputerTest, can_cluster_edges_into_independent_facets_to_compute_closing_facet)
{
    const std::vector<std::vector<size_t> > closing_facets = ClosingFacetComputer::group_connected_edges(case_14().edges);
    ASSERT_EQ(2,  closing_facets.size());
    ASSERT_THAT(closing_facets.at(0), ElementsAre(0,1,2));
    ASSERT_THAT(closing_facets.at(1), ElementsAre(3,4,5));
}

TEST_F(ClosingFacetComputerTest, can_extract_relevant_nodes)
{
    ASSERT_THAT(make(case_0()).extract_nodes(), ElementsAre(13,15,9,11));
}

TEST_F(ClosingFacetComputerTest, can_find_extreme_node)
{
    ASSERT_EQ(13, make(case_0()).find_extreme_node());
    ASSERT_EQ(0,  make(case_1()).find_extreme_node());
    ASSERT_EQ(0,  make(case_2()).find_extreme_node());
    ASSERT_EQ(0,  make(case_3()).find_extreme_node());
    ASSERT_EQ(0,  make(case_4()).find_extreme_node());
    ASSERT_EQ(0,  make(case_5()).find_extreme_node());
    ASSERT_EQ(0,  make(case_6()).find_extreme_node());
    ASSERT_EQ(0,  make(case_7()).find_extreme_node());
    ASSERT_EQ(0,  make(case_8()).find_extreme_node());
    ASSERT_EQ(0,  make(case_9()).find_extreme_node());
    ASSERT_EQ(0,  make(case_10()).find_extreme_node());
    ASSERT_EQ(7,  make(case_11()).find_extreme_node());
}

TEST_F(ClosingFacetComputerTest, can_retrieve_angle_between_two_edges)
{
    ASSERT_DOUBLE_EQ(PI/2, make(case_1()).angle_between_edges(0,1));
    ASSERT_DOUBLE_EQ(3*PI/4-PI/2, make(case_1()).angle_between_edges(1,2));
    ASSERT_DOUBLE_EQ(-PI/2, make(case_1()).angle_between_edges(2,5));
}

TEST_F(ClosingFacetComputerTest, angle_between_closing_edges_should_be_the_greatest_when_positive)
{
    ASSERT_GT(make(case_13()).angle_between_edges(0,1), make(case_13()).angle_between_edges(0,2));
    ASSERT_GT(make(case_10()).angle_between_edges(3,4), make(case_10()).angle_between_edges(3,12));
}

TEST_F(ClosingFacetComputerTest, angle_between_closing_edges_should_be_the_lowest_when_negative)
{
    ASSERT_LT(make(case_12()).angle_between_edges(0,1), make(case_12()).angle_between_edges(0,2));
}

TEST_F(ClosingFacetComputerTest, edge_angle_should_throw_if_edges_do_not_exist)
{
    ASSERT_THROW(make(case_1()).angle_between_edges(5,6), ClosingFacetComputerException);
    ASSERT_THROW(make(case_1()).angle_between_edges(6,5), ClosingFacetComputerException);
}

TEST_F(ClosingFacetComputerTest, edge_angle_should_throw_if_edges_do_not_have_a_point_in_common)
{
    ASSERT_THROW(make(case_1()).angle_between_edges(0,3), ClosingFacetComputerException);
}

TEST_F(ClosingFacetComputerTest, can_find_edges_connected_to_second_point_of_a_particular_edge)
{
    ASSERT_THAT(make(case_1()).edges_connected_to_second_node_of_edge(0), ElementsAre(1));
    ASSERT_THAT(make(case_1()).edges_connected_to_second_node_of_edge(1), ElementsAre(2,3));
    ASSERT_THAT(make(case_1()).edges_connected_to_second_node_of_edge(3), ElementsAre(4,5));
    ASSERT_THAT(make(case_1()).edges_connected_to_second_node_of_edge(5), ElementsAre(3,4));
    ASSERT_THAT(make(case_9()).edges_connected_to_second_node_of_edge(3), ElementsAre(0,1,4));
}

TEST_F(ClosingFacetComputerTest, next_edge_should_throw_if_edge_does_not_exist)
{
    ASSERT_THROW(make(case_1()).next_edge(10), ClosingFacetComputerException);
}

TEST_F(ClosingFacetComputerTest, next_edge_should_throw_if_edge_is_not_connected_to_anything)
{
    ASSERT_THROW(make(case_8()).next_edge(5), ClosingFacetComputerException);
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_1)
{
    ASSERT_EQ(1, make(case_1()).next_edge(0));
    ASSERT_EQ(3, make(case_1()).next_edge(1));
    ASSERT_EQ(4, make(case_1()).next_edge(3));
    ASSERT_EQ(0, make(case_1()).next_edge(4));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_2)
{
    ASSERT_EQ(1, make(case_2()).next_edge(0));
    ASSERT_EQ(2, make(case_2()).next_edge(1));
    ASSERT_EQ(5, make(case_2()).next_edge(2));
    ASSERT_EQ(4, make(case_2()).next_edge(5));
    ASSERT_EQ(0, make(case_2()).next_edge(4));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_3)
{
    ASSERT_EQ(1, make(case_3()).next_edge(0));
    ASSERT_EQ(2, make(case_3()).next_edge(1));
    ASSERT_EQ(3, make(case_3()).next_edge(2));
    ASSERT_EQ(4, make(case_3()).next_edge(3));
    ASSERT_EQ(5, make(case_3()).next_edge(4));
    ASSERT_EQ(6, make(case_3()).next_edge(5));
    ASSERT_EQ(7, make(case_3()).next_edge(6));
    ASSERT_EQ(0, make(case_3()).next_edge(7));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_4)
{
    ASSERT_EQ(5, make(case_4()).next_edge(0));
    ASSERT_EQ(4, make(case_4()).next_edge(5));
    ASSERT_EQ(7, make(case_4()).next_edge(4));
    ASSERT_EQ(0, make(case_4()).next_edge(7));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_5)
{
    ASSERT_EQ(1, make(case_5()).next_edge(0));
    ASSERT_EQ(2, make(case_5()).next_edge(1));
    ASSERT_EQ(3, make(case_5()).next_edge(2));
    ASSERT_EQ(4, make(case_5()).next_edge(3));
    ASSERT_EQ(6, make(case_5()).next_edge(4));
    ASSERT_EQ(0, make(case_5()).next_edge(6));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_6)
{
    ASSERT_EQ(1, make(case_6()).next_edge(0));
    ASSERT_EQ(2, make(case_6()).next_edge(1));
    ASSERT_EQ(3, make(case_6()).next_edge(2));
    ASSERT_EQ(4, make(case_6()).next_edge(3));
    ASSERT_EQ(5, make(case_6()).next_edge(4));
    ASSERT_EQ(0, make(case_6()).next_edge(5));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_7)
{
    ASSERT_EQ(1, make(case_7()).next_edge(0));
    ASSERT_EQ(2, make(case_7()).next_edge(1));
    ASSERT_EQ(3, make(case_7()).next_edge(2));
    ASSERT_EQ(4, make(case_7()).next_edge(3));
    ASSERT_EQ(5, make(case_7()).next_edge(4));
    ASSERT_EQ(0, make(case_7()).next_edge(5));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_8)
{
    ASSERT_EQ(1, make(case_8()).next_edge(0));
    ASSERT_EQ(2, make(case_8()).next_edge(1));
    ASSERT_EQ(3, make(case_8()).next_edge(2));
    ASSERT_EQ(4, make(case_8()).next_edge(3));
    ASSERT_EQ(0, make(case_8()).next_edge(4));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_9)
{
    ASSERT_EQ(1, make(case_9()).next_edge(0));
    ASSERT_EQ(2, make(case_9()).next_edge(1));
    ASSERT_EQ(3, make(case_9()).next_edge(2));
    ASSERT_EQ(4, make(case_9()).next_edge(3));
    ASSERT_EQ(5, make(case_9()).next_edge(4));
    ASSERT_EQ(0, make(case_9()).next_edge(5));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_10)
{
    ASSERT_EQ(1, make(case_10()).next_edge(0));
    ASSERT_EQ(2, make(case_10()).next_edge(1));
    ASSERT_EQ(3, make(case_10()).next_edge(2));
    ASSERT_EQ(4, make(case_10()).next_edge(3));
    ASSERT_EQ(5, make(case_10()).next_edge(4));
    ASSERT_EQ(6, make(case_10()).next_edge(5));
    ASSERT_EQ(7, make(case_10()).next_edge(6));
    ASSERT_EQ(0, make(case_10()).next_edge(7));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_11)
{
    ASSERT_EQ(8, make(case_11()).next_edge(0));
    ASSERT_EQ(2, make(case_11()).next_edge(1));
    ASSERT_EQ(3, make(case_11()).next_edge(2));
    ASSERT_EQ(4, make(case_11()).next_edge(3));
    ASSERT_EQ(5, make(case_11()).next_edge(4));
    ASSERT_EQ(6, make(case_11()).next_edge(5));
}

TEST_F(ClosingFacetComputerTest, can_find_first_extreme_edge)
{
    ASSERT_EQ(0, make(case_1()).first_extreme_edge());
    ASSERT_EQ(0, make(case_2()).first_extreme_edge());
    ASSERT_EQ(0, make(case_3()).first_extreme_edge());
    ASSERT_EQ(0, make(case_4()).first_extreme_edge());
    ASSERT_EQ(0, make(case_5()).first_extreme_edge());
    ASSERT_EQ(0, make(case_6()).first_extreme_edge());
    ASSERT_EQ(5, make(case_7()).first_extreme_edge());
    ASSERT_EQ(0, make(case_8()).first_extreme_edge());
    ASSERT_EQ(5, make(case_9()).first_extreme_edge());
    ASSERT_EQ(7, make(case_10()).first_extreme_edge());
    ASSERT_EQ(0, make(case_11()).first_extreme_edge());
}

TEST_F(ClosingFacetComputerTest, can_find_contour)
{
    ASSERT_THAT(make(case_1()).contour(), ElementsAre(0,1,3,4));
    ASSERT_THAT(make(case_2()).contour(), ElementsAre(0,1,2,5,4));
    ASSERT_THAT(make(case_3()).contour(), ElementsAre(0,1,2,3,4,5,6,7));
    ASSERT_THAT(make(case_4()).contour(), ElementsAre(0,5,4,7));
    ASSERT_THAT(make(case_5()).contour(), ElementsAre(0,1,2,3,4,6));
    ASSERT_THAT(make(case_6()).contour(), ElementsAre(0,1,2,3,4,5));
    ASSERT_THAT(make(case_7()).contour(), ElementsAre(5,0,1,2,3,4));
    ASSERT_THAT(make(case_8()).contour(), ElementsAre(0,1,2,3,4));
    ASSERT_THAT(make(case_9()).contour(), ElementsAre(5,0,1,2,3,4));
    ASSERT_THAT(make(case_10()).contour(), ElementsAre(7,0,1,2,3,4,5,6));
    ASSERT_THAT(make(case_11()).contour(), ElementsAre(0,8,4,5,6));
}

TEST_F(ClosingFacetComputerTest, bug_in_group_connected_edges_detected_in_MeshIntersector)
{
    const std::vector<std::vector<size_t> > closing_facets = ClosingFacetComputer::group_connected_edges(case_16().edges);
    ASSERT_EQ(2,  closing_facets.size());
    ASSERT_THAT(closing_facets.at(0), ElementsAre(0,1,2,3,4));
    ASSERT_THAT(closing_facets.at(1), ElementsAre(5,6,7,8,9));
}
