/*
 * hydrostaticTest.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#include "hydrostaticTest.hpp"
#include "hydrostatic.hpp"
#include "HydrostaticException.hpp"
#include "TriMeshTestData.hpp"
#include "MeshBuilder.hpp"

hydrostaticTest::hydrostaticTest() : a(DataGenerator(2))
{
}

hydrostaticTest::~hydrostaticTest()
{
}

void hydrostaticTest::SetUp()
{
}

void hydrostaticTest::TearDown()
{
}

using namespace hydrostatic;

TEST_F(hydrostaticTest, can_compute_average_immersion)
{
//! [hydrostaticTest average_immersion_example]
    const Mesh mesh = MeshBuilder(trapezium()).build();
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double h = a.random<double>();
        ASSERT_DOUBLE_EQ(5*h/12., average_immersion(mesh.nodes, mesh.facets.front().index, {0,0,h,h}));
    }
//! [hydrostaticTest average_immersion_example]
}

TEST_F(hydrostaticTest, can_compute_average_immersion_even_when_all_nodes_are_used)
{
    const Mesh mesh = MeshBuilder(one_triangle()).build();
    ASSERT_DOUBLE_EQ(5, average_immersion(mesh.nodes, {4,5,6}));
}

TEST_F(hydrostaticTest, average_immersion_should_throw_if_index_does_not_have_the_right_size)
{
    const Mesh mesh = MeshBuilder(one_triangle()).build();
    ASSERT_THROW(average_immersion(mesh.nodes, {4,5}), HydrostaticException);
    ASSERT_THROW(average_immersion(mesh.nodes, {4,5,6,7}), HydrostaticException);
}

TEST_F(hydrostaticTest, can_compute_average_immersion_even_when_not_all_nodes_are_used)
{
    const Mesh mesh = MeshBuilder(two_triangles()).build();
    ASSERT_DOUBLE_EQ(7/3., average_immersion(mesh.nodes, mesh.facets.back().index, {1,2,3,4}));
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_for_one_immerged_node)
{
//! [hydrostaticTest immerged_polygon_example_1]
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1; M(0,1) = 4; M(0,2) = 7;
    M(1,0) = 1; M(1,1) = 6; M(1,2) = 8;
    M(2,0) = 3; M(2,1) = 6; M(2,2) = 9;

    std::vector<size_t> index(3);
    index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({-1,-1,1});
    const auto polygon_and_immersions = immerged_polygon(M, index, v);
    const auto polygon = polygon_and_immersions.first;
    ASSERT_EQ(3, polygon.cols());
    ASSERT_DOUBLE_EQ(4,polygon(0,0));
    ASSERT_DOUBLE_EQ(4.5,polygon(1,0));
    ASSERT_DOUBLE_EQ(6,polygon(2,0));
    ASSERT_DOUBLE_EQ(5.5,polygon(0,1));
    ASSERT_DOUBLE_EQ(7,polygon(1,1));
    ASSERT_DOUBLE_EQ(7.5,polygon(2,1));
    ASSERT_DOUBLE_EQ(7,polygon(0,2));
    ASSERT_DOUBLE_EQ(8,polygon(1,2));
    ASSERT_DOUBLE_EQ(9,polygon(2,2));
    ASSERT_EQ(3, polygon_and_immersions.second.size());
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(0));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(1));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(2));
//! [hydrostaticTest immerged_polygon_example_1]
}

TEST_F(hydrostaticTest, can_find_the_next_point)
{
    const std::vector<size_t> idx({1,3,8});
    ASSERT_THROW(next(idx, 2), HydrostaticException);
    ASSERT_EQ(3, next(idx,1));
    ASSERT_EQ(8, next(idx,3));
    ASSERT_EQ(1, next(idx,8));
}

TEST_F(hydrostaticTest, can_find_the_previous_point)
{
    const std::vector<size_t> idx({1,3,8});
    ASSERT_THROW(previous(idx, 2), HydrostaticException);
    ASSERT_EQ(8, previous(idx,1));
    ASSERT_EQ(1, previous(idx,3));
    ASSERT_EQ(3, previous(idx,8));
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_for_two_immerged_node)
{
//! [hydrostaticTest immerged_polygon_example_2]
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1; M(0,1) = 4; M(0,2) = 7;
    M(1,0) = 1; M(1,1) = 6; M(1,2) = 8;
    M(2,0) = 3; M(2,1) = 6; M(2,2) = 9;

    std::vector<size_t> index(3);
    index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({-1,1,1});
    const auto polygon_and_immersions = immerged_polygon(M, index, v);
    const auto polygon = polygon_and_immersions.first;
    ASSERT_EQ(4, polygon.cols());
    ASSERT_DOUBLE_EQ(2.5,polygon(0,0));
    ASSERT_DOUBLE_EQ(3.5,polygon(1,0));
    ASSERT_DOUBLE_EQ(4.5,polygon(2,0));
    ASSERT_DOUBLE_EQ(4,polygon(0,1));
    ASSERT_DOUBLE_EQ(6,polygon(1,1));
    ASSERT_DOUBLE_EQ(6,polygon(2,1));
    ASSERT_DOUBLE_EQ(7,polygon(0,2));
    ASSERT_DOUBLE_EQ(8,polygon(1,2));
    ASSERT_DOUBLE_EQ(9,polygon(2,2));
    ASSERT_DOUBLE_EQ(4,polygon(0,3));
    ASSERT_DOUBLE_EQ(4.5,polygon(1,3));
    ASSERT_DOUBLE_EQ(6,polygon(2,3));
    ASSERT_EQ(4, polygon_and_immersions.second.size());
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(0));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(1));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(2));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(3));
//! [hydrostaticTest immerged_polygon_example_2]
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_when_emerged_nodes_are_the_first_nodes)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    std::vector<size_t> index(5);
    index[0] = 0; index[1] = 1; index[2] = 2; index[3] = 3; index[4] = 4;
    const std::vector<double> v({-1,-1,-1,1,1});
    const auto polygon_and_immersions = immerged_polygon(M, index, v);
    const auto polygon = polygon_and_immersions.first;
    ASSERT_EQ(4, polygon.cols());
    ASSERT_DOUBLE_EQ(5,polygon(0,0));
    ASSERT_DOUBLE_EQ(5,polygon(1,0));
    ASSERT_DOUBLE_EQ(6.5,polygon(2,0));
    ASSERT_DOUBLE_EQ(4.5,polygon(0,1));
    ASSERT_DOUBLE_EQ(5,polygon(1,1));
    ASSERT_DOUBLE_EQ(5.5,polygon(2,1));
    ASSERT_DOUBLE_EQ(5,polygon(0,2));
    ASSERT_DOUBLE_EQ(6,polygon(1,2));
    ASSERT_DOUBLE_EQ(6,polygon(2,2));
    ASSERT_DOUBLE_EQ(9,polygon(0,3));
    ASSERT_DOUBLE_EQ(9,polygon(1,3));
    ASSERT_DOUBLE_EQ(11,polygon(2,3));
    ASSERT_EQ(4, polygon_and_immersions.second.size());
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(0));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(1));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(2));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(3));
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_when_emerged_nodes_are_the_last_nodes)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    std::vector<size_t> index(5);
    index[0] = 0; index[1] = 1; index[2] = 2; index[3] = 3; index[4] = 4;
    const std::vector<double> v({1,1,-1,-1,-1});
    const auto polygon_and_immersions = immerged_polygon(M, index, v);
    const auto polygon = polygon_and_immersions.first;
    ASSERT_EQ(4, polygon.cols());
    ASSERT_DOUBLE_EQ(1,polygon(0,0));
    ASSERT_DOUBLE_EQ(1,polygon(1,0));
    ASSERT_DOUBLE_EQ(2,polygon(2,0));
    ASSERT_DOUBLE_EQ(2,polygon(0,1));
    ASSERT_DOUBLE_EQ(3,polygon(1,1));
    ASSERT_DOUBLE_EQ(3,polygon(2,1));
    ASSERT_DOUBLE_EQ(3,polygon(0,2));
    ASSERT_DOUBLE_EQ(3.5,polygon(1,2));
    ASSERT_DOUBLE_EQ(4,polygon(2,2));
    ASSERT_DOUBLE_EQ(5,polygon(0,3));
    ASSERT_DOUBLE_EQ(5,polygon(1,3));
    ASSERT_DOUBLE_EQ(6.5 ,polygon(2,3));
    ASSERT_EQ(4, polygon_and_immersions.second.size());
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(0));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(1));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(2));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(3));
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_when_emerged_nodes_are_in_the_middle)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    std::vector<size_t> index(5);
    index[0] = 0; index[1] = 1; index[2] = 2; index[3] = 3; index[4] = 4;
    const std::vector<double> v({1,-1,-1,-1,1});
    const auto polygon_and_immersions = immerged_polygon(M, index, v);
    const auto polygon = polygon_and_immersions.first;
    ASSERT_EQ(4, polygon.cols());
    ASSERT_DOUBLE_EQ(1,polygon(0,0));
    ASSERT_DOUBLE_EQ(1,polygon(1,0));
    ASSERT_DOUBLE_EQ(2,polygon(2,0));
    ASSERT_DOUBLE_EQ(1.5,polygon(0,1));
    ASSERT_DOUBLE_EQ(2,polygon(1,1));
    ASSERT_DOUBLE_EQ(2.5,polygon(2,1));
    ASSERT_DOUBLE_EQ(7,polygon(0,2));
    ASSERT_DOUBLE_EQ(7.5,polygon(1,2));
    ASSERT_DOUBLE_EQ(8.5,polygon(2,2));
    ASSERT_DOUBLE_EQ(9,polygon(0,3));
    ASSERT_DOUBLE_EQ(9,polygon(1,3));
    ASSERT_DOUBLE_EQ(11 ,polygon(2,3));
    ASSERT_EQ(4, polygon_and_immersions.second.size());
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(0));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(1));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(2));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(3));
}

TEST_F(hydrostaticTest, immerged_polygon_should_throw_if_all_points_are_immerged)
{
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1;
    M(1,0) = 2;
    M(2,0) = 3;
    M(0,1) = 4;
    M(1,1) = 5;
    M(2,1) = 6;
    M(0,2) = 7;
    M(1,2) = 8;
    M(2,2) = 9;
    std::vector<size_t> index(3);
    index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({1,2,5});
    ASSERT_THROW(immerged_polygon(M, index, v), HydrostaticException);
}

TEST_F(hydrostaticTest, can_compute_intersection_with_the_surface)
{
    const EPoint A(1,5,9);
    const EPoint B(98,2,0);
    const double dzA = 77;
    const double dzB = -1;
    const EPoint I = intersection(A, dzA, B, dzB);
    ASSERT_DOUBLE_EQ(77./78.*97+1, I(0));
    ASSERT_DOUBLE_EQ(5-77./78.*3, I(1));
    ASSERT_DOUBLE_EQ(9-77./78.*9, I(2));
}

TEST_F(hydrostaticTest, can_compute_intersection_with_the_surface_second_test)
{
    const EPoint A(-98,-2,9);
    const EPoint B(98,2,-9);
    const double dzA = 9;
    const double dzB = -9;
    const EPoint I = intersection(A, dzA, B, dzB);
    ASSERT_DOUBLE_EQ(0, I(0));
    ASSERT_DOUBLE_EQ(0, I(1));
    ASSERT_DOUBLE_EQ(0, I(2));
}

TEST_F(hydrostaticTest, can_compute_intersection_with_the_surface_third_test)
{
    const EPoint A(-1,-2,10);
    const EPoint B(99,2,-10);
    const double dzA = 1;
    const double dzB = -3;
    const EPoint I = intersection(A, dzA, B, dzB);
    ASSERT_DOUBLE_EQ(24, I(0));
    ASSERT_DOUBLE_EQ(-1, I(1));
    ASSERT_DOUBLE_EQ(5, I(2));
}

TEST_F(hydrostaticTest, immerged_polygon_should_throw_if_no_points_are_immerged)
{
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1;
    M(1,0) = 2;
    M(2,0) = 3;
    M(0,1) = 4;
    M(1,1) = 5;
    M(2,1) = 6;
    M(0,2) = 7;
    M(1,2) = 8;
    M(2,2) = 9;
    std::vector<size_t> index(3);
    index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({-1,-2,-5});
    ASSERT_THROW(immerged_polygon(M, index, v), HydrostaticException);
}

TEST_F(hydrostaticTest, can_compute_the_elementary_hydrostatic_force)
{
    const std::string frame = a.random<std::string>();
    const Point O(frame, 1,2,4);
    const Eigen::Vector3d C(78,-4,6);
    const Eigen::Vector3d dS(0,3,4);
    const double rho = 1024;
    const double g = 10;
    const double dz = 3;
    const UnsafeWrench Fhs = dF(O, C, rho, g, dz, dS);

    ASSERT_EQ(frame, Fhs.get_frame());

    ASSERT_DOUBLE_EQ(0, Fhs.X);
    ASSERT_DOUBLE_EQ(1024*10*3*3, Fhs.Y);
    ASSERT_DOUBLE_EQ(1024*10*3*4, Fhs.Z);
    ASSERT_DOUBLE_EQ(-6*Fhs.Z-2*Fhs.Y, Fhs.K);
    ASSERT_DOUBLE_EQ(-77*Fhs.Z, Fhs.M);
    ASSERT_DOUBLE_EQ(77*Fhs.Y, Fhs.N);
}

TEST_F(hydrostaticTest, bug_in_immerged_polygon)
{
    const Mesh mesh = MeshBuilder(two_triangles()).build();
    const std::vector<double> z = {-1,-1,-2,1};
    ASSERT_NO_THROW(immerged_polygon(mesh.nodes, mesh.facets.at(1).index, z));
}

TEST_F(hydrostaticTest, another_bug_in_immerged_polygon)
{
    const Mesh mesh = MeshBuilder(two_triangles()).build();
    const std::vector<double> z = {-1,-1,-2,1};
    const auto polygon_and_immersions = immerged_polygon(mesh.nodes, mesh.facets.at(1).index, z);
    const auto polygon = polygon_and_immersions.first;

    ASSERT_DOUBLE_EQ(1, polygon(0,0));
    ASSERT_DOUBLE_EQ(-0.5, polygon(1,0));
    ASSERT_DOUBLE_EQ(0, polygon(2,0));

    ASSERT_DOUBLE_EQ(2, polygon(0,1));
    ASSERT_DOUBLE_EQ(-1, polygon(1,1));
    ASSERT_DOUBLE_EQ(0, polygon(2,1));

    ASSERT_DOUBLE_EQ(3, polygon(0,2));
    ASSERT_DOUBLE_EQ(-0.5, polygon(1,2));
    ASSERT_DOUBLE_EQ(0, polygon(2,2));

    ASSERT_EQ(3, polygon_and_immersions.second.size());
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(0));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(1));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(2));
}

TEST_F(hydrostaticTest, bug_in_first_and_last_emerged_points)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,1,-1});
    ASSERT_EQ(2,first_and_last.first);
    ASSERT_EQ(0,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T01)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,-1,-1,1,1,1,-1,-1,-1});
    ASSERT_EQ(6,first_and_last.first);
    ASSERT_EQ(2,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T02)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({1,1,1,-1,-1,1,1});
    ASSERT_EQ(3,first_and_last.first);
    ASSERT_EQ(4,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T03)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,-1,-1,1,1,1});
    ASSERT_EQ(0,first_and_last.first);
    ASSERT_EQ(2,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T04)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({1,1,1,-1,-1,-1});
    ASSERT_EQ(3,first_and_last.first);
    ASSERT_EQ(5,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T05)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,-1,-1,1});
    ASSERT_EQ(0,first_and_last.first);
    ASSERT_EQ(2,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T06)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,-1,1,-1});
    ASSERT_EQ(3,first_and_last.first);
    ASSERT_EQ(1,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T07)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,1,-1,-1});
    ASSERT_EQ(2,first_and_last.first);
    ASSERT_EQ(0,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T08)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,1,1,1});
    ASSERT_EQ(0,first_and_last.first);
    ASSERT_EQ(0,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T09)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({1,1,1,-1});
    ASSERT_EQ(3,first_and_last.first);
    ASSERT_EQ(3,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T10)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({1,1,-1,1});
    ASSERT_EQ(2,first_and_last.first);
    ASSERT_EQ(2,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_should_throw_if_set_of_emerged_points_is_not_convex)
{
    ASSERT_THROW(first_and_last_emerged_points({1,1,-1,-1,1,-1,-1}), HydrostaticException);
    ASSERT_THROW(first_and_last_emerged_points({1,1,-1,-1,1,-1,-1,1}), HydrostaticException);
    ASSERT_THROW(first_and_last_emerged_points({-1,-1,1,-1,-1,1}), HydrostaticException);
    ASSERT_THROW(first_and_last_emerged_points({1,-1,-1,1,-1,-1}), HydrostaticException);
}
