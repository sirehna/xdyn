/*
 * OutputGeneratorTest.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */

#include "OutputTransformerTest.hpp"
#include "OutputTransformer.hpp"
#include "Kinematics.hpp"
#include "SimObserver.hpp"
#include "Sim.hpp"
#include "yaml_data.hpp"
#include "simulator_api.hpp"
#include "steppers.hpp"

#define PI (4.*atan(1.))

OutputTransformerTest::OutputTransformerTest() : a(DataGenerator(42022))
{
}

OutputTransformerTest::~OutputTransformerTest()
{
}

void OutputTransformerTest::SetUp()
{
}

void OutputTransformerTest::TearDown()
{
}

TEST_F(OutputTransformerTest, DISABLED_acceptance_test)
{
    auto res = simulate<EulerStepper>(test_data::falling_ball_example(), 0, 2, 1);
    const OutputTransformer transform;
    std::vector<std::map<std::string,double> > out;
    for (const auto r:res) out.push_back(transform(r));
    ASSERT_EQ(3, out.size());
    ASSERT_EQ(3, out[0].size());
    ASSERT_DOUBLE_EQ(14, out[0]["O/NED->ball(x)"]);
    ASSERT_DOUBLE_EQ(32, out[0]["O/NED->ball(z)"]);
    ASSERT_DOUBLE_EQ(14+2, out[2]["O/NED->ball(x)"]);
    ASSERT_DOUBLE_EQ(32+9.81, out[2]["O/NED->ball(z)"]);
}
