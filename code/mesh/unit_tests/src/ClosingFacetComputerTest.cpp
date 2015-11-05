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
#include "InternalErrorException.hpp"
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

ClosingFacetComputer make(const TestMesh& mesh, const std::vector<size_t>& idx_of_relevant_edges=std::vector<size_t>());
ClosingFacetComputer make(const TestMesh& mesh, const std::vector<size_t>& idx_of_relevant_edges)
{
    return ClosingFacetComputer(mesh.all_nodes, mesh.edges, idx_of_relevant_edges);
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
    ASSERT_THAT(ClosingFacetComputer::extract_nodes(case_0().edges), ElementsAre(13,15,9,11));
}

TEST_F(ClosingFacetComputerTest, can_find_extreme_node_using_xmin_strategy)
{
    ASSERT_EQ(13, make(case_0()).find_extreme_nodes().first);
    ASSERT_EQ(0,  make(case_1()).find_extreme_nodes().first);
    ASSERT_EQ(0,  make(case_2()).find_extreme_nodes().first);
    ASSERT_EQ(0,  make(case_3()).find_extreme_nodes().first);
    ASSERT_EQ(0,  make(case_4()).find_extreme_nodes().first);
    ASSERT_EQ(0,  make(case_5()).find_extreme_nodes().first);
    ASSERT_EQ(0,  make(case_6()).find_extreme_nodes().first);
    ASSERT_EQ(0,  make(case_7()).find_extreme_nodes().first);
    ASSERT_EQ(0,  make(case_8()).find_extreme_nodes().first);
    ASSERT_EQ(0,  make(case_9()).find_extreme_nodes().first);
    ASSERT_EQ(0,  make(case_10()).find_extreme_nodes().first);
    ASSERT_EQ(7,  make(case_11()).find_extreme_nodes().first);
}

TEST_F(ClosingFacetComputerTest, can_retrieve_angle_between_two_edges)
{
    ASSERT_DOUBLE_EQ(3*PI/2, make(case_1()).angle_between_edges(0,1));
    ASSERT_DOUBLE_EQ(7*PI/4, make(case_1()).angle_between_edges(1,2));
    ASSERT_DOUBLE_EQ(-PI/2, make(case_1()).angle_between_edges(2,5));
    ASSERT_DOUBLE_EQ(-PI/2, make(case_9()).angle_between_edges(0,1));
    ASSERT_DOUBLE_EQ(PI, make(case_9()).angle_between_edges(0,3));
    ASSERT_DOUBLE_EQ(3*PI/2, make(case_9()).angle_between_edges(0,4));
    ASSERT_DOUBLE_EQ(-PI/4, make(case_15()).angle_between_edges(1,4));
    ASSERT_DOUBLE_EQ(-PI/2, make(case_15()).angle_between_edges(1,0));
    ASSERT_DOUBLE_EQ(7*PI/4, make(case_15()).angle_between_edges(0,4));
    ASSERT_DOUBLE_EQ(3*PI/2, make(case_15()).angle_between_edges(0,1));
    ASSERT_DOUBLE_EQ(3*PI/2, make(case_15()).angle_between_edges(3,0));
    ASSERT_DOUBLE_EQ(7*PI/4, make(case_15()).angle_between_edges(2,4));
    ASSERT_DOUBLE_EQ(3*PI/2, make(case_15()).angle_between_edges(2,3));
}

TEST_F(ClosingFacetComputerTest, angle_between_closing_edges_should_be_the_lowest_when_negative)
{
    ASSERT_LT(make(case_12()).angle_between_edges(0,1), make(case_12()).angle_between_edges(0,2));
}

TEST_F(ClosingFacetComputerTest, edge_angle_should_throw_if_edges_do_not_exist)
{
    ASSERT_THROW(make(case_1()).angle_between_edges(5,6), InternalErrorException);
    ASSERT_THROW(make(case_1()).angle_between_edges(6,5), InternalErrorException);
}

TEST_F(ClosingFacetComputerTest, edge_angle_should_throw_if_edges_do_not_have_a_point_in_common)
{
    ASSERT_THROW(make(case_1()).angle_between_edges(0,3), InternalErrorException);
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
    ASSERT_THROW(make(case_1()).next_edge(10), InternalErrorException);
}

