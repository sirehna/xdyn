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

