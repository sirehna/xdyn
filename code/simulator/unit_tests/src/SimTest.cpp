/*
 * SimTest.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include <boost/algorithm/string.hpp> // replace in string

#include "SimObserver.hpp"
#include "Sim.hpp"
#include "SimTest.hpp"
#include "YamlSimulatorInput.hpp"
#include "yaml_data.hpp"
#include "SimException.hpp"
#include "SimulatorYamlParser.hpp"
#include "STL_data.hpp"
#include "simulator_api.hpp"
#include "SimCsvObserver.hpp"
#include "SimNoObserver.hpp"
#include "steppers.hpp"
#include "TriMeshTestData.hpp"
#include "generate_anthineas.hpp"

#define EPS (1E-10)
#define SQUARE(x) ((x)*(x))
#define DEG (atan(1.)/45.)

const VectorOfVectorOfPoints SimTest::anthineas_stl = anthineas();

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
    const auto res = simulate<EulerStepper>(test_data::falling_ball_example(), 0, N, 1);
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
    const auto res = simulate<RK4Stepper>(test_data::oscillating_cube_example(), test_data::cube(), 0, tend, dt);

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
        const double t = (double)i*dt;
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
    const double dt = 1E-1;
    const double tend = 10;
    const auto res = simulate<RK4Stepper>(test_data::stable_cube_example(), test_data::cube(), 0, tend, dt);

    const size_t N = (size_t)(floor(tend/dt+0.5))+1;
    const double rho = 1026;
    const double L = 1;
    const double m = 1e3;
    const double z0 = m/rho-L/2;
    const double eps = 1E-3;
    ASSERT_EQ(N, res.size());
    for (size_t i = 0 ; i < N ; ++i)
    {
        const double t = (double)i*dt;
        ASSERT_EQ(13,                          res.at(i).x.size())         << "Time step: i=" << i;
        ASSERT_DOUBLE_EQ(t,                    res.at(i).t)                << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[XIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[YIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(z0,                        res.at(i).x[ZIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[UIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[VIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[WIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[PIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[QIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[RIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(1,                         res.at(i).x[QRIDX(0)], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[QIIDX(0)], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[QJIDX(0)], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0,                         res.at(i).x[QKIDX(0)], EPS) << "Time step: i=" << i;
    }
}

TEST_F(SimTest, initial_angle_should_not_change_results_for_falling_ball)
{
    const size_t N = 3;
    auto yaml = SimulatorYamlParser(test_data::falling_ball_example()).parse();
    yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.theta = 45*DEG;
    yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.u = 0;
    const auto res = simulate<EulerStepper>(yaml, 0, N, 1);
    ASSERT_EQ(N+1, res.size());
    const double g = 9.81;
    for (size_t i = 0 ; i < N+1 ; ++i)
    {
        const double t = (double) i;
        const auto quaternion_norm = SQUARE(res.at(i).x[QRIDX(0)])
                                   + SQUARE(res.at(i).x[QIIDX(0)])
                                   + SQUARE(res.at(i).x[QJIDX(0)])
                                   + SQUARE(res.at(i).x[QKIDX(0)]);
        ASSERT_EQ(13, res.at(i).x.size())                        << "Time step: i=" << i;
        ASSERT_DOUBLE_EQ(t,          res.at(i).t)                << "Time step: i=" << i;
        ASSERT_NEAR(4,               res.at(i).x[XIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(8,               res.at(i).x[YIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(12+g*t*(t-1)/2., res.at(i).x[ZIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(0,               res.at(i).x[PIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(0,               res.at(i).x[QIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(0,               res.at(i).x[RIDX(0)], EPS)  << "Time step: i=" << i;
        ASSERT_NEAR(1,               quaternion_norm, EPS)       << "Time step: i=" << i;
    }
}

TEST_F(SimTest, hydrostatic_test_on_anthineas)
{
    const auto yaml = SimulatorYamlParser(test_data::anthineas_hydrostatic_test()).parse();
    const auto res = simulate<RK4Stepper>(yaml, anthineas_stl, 0, 4.79, 0.479);
    ASSERT_EQ(11, res.size());
    ASSERT_NEAR(0, res[0].x[XIDX(0)], EPS);
    ASSERT_NEAR(0, res[0].x[YIDX(0)], EPS);
    ASSERT_NEAR(-5, res[0].x[ZIDX(0)], EPS);

    for (size_t i = 0 ; i < 5 ; ++i)
    {
        ASSERT_NEAR(res[i].x[ZIDX(0)],res[10-i].x[ZIDX(0)], 0.1) << " i = " << i;
    }
    ASSERT_NEAR(3.84, res[5].x[ZIDX(0)], 1E-2);
    for (size_t i = 0 ; i < 11 ; ++i)
    {
        ASSERT_NEAR(0, fabs(res[i].x[XIDX(0)]), 0.2) << " i = " << i;
        ASSERT_NEAR(0, fabs(res[i].x[YIDX(0)]), 1E-2) << " i = " << i;
    }
}

TEST_F(SimTest, should_throw_if_wave_output_mesh_does_not_exist)
{
    auto input = SimulatorYamlParser(test_data::waves()).parse();
    boost::replace_all(input.environment[0].yaml, "frame of reference: NED", "frame of reference: foo");
    const Sim sys = get_system(input);
    ASSERT_THROW(sys.get_waves(a.random<double>()),SimException);
}

TEST_F(SimTest, can_generate_wave_height_on_mesh)
{
    const Sim sys = get_system(test_data::waves());
    const double t = 9951191801.8523445;
    const std::vector<Point> w = sys.get_waves(t);
    ASSERT_EQ(9, w.size());
    ASSERT_DOUBLE_EQ(1, (double)w[0].x());
    ASSERT_DOUBLE_EQ(1, (double)w[0].y());
    ASSERT_NEAR(-23.180900790763086, (double)w[0].z(), 1E-5);

    ASSERT_DOUBLE_EQ(2, (double)w[1].x());
    ASSERT_DOUBLE_EQ(1, (double)w[1].y());
    ASSERT_NEAR(-23.180874012509221, (double)w[1].z(), 1E-5);

    ASSERT_DOUBLE_EQ(3, (double)w[2].x());
    ASSERT_DOUBLE_EQ(1, (double)w[2].y());
    ASSERT_NEAR(-23.180846368173889, (double)w[2].z(), 1E-5);

    ASSERT_DOUBLE_EQ(4, (double)w[3].x());
    ASSERT_DOUBLE_EQ(1, (double)w[3].y());
    ASSERT_NEAR(-23.1808178639421, (double)w[3].z(), 1E-5);

    ASSERT_DOUBLE_EQ(5, (double)w[4].x());
    ASSERT_DOUBLE_EQ(1, (double)w[4].y());
    ASSERT_NEAR(-23.1807888028812, (double)w[4].z(), 1E-5);

    ASSERT_DOUBLE_EQ(1, (double)w[5].x());
    ASSERT_DOUBLE_EQ(2, (double)w[5].y());
    ASSERT_NEAR(-23.2121733088151, (double)w[5].z(), 1E-5);

    ASSERT_DOUBLE_EQ(2, (double)w[6].x());
    ASSERT_DOUBLE_EQ(2, (double)w[6].y());
    ASSERT_NEAR(-23.2121465379194, (double)w[6].z(), 1E-5);

    ASSERT_DOUBLE_EQ(3, (double)w[7].x());
    ASSERT_DOUBLE_EQ(2, (double)w[7].y());
    ASSERT_NEAR(-23.2121188534555, (double)w[7].z(), 1E-5);

    ASSERT_DOUBLE_EQ(4, (double)w[8].x());
    ASSERT_DOUBLE_EQ(2, (double)w[8].y());
    ASSERT_NEAR(-23.2120903012885, (double)w[8].z(), 1E-5);
}

TEST_F(SimTest, can_generate_wave_height_on_mesh_for_default_wave_model)
{
    const Sim sys = get_system(test_data::stable_rolling_cube_test(), test_data::cube());
    const std::vector<Point> w = sys.get_waves(a.random<double>());
    ASSERT_EQ(9, w.size());
    for (size_t i = 0 ; i < 9 ; ++i)
    {
        ASSERT_DOUBLE_EQ(0, (double)w[i].z());
    }
}

TEST_F(SimTest, waves_test_on_anthineas)
{
    const auto yaml = SimulatorYamlParser(test_data::anthineas_waves_test()).parse();
    const auto res = simulate<RK4Stepper>(yaml, anthineas_stl, 0, 4.79, 0.479);
    ASSERT_EQ(11, res.size());
    ASSERT_NEAR(0, res[0].x[XIDX(0)], EPS);
    ASSERT_NEAR(0, res[0].x[YIDX(0)], EPS);
    ASSERT_NEAR(-5, res[0].x[ZIDX(0)], EPS);
}
