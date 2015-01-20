/*
 * ManeuveringForceModelTest.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: cady
 */


#include "ManeuveringForceModelTest.hpp"
#include "ManeuveringForceModel.hpp"
#include "yaml_data.hpp"

ManeuveringForceModelTest::ManeuveringForceModelTest() : a(ssc::random_data_generator::DataGenerator(87542))
{
}

ManeuveringForceModelTest::~ManeuveringForceModelTest()
{
}

void ManeuveringForceModelTest::SetUp()
{
}

void ManeuveringForceModelTest::TearDown()
{
}

TEST_F(ManeuveringForceModelTest, can_parse_point_of_application)
{
//! [ManeuveringForceModelTest example]
    const auto data = ManeuveringForceModel::parse(test_data::maneuvering());
//! [ManeuveringForceModelTest example]
//! [ManeuveringForceModelTest expected output]
    ASSERT_DOUBLE_EQ(0.696, data.point_of_application.x);
    ASSERT_DOUBLE_EQ(0, data.point_of_application.y);
    ASSERT_DOUBLE_EQ(1.418, data.point_of_application.z);
//! [ManeuveringForceModelTest expected output]
}

