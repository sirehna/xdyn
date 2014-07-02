/*
 * OutputGeneratorTest.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */

#include "OutputTransformerTest.hpp"
#include "OutputTransformer.hpp"
#include "yaml_data.hpp"
#include "simulator_api.hpp"
#include "steppers.hpp"
#include "SimulatorYamlParser.hpp"
#include "test_macros.hpp"
#define PI (4.*atan(1.))

const YamlSimulatorInput OutputTransformerTest::yaml = OutputTransformerTest::get_yaml();

YamlSimulatorInput OutputTransformerTest::get_yaml()
{
    return SimulatorYamlParser(test_data::falling_ball_example()).parse();
}

OutputTransformerTest::OutputTransformerTest() : a(DataGenerator(42022)), out(std::vector<std::map<std::string,double> >())
{
    auto res = simulate<EulerStepper>(yaml, 0, 2, 1);
    const OutputTransformer transform(yaml);
    for (const auto r:res) out.push_back(transform(r));
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

TEST_F(OutputTransformerTest, processed_output_should_have_the_right_size)
{
    ASSERT_EQ(3, out.size());
}

TEST_F(OutputTransformerTest, can_compute_positions)
{
    const auto x0 = out.at(0)["x(O in NED / ball -> ball)"];
    const auto z0 = out.at(0)["z(O in NED / ball -> ball)"];
    const auto x2 = out.at(2)["x(O in NED / ball -> ball)"];
    const auto z2 = out.at(2)["z(O in NED / ball -> ball)"];
    ASSERT_DOUBLE_EQ(-4, x0) << "t = 0";
    ASSERT_DOUBLE_EQ(-12, z0) << "t = 0";
    ASSERT_DOUBLE_EQ(-(4+2), x2) << "t = 2";
    ASSERT_DOUBLE_EQ(-(12+9.81), z2) << "t = 2";
}
