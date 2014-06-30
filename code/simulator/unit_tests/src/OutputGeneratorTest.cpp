/*
 * OutputGeneratorTest.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */

#include "OutputGeneratorTest.hpp"
#include "OutputGenerator.hpp"
#include "Kinematics.hpp"
#include "SimObserver.hpp"
#include "Sim.hpp"
#include "yaml_data.hpp"
#include "simulator_api.hpp"
#include "steppers.hpp"

#define PI (4.*atan(1.))

OutputGeneratorTest::OutputGeneratorTest() : a(DataGenerator(42022))
{
}

OutputGeneratorTest::~OutputGeneratorTest()
{
}

void OutputGeneratorTest::SetUp()
{
}

void OutputGeneratorTest::TearDown()
{
}

TEST_F(OutputGeneratorTest, DISABLED_acceptance_test)
{
    auto observer = simulate<SimObserver,EulerStepper>(test_data::falling_ball_example(), 0, 2, 1);
    auto res = observer.get();
    ASSERT_EQ(3, res.size());
    ASSERT_EQ(3, res[0].size());
    ASSERT_DOUBLE_EQ(14, res[0]["O/NED->ball(x)"]);
    ASSERT_DOUBLE_EQ(32, res[0]["O/NED->ball(z)"]);
    ASSERT_DOUBLE_EQ(14+2, res[2]["O/NED->ball(x)"]);
    ASSERT_DOUBLE_EQ(32+9.81, res[2]["O/NED->ball(z)"]);
}
