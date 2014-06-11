/*
 * IntegrationTests.cpp
 *
 *  Created on: Jun 5, 2014
 *      Author: cady
 */

#include "IntegrationTests.hpp"
#include "yaml_data.hpp"
#include "DsMapObserver.hpp"
#include "SimulatorYamlParser.hpp"
#include "check_input_yaml.hpp"
#include "simulator_api.hpp"
#include "DsSystem.hpp"
#include "DsSolve.hpp"
#include "GeometricTypes3d.hpp"
#include "STL_data.hpp"
#include "StlReader.hpp"

#define EPS (1E-10)

IntegrationTests::IntegrationTests() : a(DataGenerator(1212))
{
}

IntegrationTests::~IntegrationTests()
{
}

void IntegrationTests::SetUp()
{
}

void IntegrationTests::TearDown()
{
}

TEST_F(IntegrationTests, yaml_data_for_falling_ball_should_be_valid)
{
    check_input_yaml(SimulatorYamlParser(test_data::falling_ball_example()).parse());
}

TEST_F(IntegrationTests, simulator_does_not_crash_for_falling_ball)
{
    DataSource ds = make_ds(test_data::falling_ball_example(),1,"euler");
    DsMapObserver observer;
    integrate(ds, 0, 10, observer);
}

TEST_F(IntegrationTests, can_simulate_falling_ball)
{
    DataSource ds = make_ds(test_data::falling_ball_example(),1,"euler");
    DsMapObserver observer;
    const size_t N = 10;
    integrate(ds, 0, N, observer);
    auto res = observer.get();
    ASSERT_EQ(N+1, res.size());
    const double g = ds.get<double>("g");
    for (size_t i = 0 ; i < N+1 ; ++i)
    {
        const double t = (double) i;
        ASSERT_EQ(1+13, res.at(i).size())          << "Time step: i=" << i;
        ASSERT_DOUBLE_EQ(t, res.at(i)["t"])        << "Time step: i=" << i;
        ASSERT_NEAR(4+1.*t, res.at(i)["x(ball)"], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(8, res.at(i)["y(ball)"], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(12+g*t*(t-1)/2., res.at(i)["z(ball)"], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(1, res.at(i)["u(ball)"], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["v(ball)"], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(g*t, res.at(i)["w(ball)"], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["p(ball)"], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["q(ball)"], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["r(ball)"], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(1, res.at(i)["qr(ball)"], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["qi(ball)"], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["qj(ball)"], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["qk(ball)"], EPS) << "Time step: i=" << i;
    }
}

TEST_F(IntegrationTests, yaml_data_for_oscillating_cube_should_be_valid)
{
    check_input_yaml(SimulatorYamlParser(test_data::oscillating_cube_example()).parse());
}

