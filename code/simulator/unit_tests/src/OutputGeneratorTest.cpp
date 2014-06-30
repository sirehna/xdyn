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
#include "solve.hpp"
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
    auto sys = get_system(test_data::falling_ball_example());
    SimObserver observer(sys);
    quicksolve<EulerStepper>(sys, 0, 2, 1, observer);
    auto res = observer.get();
    ASSERT_FALSE(res.empty());
    ASSERT_DOUBLE_EQ(14, res.front()["O/NED->ball(x)"]);
    ASSERT_DOUBLE_EQ(32, res.front()["O/NED->ball(z)"]);
    ASSERT_DOUBLE_EQ(14+2, res.back()["O/NED->ball(x)"]);
    ASSERT_DOUBLE_EQ(32+9.81, res.back()["O/NED->ball(z)"]);
}
