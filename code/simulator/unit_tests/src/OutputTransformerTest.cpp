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
#include <ssc/solver.hpp>
#include "SimulatorYamlParser.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define EPS (1E-10)

const YamlSimulatorInput OutputTransformerTest::falling_ball_yaml = OutputTransformerTest::get_falling_ball_yaml();
const YamlSimulatorInput OutputTransformerTest::full_example_yaml = OutputTransformerTest::get_full_example_yaml();
const YamlSimulatorInput OutputTransformerTest::rolling_cube_yaml = OutputTransformerTest::get_rolling_cube_yaml();
const YamlSimulatorInput OutputTransformerTest::falling_cube_yaml = OutputTransformerTest::get_falling_cube_yaml();

YamlSimulatorInput OutputTransformerTest::get_falling_ball_yaml()
{
    return SimulatorYamlParser(test_data::falling_ball_example()+test_data::outputs()).parse();
}

YamlSimulatorInput OutputTransformerTest::get_full_example_yaml()
{
    YamlSimulatorInput ret = SimulatorYamlParser(test_data::full_example()).parse();
    ret.bodies.front().mesh = "";
    return ret;
}

YamlSimulatorInput OutputTransformerTest::get_rolling_cube_yaml()
{
    YamlSimulatorInput ret = SimulatorYamlParser(test_data::rolling_cube()).parse();
    ret.bodies.front().mesh = "";
    return ret;
}

YamlSimulatorInput OutputTransformerTest::get_falling_cube_yaml()
{
    YamlSimulatorInput ret = SimulatorYamlParser(test_data::falling_cube()).parse();
    ret.bodies.front().mesh = "";
    return ret;
}

OutputTransformerTest::OutputTransformerTest() :
                a(ssc::random_data_generator::DataGenerator(42022)),
                falling_ball(std::vector<std::map<std::string,double> >()),
                full_example(std::vector<std::map<std::string,double> >()),
                rolling_cube(std::vector<std::map<std::string,double> >()),
                falling_cube(std::vector<std::map<std::string,double> >())
{
    auto res1 = simulate<ssc::solver::EulerStepper>(falling_ball_yaml, 0, 2, 1);
    const OutputTransformer transform1(falling_ball_yaml);
    for (auto r=res1.begin() ; r != res1.end() ; ++r) falling_ball.push_back(transform1(*r));

    auto res2 = simulate<ssc::solver::EulerStepper>(full_example_yaml, 0, 2, 1);
    const OutputTransformer transform2(full_example_yaml);
    for (auto r=res2.begin() ; r != res2.end() ; ++r) full_example.push_back(transform2(*r));

    auto res3 = simulate<ssc::solver::EulerStepper>(rolling_cube_yaml, 0, 2, 1);
    const OutputTransformer transform3(rolling_cube_yaml);
    for (auto r=res2.begin() ; r != res2.end() ; ++r) rolling_cube.push_back(transform2(*r));

    auto res4 = simulate<ssc::solver::EulerStepper>(falling_cube_yaml, 0, 2, 1);
    const OutputTransformer transform4(falling_cube_yaml);
    for (auto r=res2.begin() ; r != res2.end() ; ++r) falling_cube.push_back(transform2(*r));
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
    ASSERT_EQ(3, falling_ball.size());
    ASSERT_EQ(3, full_example.size());
    ASSERT_EQ(3, rolling_cube.size());
    ASSERT_EQ(3, falling_cube.size());
}

TEST_F(OutputTransformerTest, position_outputs_should_exist_in_map)
{
    ASSERT_NE(falling_ball.at(0).end(), falling_ball.at(0).find("x(O in NED / ball -> ball)"));
    ASSERT_NE(falling_ball.at(0).end(), falling_ball.at(0).find("z(O in NED / ball -> ball)"));
}

TEST_F(OutputTransformerTest, angle_outputs_should_exist_in_map)
{
    ASSERT_NE(full_example.at(0).end(), full_example.at(0).find("phi(body 1 / NED -> body 1)"));
    ASSERT_NE(full_example.at(0).end(), full_example.at(0).find("theta(body 1 / NED -> body 1)"));
    ASSERT_NE(full_example.at(0).end(), full_example.at(0).find("psi(body 1 / NED -> body 1)"));
}

TEST_F(OutputTransformerTest, can_compute_positions)
{
    const auto x0 = falling_ball.at(0)["x(O in NED / ball -> ball)"];
    const auto z0 = falling_ball.at(0)["z(O in NED / ball -> ball)"];
    const auto x2 = falling_ball.at(2)["x(O in NED / ball -> ball)"];
    const auto z2 = falling_ball.at(2)["z(O in NED / ball -> ball)"];
    ASSERT_DOUBLE_EQ(-4,         x0) << "t = 0";
    ASSERT_DOUBLE_EQ(-12,        z0) << "t = 0";
    ASSERT_DOUBLE_EQ(-(4+2),     x2) << "t = 2";
    ASSERT_DOUBLE_EQ(-(12+9.81), z2) << "t = 2";
}

TEST_F(OutputTransformerTest, can_compute_angles)
{
    const auto phi   = full_example.at(0)["phi(body 1 / NED -> body 1)"];
    const auto theta = full_example.at(0)["theta(body 1 / NED -> body 1)"];
    const auto psi   = full_example.at(0)["psi(body 1 / NED -> body 1)"];
    EXPECT_NEAR(1.3, phi, EPS);
    EXPECT_NEAR(1.4, theta, EPS);
    EXPECT_NEAR(1.5, psi, EPS);
}