TEST_F(ClosingFacetComputerTest, next_edge_should_throw_if_edge_is_not_connected_to_anything)
{
    ASSERT_THROW(make(case_8()).next_edge(5), InternalErrorException);
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

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_15)
{
    ASSERT_EQ(1, make(case_15()).next_edge(0));
    ASSERT_EQ(1, make(case_9()).next_edge(0));
    ASSERT_EQ(0, make(case_15()).next_edge(1));
    ASSERT_EQ(2, make(case_15()).next_edge(1,true));
    ASSERT_EQ(3, make(case_15()).next_edge(2));
    ASSERT_EQ(0, make(case_15()).next_edge(3));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_18)
{
    ASSERT_EQ(1, make(case_18()).next_edge(0));
    ASSERT_EQ(0, make(case_18()).next_edge(1));
    ASSERT_EQ(1, make(case_18()).next_edge(2));
    ASSERT_EQ(2, make(case_18()).next_edge(3));
    ASSERT_EQ(3, make(case_18()).next_edge(0,true));
    ASSERT_EQ(2, make(case_18()).next_edge(1,true));
    ASSERT_EQ(3, make(case_18()).next_edge(2,true));
    ASSERT_EQ(0, make(case_18()).next_edge(3,true));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_19)
{
    ASSERT_EQ(0, make(case_19()).next_edge(2,true));
}


TEST_F(ClosingFacetComputerTest, can_find_first_extreme_edge_using_xmin_strategy)
{
    ASSERT_EQ(4, make(case_1()).extreme_edges().first);
    ASSERT_EQ(4, make(case_2()).extreme_edges().first);
    ASSERT_EQ(7, make(case_3()).extreme_edges().first);
    ASSERT_EQ(7, make(case_4()).extreme_edges().first);
    ASSERT_EQ(6, make(case_5()).extreme_edges().first);
    ASSERT_EQ(5, make(case_6()).extreme_edges().first);
    ASSERT_EQ(5, make(case_7()).extreme_edges().first);
    ASSERT_EQ(4, make(case_8()).extreme_edges().first);
    ASSERT_EQ(5, make(case_9()).extreme_edges().first);
    ASSERT_EQ(7, make(case_10()).extreme_edges().first);
    ASSERT_EQ(0, make(case_11()).extreme_edges().first);
}

TEST_F(ClosingFacetComputerTest, can_find_contour)
{
    ASSERT_THAT(make(case_1()).contour().edge_idx, ElementsAre(3,1,0,4));
    ASSERT_THAT(make(case_2()).contour().edge_idx, ElementsAre(5,2,1,0,4));
    ASSERT_THAT(make(case_3()).contour().edge_idx, ElementsAre(6,5,4,3,2,1,0,7));
    ASSERT_THAT(make(case_4()).contour().edge_idx, ElementsAre(4,5,0,7));
    ASSERT_THAT(make(case_5()).contour().edge_idx, ElementsAre(4,3,2,1,0,6));
    ASSERT_THAT(make(case_6()).contour().edge_idx, ElementsAre(4,3,2,1,0,5));
    ASSERT_THAT(make(case_7()).contour().edge_idx, ElementsAre(4,3,2,1,0,5));
    ASSERT_THAT(make(case_8()).contour().edge_idx, ElementsAre(3,2,1,0,4));
    ASSERT_THAT(make(case_9()).contour().edge_idx, ElementsAre(4,3,2,1,0,5));
    ASSERT_THAT(make(case_10()).contour().edge_idx, ElementsAre(6,5,4,3,2,1,0,7));
}

TEST_F(ClosingFacetComputerTest, bug_in_group_connected_edges_detected_in_MeshIntersector)
{
    const std::vector<std::vector<size_t> > closing_facets = ClosingFacetComputer::group_connected_edges(case_16().edges);
    ASSERT_EQ(2,  closing_facets.size());
    ASSERT_THAT(closing_facets.at(0), ElementsAre(0,1,2,3,4));
    ASSERT_THAT(closing_facets.at(1), ElementsAre(5,6,7,8,9));
}

