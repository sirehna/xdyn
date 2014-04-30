/*
 * hydrostaticTest.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#include "hydrostaticTest.hpp"
#include "hydrostatic.hpp"

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
