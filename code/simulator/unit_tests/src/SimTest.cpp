/*
 * SimTest.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "SimObserver.hpp"
#include "Sim.hpp"
#include "SimTest.hpp"
#include "YamlSimulatorInput.hpp"
#include "yaml_data.hpp"
#include "SimulatorYamlParser.hpp"
#include "STL_data.hpp"
#include "simulator_api.hpp"
#include "SimCsvObserver.hpp"
#include "SimNoObserver.hpp"
#include "steppers.hpp"

#define EPS (1E-10)

SimTest::SimTest() : a(DataGenerator(42222))
{
}

SimTest::~SimTest()
{
}

void SimTest::SetUp()
{
}

void SimTest::TearDown()
{
}

TEST_F(SimTest, can_simulate_falling_ball)
{
    const size_t N = 10;
    auto res = simulate<EulerStepper>(test_data::falling_ball_example(), 0, N, 1);
    ASSERT_EQ(N+1, res.size());
    const double g = 9.81;
    for (size_t i = 0 ; i < N+1 ; ++i)
    {
        const double t = (double) i;
        ASSERT_EQ(13, res.at(i).x.size())                             << "Time step: i=" << i;
        ASSERT_DOUBLE_EQ(t,          res.at(i).t)                << "Time step: i=" << i;
        ASSERT_NEAR(4+1.*t,          res.at(i).x[XIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(8,               res.at(i).x[YIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(12+g*t*(t-1)/2., res.at(i).x[ZIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(1,               res.at(i).x[UIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(0,               res.at(i).x[VIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(g*t,             res.at(i).x[WIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(0,               res.at(i).x[PIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(0,               res.at(i).x[QIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(0,               res.at(i).x[RIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(1,               res.at(i).x[QRIDX(0)], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0,               res.at(i).x[QIIDX(0)], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0,               res.at(i).x[QJIDX(0)], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0,               res.at(i).x[QKIDX(0)], EPS) << "Time step: i=" << i;
    }
}

TEST_F(SimTest, can_simulate_oscillating_cube)
{
    const double dt = 1E-1;
    const double tend = 10;
    auto res = simulate<RK4Stepper>(test_data::oscillating_cube_example(), test_data::cube(), 0, tend, dt);

    const double g = 9.81;
    const double rho = 1026;
    const double L = 1;
    const double m = 1e3;
    const double omega = L*sqrt(rho*g/m);
    const double A = m/(rho*L*L)*(1-rho*L*L*L/(2*m));
    const double z0 = L/2;
    const double eps = 1E-3;
    const size_t N = (size_t)(floor(tend/dt+0.5))+1;
    ASSERT_EQ(N, res.size());
    for (size_t i = 0 ; i < N ; ++i)
    {
        const double t = i*dt;
        ASSERT_EQ(13,                          res.at(i).x.size())         << "Time step: i=" << i;
        ASSERT_DOUBLE_EQ(t,                    res.at(i).t)                << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[XIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[YIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR((z0-A)*cos(omega*t)+A,     res.at(i).x[ZIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[UIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[VIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(omega*(A-z0)*sin(omega*t), res.at(i).x[WIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[PIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[QIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[RIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(1,                         res.at(i).x[QRIDX(0)], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[QIIDX(0)], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[QJIDX(0)], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[QKIDX(0)], EPS) << "Time step: i=" << i;
    }
}

TEST_F(SimTest, can_simulate_stable_cube)
{
    auto sys = get_system(test_data::stable_cube_example(),
                          test_data::cube());

    const double dt = 1E-1;

    SimObserver observer(sys.get_names_of_bodies());
    const double tend = 10;
    const size_t N = (size_t)(floor(tend/dt+0.5))+1;
    quicksolve<RK4Stepper>(sys, 0, tend, dt, observer);
    auto res = observer.get();

    const double rho = 1026;
    const double L = 1;
    const double m = 1e3;
    const double z0 = m/rho-L/2;
    const double eps = 1E-3;
    ASSERT_EQ(N, res.size());
    for (size_t i = 0 ; i < N ; ++i)
    {
        const double t = (double)i*dt;
        ASSERT_EQ(1+13, res.at(i).size())          << "Time step: i=" << i;
        ASSERT_DOUBLE_EQ(t, res.at(i)["t"])        << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["x(cube)"], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["y(cube)"], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(z0, res.at(i)["z(cube)"], eps) << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["u(cube)"], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["v(cube)"], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["w(cube)"], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["p(cube)"], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["q(cube)"], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["r(cube)"], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(1, res.at(i)["qr(cube)"], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["qi(cube)"], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["qj(cube)"], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0, res.at(i)["qk(cube)"], EPS) << "Time step: i=" << i;
    }
}

