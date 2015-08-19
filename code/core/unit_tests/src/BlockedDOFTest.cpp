/*
 * BlockedDOFTest.cpp
 *
 *  Created on: Aug 19, 2015
 *      Author: cady
 */


#include "BlockedDOFTest.hpp"

BlockedDOFTest::BlockedDOFTest() : a(ssc::random_data_generator::DataGenerator(854512))
{
}

BlockedDOFTest::~BlockedDOFTest()
{
}

void BlockedDOFTest::SetUp()
{
}

void BlockedDOFTest::TearDown()
{
}

TEST_F(BlockedDOFTest, DISABLED_can_parse_forced_dof)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_should_throw_if_forcing_anything_other_than_uvwpqr)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_should_throw_if_forcing_same_state_twice)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_should_throw_if_both_read_from_and_value_are_specified)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_should_throw_if_not_as_many_values_as_instants)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_should_throw_if_t_not_strictly_increasing)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_interpolation_type_should_be_valid)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_should_throw_if_CSV_file_does_not_exist)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_piecewise_constant_should_work)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_linear_should_work)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_spline_should_work)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_blocked_derivative_should_work)
{
    ASSERT_TRUE(false);
}

TEST_F(BlockedDOFTest, DISABLED_force_delta)
{
    ASSERT_TRUE(false);
}