TEST_F(ClosingFacetComputerTest, another_bug_in_group_connected_edges_detected_in_MeshIntersector)
{
    const std::vector<std::vector<size_t> > closing_facets = ClosingFacetComputer::group_connected_edges({{1,2}, {7,8}, {10,11}, {17,18}, {11,1}, {18,7}, {8,17}, {2,11}, {10,2}, {8,18}});
    ASSERT_EQ(2,  closing_facets.size());
    ASSERT_THAT(closing_facets.at(0), ElementsAre(0,2,4,7,8));
    ASSERT_THAT(closing_facets.at(1), ElementsAre(1,3,5,6,9));
}

TEST_F(ClosingFacetComputerTest, contour_does_not_work_for_test_case_11)
{
    ASSERT_THROW(make(case_11()).contour(), InternalErrorException);
}

TEST_F(ClosingFacetComputerTest, contour_does_not_work_for_test_case_15)
{
    ASSERT_THAT(make(case_15()).contour().edge_idx, ElementsAre(2,1,0,3));
}

TEST_F(ClosingFacetComputerTest, bug_detected_in_MeshIntersector)
{
    TestMesh mesh;
    mesh.edges = {{0,1}, {6,7}, {7,1}, {0,8}, {8,6}};
    Eigen::MatrixXd M = Eigen::MatrixXd::Zero(9,3);
    M << -0.5, -0.5,  0.5,
          0.5, -0.5,  0.5,
          0.5,  0.5,  0.5,
         -0.5,  0.5,  0.5,
          0.5, -0.5, -0.5,
         -0.5, -0.5, -0.5,
         -0.5,  0.5, -0.5,
          0.5,  0.5, -0.5,
         -0.5,    0,    0;

    mesh.all_nodes = M.transpose();
    ASSERT_THAT(make(mesh).contour().edge_idx, ElementsAre(3,4,1,2,0));
}

TEST_F(ClosingFacetComputerTest, can_use_only_some_edges_in_mesh)
{
    TestMesh mesh;
    mesh.edges = {{3,4},{0,1}, {6,7}, {2,3} , {7,1}, {0,8}, {8,6},{0,2}};
    Eigen::MatrixXd M = Eigen::MatrixXd::Zero(9,3);
    M << -0.5, -0.5,  0.5,
          0.5, -0.5,  0.5,
          0.5,  0.5,  0.5,
         -0.5,  0.5,  0.5,
          0.5, -0.5, -0.5,
         -0.5, -0.5, -0.5,
         -0.5,  0.5, -0.5,
          0.5,  0.5, -0.5,
         -0.5,    0,    0;

    mesh.all_nodes = M.transpose();
    ASSERT_THAT(make(mesh, {1,2,4,5,6}).contour().edge_idx, ElementsAre(5,6,2,4,1));
}

TEST_F(ClosingFacetComputerTest, can_use_only_some_edges_in_mesh_when_grouping_connected_edges)
{
    const ClosingFacetComputer::ListOfEdges all_edges = {{1,7},{0,1},{0,6},{6,2},{2,5},{6,7},{3,5},{2,4},{4,5},{3,4},{2,3},{0,7},{1,6}};
    const std::vector<size_t> idx_of_relevant_edges = {1,2,4,5,7,8,9,10,11,12};
    const std::vector<std::vector<size_t> > grouped_edges = ClosingFacetComputer::group_connected_edges(all_edges, idx_of_relevant_edges);
    ASSERT_EQ(2,  grouped_edges.size());
    ASSERT_THAT(grouped_edges.at(0), ElementsAre(1,2,5,11,12));
    ASSERT_THAT(grouped_edges.at(1), ElementsAre(4,7,8,9,10));
}

TEST_F(ClosingFacetComputerTest, orientation_should_be_correct_for_case_18)
{
    const auto case_18_contour = make(case_18()).contour();
    ASSERT_THAT(case_18_contour.edge_idx, ElementsAre(3,2,1,0));
    ASSERT_THAT(case_18_contour.reversed, ElementsAre(false,false,false,true));
}

