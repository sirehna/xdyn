/*
 * update_kinematicsTests.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#include "update_kinematics.hpp"
#include "update_kinematicsTests.hpp"
#include "BodyBuilderTest.hpp"
#include <ssc/kinematics.hpp>
#include "SimulatorYamlParser.hpp"
#include "yaml_data.hpp"

#define EPS (1E-10)

update_kinematicsTests::update_kinematicsTests() : a(ssc::random_data_generator::DataGenerator(121))
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

TEST_F(update_kinematicsTests, can_get_initial_states)
{
    const auto yaml = SimulatorYamlParser(test_data::full_example()).parse();
    const StateType x = get_initial_states(yaml.rotations, yaml.bodies);
    ASSERT_EQ(13, x.size());
    ASSERT_DOUBLE_EQ(4, x.at(0));
    ASSERT_DOUBLE_EQ(8, x.at(1));
    ASSERT_DOUBLE_EQ(12, x.at(2));
    ASSERT_DOUBLE_EQ(-8, x.at(3));
    ASSERT_DOUBLE_EQ(-9, x.at(4));
    ASSERT_DOUBLE_EQ(14, x.at(5));
    ASSERT_DOUBLE_EQ(56, x.at(6));
    ASSERT_DOUBLE_EQ(7, x.at(7));
    ASSERT_DOUBLE_EQ(6, x.at(8));

    ASSERT_NEAR(cos(1.3/2)*cos(1.4/2)*cos(1.5/2)+sin(1.3/2)*sin(1.4/2)*sin(1.5/2), x.at(9), EPS);
    ASSERT_NEAR(sin(1.3/2)*cos(1.4/2)*cos(1.5/2)-cos(1.3/2)*sin(1.4/2)*sin(1.5/2), x.at(10), EPS);
    ASSERT_NEAR(cos(1.3/2)*sin(1.4/2)*cos(1.5/2)+sin(1.3/2)*cos(1.4/2)*sin(1.5/2), x.at(11), EPS);
    ASSERT_NEAR(cos(1.3/2)*cos(1.4/2)*sin(1.5/2)-sin(1.3/2)*sin(1.4/2)*cos(1.5/2), x.at(12), EPS);
}
