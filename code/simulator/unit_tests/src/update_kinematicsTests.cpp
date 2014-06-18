/*
 * update_kinematicsTests.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#include "update_kinematics.hpp"
#include "update_kinematicsTests.hpp"

update_kinematicsTests::update_kinematicsTests() : a(DataGenerator(121))
{
}

update_kinematicsTests::~update_kinematicsTests()
{
}

void update_kinematicsTests::SetUp()
{
}

void update_kinematicsTests::TearDown()
{
}

TEST_F(update_kinematicsTests, frame_of_origin_point_is_correct)
{
    const Point O = get_origin(a.random_vector_of<double>().of_size(1000), 46);
    ASSERT_EQ("NED", O.get_frame());
}



