/*
 * hydrostaticTest.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#include "hydrostaticTest.hpp"
#include "hydrostatic.hpp"
#include "HydrostaticException.hpp"

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

TEST_F(hydrostaticTest, can_tell_if_three_points_are_totally_immerged_or_not)
{
//! [hydrostaticTest totally_immerged_example]
    size_t index[3]; index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({-1,2,3});
    ASSERT_FALSE(totally_immerged(index, v));
//! [hydrostaticTest totally_immerged_example]
}

TEST_F(hydrostaticTest, can_compute_average_immersion)
{
//! [hydrostaticTest average_immersion_example]
    size_t index[3]; index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({-1,2,5});
    ASSERT_DOUBLE_EQ(2, average_immersion(index, v));
//! [hydrostaticTest average_immersion_example]
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_for_one_immerged_node)
{
//! [hydrostaticTest immerged_polygon_example_1]
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1; M(0,1) = 4; M(0,2) = 7;
    M(1,0) = 1; M(1,1) = 6; M(1,2) = 8;
    M(2,0) = 3; M(2,1) = 6; M(2,2) = 9;

    size_t index[3];
    index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({-1,-1,1});
    const Eigen::Matrix<double,3,Eigen::Dynamic> polygon = immerged_polygon(M, index, v);
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

TEST_F(hydrostaticTest, can_find_index_of_first_and_last_emerged_points)
{
    const std::vector<double> v1({1,2,-3,4,-5,6,7,-8});
    const std::vector<double> v2({-1,-2,3,4,-5,6,-7,8});
    const std::pair<size_t,size_t> idx1 = first_and_last_emerged_points(v1);
    const std::pair<size_t,size_t> idx2 = first_and_last_emerged_points(v2);
    ASSERT_EQ(2,idx1.first);
    ASSERT_EQ(2,idx1.second);
    ASSERT_EQ(0,idx2.first);
    ASSERT_EQ(1,idx2.second);
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_for_two_immerged_node)
{
//! [hydrostaticTest immerged_polygon_example_2]
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1; M(0,1) = 4; M(0,2) = 7;
    M(1,0) = 1; M(1,1) = 6; M(1,2) = 8;
    M(2,0) = 3; M(2,1) = 6; M(2,2) = 9;

    size_t index[3];
    index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({-1,1,1});
    const Eigen::Matrix<double,3,Eigen::Dynamic> polygon = immerged_polygon(M, index, v);
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
//! [hydrostaticTest immerged_polygon_example_2]
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
    size_t index[3];
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
    size_t index[3];
    index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({-1,-2,-5});
    ASSERT_THROW(immerged_polygon(M, index, v), HydrostaticException);
}