TEST_F(ClosingFacetComputerTest, can_compute_contour_for_case_19)
{
    const auto case_19_contour = make(case_19()).contour(4);
    ASSERT_THAT(case_19_contour.edge_idx, ElementsAre(4,1,2,0));
    ASSERT_THAT(case_19_contour.reversed, ElementsAre(false,true,true,true));
}

TEST_F(ClosingFacetComputerTest, bug_detected_in_MeshIntersectorTest_bug_in_centroid_1)
{
    const auto case_20_contour = make(case_20()).contour(4);
    ASSERT_THAT(case_20_contour.edge_idx, ElementsAre(4,3,2,5,0,1));
    ASSERT_THAT(case_20_contour.reversed, ElementsAre(false,false,false,false,false,false));
    ASSERT_NO_THROW(make(case_20()).contour());
}

TEST_F(ClosingFacetComputerTest, bug_detected_in_MeshIntersectorTest_bug_in_centroid_2)
{
    ASSERT_EQ(2,make(case_20()).next_edge(3));
}

TEST_F(ClosingFacetComputerTest, bug_detected_in_MeshIntersectorTest_bug_in_centroid_3)
{
    auto node2edge = ClosingFacetComputer::get_node_to_connected_edges(case_20().edges);
    auto S0 = node2edge[0];
    auto S1 = node2edge[1];
    auto S2 = node2edge[2];
    auto S3 = node2edge[3];
    auto S4 = node2edge[4];
    auto S5 = node2edge[5];

    ASSERT_EQ(2,S0.size());
    ASSERT_TRUE(has(S0, 0));
    ASSERT_TRUE(has(S0, 5));

    ASSERT_EQ(4,S1.size());
    ASSERT_TRUE(has(S1, 0));
    ASSERT_TRUE(has(S1, 1));
    ASSERT_TRUE(has(S1, 6));
    ASSERT_TRUE(has(S1, 7));

    ASSERT_EQ(3,S2.size());
    ASSERT_TRUE(has(S2, 1));
    ASSERT_TRUE(has(S2, 4));
    ASSERT_TRUE(has(S2, 8));

    ASSERT_EQ(3,S3.size());
    ASSERT_TRUE(has(S3, 2));
    ASSERT_TRUE(has(S3, 5));
    ASSERT_TRUE(has(S3, 6));

    ASSERT_EQ(4,S4.size());
    ASSERT_TRUE(has(S4, 2));
    ASSERT_TRUE(has(S4, 3));
    ASSERT_TRUE(has(S4, 7));
    ASSERT_TRUE(has(S4, 8));

    ASSERT_EQ(2,S5.size());
    ASSERT_TRUE(has(S5, 3));
    ASSERT_TRUE(has(S5, 4));
}

TEST_F(ClosingFacetComputerTest, bug_in_extreme_edges_found_in_MeshIntersectorTest_bug_in_centroid)
{
    const auto extreme_edges = make(case_21()).extreme_edges();
    ASSERT_EQ(0, extreme_edges.first);
    ASSERT_EQ(4, extreme_edges.second);
}

TEST_F(ClosingFacetComputerTest, bug_in_extreme_edges_found_in_MeshIntersectorTest_bug_in_centroid_2)
{
    const auto extreme_edges = make(case_20()).extreme_edges();
    ASSERT_EQ(0, extreme_edges.first);
    ASSERT_EQ(4, extreme_edges.second);
}

TEST_F(ClosingFacetComputerTest, bug_in_next_edge_found_in_MeshIntersectorTest_bug_in_centroid)
{
    ASSERT_EQ(3, make(case_21()).next_edge(2, true));
}

TEST_F(ClosingFacetComputerTest, contour_in_MeshIntersectorTest_bug_in_centroid)
{
    const auto contour = make(case_21()).contour();
    ASSERT_THAT(contour.edge_idx, ElementsAre(1,4,3,2,7,0));
    ASSERT_THAT(contour.reversed, ElementsAre(true,true,true,false,true,true));
}

TEST_F(ClosingFacetComputerTest, facet_orientation_should_be_correct)
{
    const auto contour = make(case_22()).contour();
    ASSERT_THAT(contour.edge_idx, ElementsAre(0,1,2,3,4,5));
    ASSERT_THAT(contour.reversed, ElementsAre(false,false,false,false,false,false));
}
