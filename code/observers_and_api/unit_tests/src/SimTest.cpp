/*
 * SimTest.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */
#include <math.h> // isnan

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include <fstream>

#include <boost/algorithm/string.hpp> // replace in string

#include <ssc/data_source.hpp>

#include "SimObserver.hpp"
#include "Sim.hpp"
#include "SimTest.hpp"
#include "YamlSimulatorInput.hpp"
#include "yaml_data.hpp"
#include "InternalErrorException.hpp"
#include "SimulatorYamlParser.hpp"
#include "stl_data.hpp"
#include "simulator_api.hpp"
#include <ssc/solver.hpp>
#include "TriMeshTestData.hpp"
#include "generate_test_ship.hpp"
#include "hdb_data.hpp"
#include "parse_output.hpp"
#include "ListOfObservers.hpp"
#include "MapObserverTest.hpp"

#define EPS (1E-10)
#define SQUARE(x) ((x)*(x))
#define DEG (atan(1.)/45.)

const VectorOfVectorOfPoints SimTest::test_ship_stl = test_ship();

SimTest::SimTest() : a(ssc::random_data_generator::DataGenerator(42222))
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
    const auto res = simulate<ssc::solver::EulerStepper>(test_data::falling_ball_example(), 0, N, 1);
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

TEST_F(SimTest, LONG_can_simulate_oscillating_cube)
{
    const double dt = 1E-1;
    const double tend = 10;
    const auto res = simulate<ssc::solver::RK4Stepper>(test_data::oscillating_cube_example(), test_data::cube(), 0, tend, dt);

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

TEST_F(SimTest, LONG_can_simulate_stable_cube)
{
    const double dt = 1E-1;
    const double tend = 10;
    const auto res = simulate<ssc::solver::RK4Stepper>(test_data::stable_cube_example(), test_data::cube(), 0, tend, dt);

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
        ASSERT_EQ(13,       res.at(i).x.size())         << "Time step: i=" << i;
        ASSERT_DOUBLE_EQ(t, res.at(i).t)                << "Time step: i=" << i;
        ASSERT_NEAR(0,      res.at(i).x[XIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,      res.at(i).x[YIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(z0,     res.at(i).x[ZIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,      res.at(i).x[UIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,      res.at(i).x[VIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,      res.at(i).x[WIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,      res.at(i).x[PIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,      res.at(i).x[QIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(0,      res.at(i).x[RIDX(0)], eps)  << "Time step: i=" << i;
        ASSERT_NEAR(1,      res.at(i).x[QRIDX(0)], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0,      res.at(i).x[QIIDX(0)], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0,      res.at(i).x[QJIDX(0)], EPS) << "Time step: i=" << i;
        ASSERT_NEAR(0,      res.at(i).x[QKIDX(0)], EPS) << "Time step: i=" << i;
    }
}

TEST_F(SimTest, initial_angle_should_not_change_results_for_falling_ball)
{
    const size_t N = 3;
    auto yaml = SimulatorYamlParser(test_data::falling_ball_example()).parse();
    yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.theta = 45*DEG;
    yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.u = 0;
    const auto res = simulate<ssc::solver::EulerStepper>(yaml, 0, N, 1);
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

TEST_F(SimTest, LONG_hydrostatic_test_on_test_ship)
{
    const auto yaml = SimulatorYamlParser(test_data::test_ship_fast_hydrostatic_test()).parse();
    const auto res = simulate<ssc::solver::RK4Stepper>(yaml, test_ship_stl, 0, 4.79, 0.479);
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

TEST_F(SimTest, LONG_exact_hydrostatic_test_on_test_ship)
{
    const auto yaml = SimulatorYamlParser(test_data::test_ship_exact_hydrostatic_test()).parse();
    const auto res = simulate<ssc::solver::RK4Stepper>(yaml, test_ship_stl, 0, 0.1, 0.4);
}

TEST_F(SimTest, should_throw_if_wave_output_mesh_does_not_exist)
{
    auto input = SimulatorYamlParser(test_data::waves()).parse();
    boost::replace_all(input.environment[0].yaml, "frame of reference: NED", "frame of reference: foo");
    const Sim sys = get_system(input, 0);
    ASSERT_THROW(sys.get_waves(a.random<double>()),InternalErrorException);
}

TEST_F(SimTest, can_generate_wave_height_on_mesh)
{
    const Sim sys = get_system(test_data::waves(), 0);
    const double t = 9951191801.8523445;
    const ssc::kinematics::PointMatrix w = sys.get_waves(t);

    ASSERT_EQ(10, w.m.cols());
    ASSERT_DOUBLE_EQ(1, (double)w.m(0,0));
    ASSERT_DOUBLE_EQ(1, (double)w.m(1,0));
    EXPECT_NEAR(-2.0378067059984941, (double)w.m(2,0), 1E-5);

    ASSERT_DOUBLE_EQ(2, (double)w.m(0,1));
    ASSERT_DOUBLE_EQ(1, (double)w.m(1,1));
    EXPECT_NEAR(-2.0378386276758222, (double)w.m(2,1), 1E-5);

    ASSERT_DOUBLE_EQ(3, (double)w.m(0,2));
    ASSERT_DOUBLE_EQ(1, (double)w.m(1,2));
    EXPECT_NEAR(-2.0378702477809414, (double)w.m(2,2), 1E-5);

    ASSERT_DOUBLE_EQ(4, (double)w.m(0,3));
    ASSERT_DOUBLE_EQ(1, (double)w.m(1,3));
    EXPECT_NEAR(-2.0379014849156922, (double)w.m(2,3), 1E-5);

    ASSERT_DOUBLE_EQ(5, (double)w.m(0,4));
    ASSERT_DOUBLE_EQ(1, (double)w.m(1,4));
    EXPECT_NEAR(-2.0379327524655073, (double)w.m(2,4), 1E-5);

    ASSERT_DOUBLE_EQ(1, (double)w.m(0,5));
    ASSERT_DOUBLE_EQ(2, (double)w.m(1,5));
    EXPECT_NEAR(-2.153434682791282, (double)w.m(2,5), 1E-5);

    ASSERT_DOUBLE_EQ(2, (double)w.m(0,6));
    ASSERT_DOUBLE_EQ(2, (double)w.m(1,6));
    EXPECT_NEAR(-2.1534665123716255, (double)w.m(2,6), 1E-5);

    ASSERT_DOUBLE_EQ(3, (double)w.m(0,7));
    ASSERT_DOUBLE_EQ(2, (double)w.m(1,7));
    EXPECT_NEAR(-2.15349849459108, (double)w.m(2,7), 1E-5);

    ASSERT_DOUBLE_EQ(4, (double)w.m(0,8));
    ASSERT_DOUBLE_EQ(2, (double)w.m(1,8));
    EXPECT_NEAR(-2.1535302409459436, (double)w.m(2,8), 1E-5);
}

TEST_F(SimTest, can_generate_wave_height_on_mesh_for_default_wave_model)
{
    const Sim sys = get_system(test_data::stable_rolling_cube_test(), test_data::cube(), 0);
    ssc::kinematics::PointMatrix w = sys.get_waves(a.random<double>());
    ASSERT_EQ(200, w.m.cols());
    for (long i = 0 ; i < 200 ; ++i)
    {
        ASSERT_DOUBLE_EQ(0, (double)w.m(2,i));
    }
}

TEST_F(SimTest, LONG_waves_test_on_test_ship)
{
    const auto yaml = SimulatorYamlParser(test_data::test_ship_waves_test()).parse();
    const auto res = simulate<ssc::solver::RK4Stepper>(yaml, test_ship_stl, 0, 4.79, 0.479);
    ASSERT_EQ(11, res.size());
    ASSERT_NEAR(0, res[0].x[XIDX(0)], EPS);
    ASSERT_NEAR(0, res[0].x[YIDX(0)], EPS);
    ASSERT_NEAR(-5, res[0].x[ZIDX(0)], EPS);
}


TEST_F(SimTest, LONG_froude_krylov)
{
    const auto yaml = SimulatorYamlParser(test_data::test_ship_froude_krylov()).parse();
    const auto res = simulate<ssc::solver::RK4Stepper>(yaml, test_ship_stl, 0, 4.79, 0.479);
}

TEST_F(SimTest, LONG_test_ship_damping)
{
    const auto yaml = SimulatorYamlParser(test_data::test_ship_damping()).parse();
    const auto res = simulate<ssc::solver::RK4Stepper>(yaml, test_ship_stl, 0, 20, 1);
    ASSERT_EQ(21, res.size());
    ASSERT_FALSE(std::isnan(res.back().x[ZIDX(0)]));
    ASSERT_EQ(res.back().x[ZIDX(0)],res.back().x[ZIDX(0)]); // Check if nan
}

TEST_F(SimTest, DISABLED_LONG_bug_2655)
{
    const auto yaml = SimulatorYamlParser(test_data::bug_2655()).parse();
    const auto res = simulate<ssc::solver::RK4Stepper>(yaml, test_ship_stl, 0, 1.4, 0.1);
    ASSERT_EQ(15, res.size());
    ASSERT_NEAR(-0.228388, res.back().x[WIDX(0)], 1e-6);
}

TEST_F(SimTest, LONG_propulsion_and_resistance)
{
    const auto yaml = SimulatorYamlParser(test_data::propulsion_and_resistance()).parse();
    ssc::data_source::DataSource commands;
    commands.set<double>("propeller(rpm)", 100*(2*PI)/60.);
    commands.set<double>("propeller(P/D)", 1.064935);
    const size_t N = 250;
    const auto res = simulate<ssc::solver::EulerStepper>(yaml, test_ship_stl, 0, N, 1, commands);
    ASSERT_EQ(N+1, res.size());
    for (size_t i = 0 ; i <= N ; ++i)
    {
        const double t = res.at(i).t;
        ASSERT_LE(2.79963*(t-40), res.at(i).x[XIDX(0)]);
        ASSERT_DOUBLE_EQ(0, res.at(i).x[YIDX(0)]);
        ASSERT_DOUBLE_EQ(1, res.at(i).x[ZIDX(0)]);
        ASSERT_LE(2.79963*(1-exp(-0.012*t)), res.at(i).x[UIDX(0)]) << i;
        ASSERT_DOUBLE_EQ(0, res.at(i).x[VIDX(0)]);
        ASSERT_DOUBLE_EQ(0, res.at(i).x[WIDX(0)]);
        ASSERT_NEAR(-0.00109667*t, res.at(i).x[PIDX(0)], 8E-2);
        ASSERT_DOUBLE_EQ(0, res.at(i).x[QIDX(0)]) << "i = " << i;
        ASSERT_DOUBLE_EQ(0, res.at(i).x[RIDX(0)]) << "i = " << i;
    }
}

TEST_F(SimTest, DISABLED_LONG_bug_2641)
{
    const auto yaml = SimulatorYamlParser(test_data::bug_2641()).parse();
    ssc::data_source::DataSource commands;
    commands.set<double>("propeller(rpm)", 5*(2*PI));
    commands.set<double>("propeller(P/D)", 0.67);
    const auto res = simulate<ssc::solver::RK4Stepper>(yaml, test_ship_stl, 0, 5, 1, commands);
    ASSERT_LT(res.back().x[VIDX(0)], -0.003);
}

TEST_F(SimTest, bug_in_exact_hydrostatic)
{
    const auto yaml = SimulatorYamlParser(test_data::test_ship_exact_hydrostatic_test()).parse();
    const auto res = simulate<ssc::solver::RK4Stepper>(test_data::bug_in_exact_hydrostatic(), test_data::cube(), 0, 0.1, 0.4);
    for (auto r:res)
    {
        ASSERT_FALSE(std::isnan(r.x.front()));
    }
}

ssc::kinematics::EulerAngles convert(const double qr, const double qi, const double qj, const double qk);
ssc::kinematics::EulerAngles convert(const double qr, const double qi, const double qj, const double qk)
{
    using namespace ssc::kinematics;
    Eigen::Quaternion<double> quat(qr,qi,qj,qk);
    const RotationMatrix R = quat.toRotationMatrix();
    return ssc::kinematics::euler_angles<INTRINSIC, CHANGING_ANGLE_ORDER, 3, 2, 1>(R);
}

TEST_F(SimTest, rolling_cube_with_big_mesh)
{
    const auto res = simulate<ssc::solver::RK4Stepper>(test_data::new_oscillating_cube_example(), test_data::big_cube(), 0, 1, 0.1);
    std::vector<double> phi;
    for (auto r:res)
    {
        ASSERT_NEAR(0, r.x[XIDX(0)], 3E-15);
        ASSERT_NEAR(0, r.x[YIDX(0)], 3E-15);
        ASSERT_NEAR(0, r.x[ZIDX(0)], 3E-15);
        ASSERT_NEAR(0, r.x[UIDX(0)], 3E-15);
        ASSERT_NEAR(0, r.x[VIDX(0)], 3E-15);
        ASSERT_NEAR(0, r.x[WIDX(0)], 3E-15);
        ASSERT_NEAR(0, r.x[QIDX(0)], 3E-15);
        ASSERT_NEAR(0, r.x[RIDX(0)], 3E-15);
        const auto angle = convert(r.x[QRIDX(0)],r.x[QIIDX(0)],r.x[QJIDX(0)],r.x[QKIDX(0)]);
        ASSERT_NEAR(0, angle.theta, 2E-15);
        ASSERT_NEAR(0, angle.psi, 2E-15);
        phi.push_back(angle.phi);
    }
    ASSERT_EQ(11, phi.size());
    ASSERT_NEAR(0.43633231299858238339, phi[0],1e-6);
    ASSERT_NEAR(0.43768435305033482, phi[1],1e-6);
    ASSERT_NEAR(0.44174294754506388, phi[2],1e-6);
    ASSERT_NEAR(0.44851515262040759, phi[3],1e-6);
    ASSERT_NEAR(0.45801148069658759, phi[4],1e-6);
    ASSERT_NEAR(0.47024394765573296, phi[5],1e-6);
    ASSERT_NEAR(0.48522317379612806, phi[6],1e-6);
    ASSERT_NEAR(0.50295436708778729, phi[7],1e-6);
    ASSERT_NEAR(0.52343196026884675, phi[8],1e-6);
    ASSERT_NEAR(0.54663261284397258, phi[9],1e-6);
    ASSERT_NEAR(0.57250622838907383, phi[10],1e-6);
}

TEST_F(SimTest, LONG_bug_2714_heading_keeping)
{
    ssc::data_source::DataSource command_listener;
    command_listener.set<double>("controller(psi_co)", 0);
    const auto yaml = SimulatorYamlParser(test_data::bug_2714_heading_keeping()).parse();
    const auto res = simulate<ssc::solver::RK4Stepper>(yaml, test_ship_stl, 0, 0.1, 0.1, command_listener);
    ASSERT_EQ(2, res.size());
}

TEST_F(SimTest, LONG_bug_2714_station_keeping)
{
    ssc::data_source::DataSource command_listener;
    command_listener.set<double>("controller(x_co)", 0);
    command_listener.set<double>("controller(y_co)", 0);
    command_listener.set<double>("controller(psi_co)", 0);
    const auto yaml = SimulatorYamlParser(test_data::bug_2714_station_keeping()).parse();
    const auto res = simulate<ssc::solver::RK4Stepper>(yaml, test_ship_stl, 0, 0.1, 0.1, command_listener);
    ASSERT_EQ(2, res.size());
}

TEST_F(SimTest, LONG_bug_2732)
{
    const auto yaml = test_data::bug_2732();
    ListOfObservers observer(parse_output(yaml));
    ssc::data_source::DataSource command_listener;
    command_listener.set<double>("Prop. & rudder(rpm)", 50);
    command_listener.set<double>("Prop. & rudder(P/D)", 1);
    command_listener.set<double>("Prop. & rudder(beta)", 0);
    auto sys = get_system(yaml,test_ship_stl,0,command_listener);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, 0.1, 0.4, observer);
    ASSERT_NO_THROW(ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, 0.1, 0.4, observer));
}

TEST_F(SimTest, LONG_bug_2838)
{
    const auto yaml = test_data::bug_2838();
    ListOfObservers observer(parse_output(yaml));
    ssc::data_source::DataSource command_listener;
    command_listener.set<double>("PropRudd(rpm)", 50);
    command_listener.set<double>("PropRudd(P/D)", 1);
    command_listener.set<double>("PropRudd(beta)", 0);
    auto sys = get_system(yaml,test_ship_stl,0,command_listener);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, 0.4, 0.1, observer);
    const auto m = get_map(observer);
    ASSERT_EQ(2, m.size());
    const auto it = m.find("Mz(PropRudd,TestShip,TestShip)");
    ASSERT_NE(m.end(), it);
    ASSERT_EQ(5, it->second.size());
    ASSERT_GT(std::abs(it->second.back()), 0);
}

TEST_F(SimTest, LONG_bug_2845)
{
    const auto yaml = test_data::bug_2845();
    ListOfObservers observer(parse_output(yaml));
    ssc::data_source::DataSource command_listener;
    command_listener.set<double>("PropRudd(rpm)", 50);
    command_listener.set<double>("PropRudd(P/D)", 1);
    command_listener.set<double>("PropRudd(beta)", 0.8);
    auto sys = get_system(yaml,test_ship_stl,0,command_listener);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, 0.4, 0.1, observer);
    const auto m = get_map(observer);
    ASSERT_EQ(3, m.size());
    const auto it = m.find("Mz(PropRudd,TestShip,TestShip)");
    ASSERT_NE(m.end(), it);
    ASSERT_EQ(5, it->second.size());
    ASSERT_NEAR(0, it->second.back(), 1E-8);
}

TEST_F(SimTest, LONG_can_retrieve_maneuvering_force)
{
    const auto yaml = test_data::bug_2845();
    ListOfObservers observer(parse_output(yaml));
    ssc::data_source::DataSource command_listener;
    command_listener.set<double>("PropRudd(rpm)", 50);
    command_listener.set<double>("PropRudd(P/D)", 1);
    command_listener.set<double>("PropRudd(beta)", 0.8);
    auto sys = get_system(yaml,test_ship_stl,0,command_listener);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, 0.4, 0.1, observer);
    const auto m = get_map(observer);
    ASSERT_EQ(3, m.size());
    const auto it = m.find("Fx(Fman,TestShip,TestShip)");
    ASSERT_NE(m.end(), it);
    ASSERT_EQ(5, it->second.size());
}

TEST_F(SimTest, LONG_can_use_commands_in_maneuvering_model)
{
    const auto yaml = test_data::maneuvering_with_commands();
    ListOfObservers observer(parse_output(yaml));
    ssc::data_source::DataSource command_listener;
    command_listener.check_in(__PRETTY_FUNCTION__);
    command_listener.set<double>("PropRudd(rpm)", 50);
    command_listener.set<double>("PropRudd(P/D)", 1);
    command_listener.set<double>("PropRudd(beta)", 0.8);
    auto sys = get_system(yaml,test_ship_stl,0,command_listener);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, 0.4, 0.1, observer);
    command_listener.check_out();
    const auto m = get_map(observer);
    ASSERT_EQ(4, m.size());
    const auto it = m.find("Fx(F1,TestShip,TestShip)");
    ASSERT_NE(m.end(), it);
    ASSERT_EQ(5, it->second.size());
    ASSERT_NEAR(50, it->second.at(1), 1E-8);
}

TEST_F(SimTest, linear_hydrostatics_without_waves)
{
    const double T = 10;
    const double dt = 0.1;
    const auto res = simulate<ssc::solver::RK4Stepper>(test_data::test_ship_linear_hydrostatics_without_waves(), test_data::cube(), 0, T, dt);

    const double k = 100002.8;
    const double m = 253310;
    const double z0 = 1;
    const double w0 = 1;
    const double omega = sqrt(k/m);
    const double zeq = -0.099;

    auto z = [k,m,z0,w0,omega,zeq](const double t){return (z0-zeq)*cos(omega*t) + w0/omega*sin(omega*t) + zeq;};
    const double eps = 1E-5;
    for (size_t i = 0 ; i < res.size() ; ++i)
    {
        const double t = res.at(i).t;
        const double model = z(t);
        const double simulation = res.at(i).x[ZIDX(0)];
        ASSERT_NEAR(0, res.at(i).x[XIDX(0)], eps) << "i = " << i;
        ASSERT_NEAR(0, res.at(i).x[YIDX(0)], eps) << "i = " << i;
        ASSERT_NEAR(model, simulation, eps) << "i = " << i;
    }
}

TEST_F(SimTest, LONG_linear_hydrostatics_with_waves)
{
    const double T = 20;
    const double dt = 0.1;
    const auto res = simulate<ssc::solver::RK4Stepper>(test_data::test_ship_linear_hydrostatics_with_waves(), test_data::cube(), 0, T, dt);

    const double eps = 1E-3;
    EXPECT_NEAR(1      , res.at(0).x[ZIDX(0)], eps);
    EXPECT_NEAR(1.1010459072143728, res.at(1).x[ZIDX(0)], eps);
    EXPECT_NEAR(1.1990755183896542, res.at(2).x[ZIDX(0)], eps);
    EXPECT_NEAR(1.2864256014997444, res.at(3).x[ZIDX(0)], eps);
    EXPECT_NEAR(1.3558158151138187, res.at(4).x[ZIDX(0)], eps);
    EXPECT_NEAR(1.4008027467011974, res.at(5).x[ZIDX(0)], eps);
    EXPECT_NEAR(1.4161836722048577, res.at(6).x[ZIDX(0)], eps);
    EXPECT_NEAR(1.3983252434201292, res.at(7).x[ZIDX(0)], eps);
    EXPECT_NEAR(1.3453969093151239, res.at(8).x[ZIDX(0)], eps);
    EXPECT_NEAR(1.2574947155099978, res.at(9).x[ZIDX(0)], eps);
    EXPECT_NEAR(1.1366478447050186, res.at(10).x[ZIDX(0)], eps);
    EXPECT_NEAR(0.98670744792990239, res.at(11).x[ZIDX(0)], eps);
    EXPECT_NEAR(0.81312453136543006, res.at(12).x[ZIDX(0)], eps);
    EXPECT_NEAR(0.62263046374421704, res.at(13).x[ZIDX(0)], eps);
}

TEST_F(SimTest, LONG_can_simulate_radiation_damping)
{
    const double T = 12;
    const double dt = 0.2;
    std::ofstream of("test_ship.hdb");
    of << test_data::test_ship_hdb();
    ssc::data_source::DataSource command_listener;
    command_listener.check_in(__PRETTY_FUNCTION__);
    command_listener.set<double>("propeller(rpm)", 50);
    command_listener.set<double>("propeller(P/D)", 1);
    command_listener.set<double>("propeller(beta)", 0.8);
    command_listener.check_out();
    const auto res = simulate<ssc::solver::RK4Stepper>(test_data::test_ship_radiation_damping(), test_data::cube(), 0, T, dt, command_listener);
    ASSERT_NEAR(0.64349959510185351, res.at(5).x[XIDX(0)], 1E-3);
}

TEST_F(SimTest, bug_2963_should_not_be_able_to_use_fast_hydrostatic_without_specifying_wave_model)
{
    ASSERT_THROW(simulate<ssc::solver::RK4Stepper>(test_data::bug_2963_hs_fast(), test_data::cube(), 0, 1, 1), InvalidInputException);
}

TEST_F(SimTest, bug_2963_should_not_be_able_to_use_exact_hydrostatic_without_specifying_wave_model)
{
    ASSERT_THROW(simulate<ssc::solver::RK4Stepper>(test_data::bug_2963_hs_exact(), test_data::cube(), 0, 1, 1), InvalidInputException);
}

TEST_F(SimTest, bug_2963_should_not_be_able_to_use_froude_krylov_without_specifying_wave_model)
{
    ASSERT_THROW(simulate<ssc::solver::RK4Stepper>(test_data::bug_2963_fk(), test_data::cube(), 0, 1, 1), InvalidInputException);
}

TEST_F(SimTest, bug_2963_should_not_be_able_to_use_GM_without_specifying_wave_model)
{
    ASSERT_THROW(simulate<ssc::solver::RK4Stepper>(test_data::bug_2963_gm(), test_data::cube(), 0, 1, 1), InvalidInputException);
}

TEST_F(SimTest, bug_2963_should_not_be_able_to_use_diffraction_without_specifying_wave_model)
{
    ASSERT_THROW(simulate<ssc::solver::RK4Stepper>(test_data::bug_2963_diff(), test_data::cube(), 0, 1, 1), InvalidInputException);
}

TEST_F(SimTest, bug_3003_crash_in_manoeuvring_model)
{
    const double T = 0.1;
    const double dt = 0.1;
    const auto res = simulate<ssc::solver::RK4Stepper>(test_data::bug_3003(), 0, T, dt);

    const double eps = 1E-10;
    ASSERT_NEAR(-0.099, res.at(0).x[ZIDX(0)], eps);
}

TEST_F(SimTest, bug_2984)
{
    const auto yaml = test_data::bug_2984();
    auto input = SimulatorYamlParser(yaml).parse();
    boost::replace_all(input.bodies[0].controlled_forces[0].yaml
            ,"  phi:\n"
             "    unit: rad\n"
             "    value: 0\n"
             "  psi:\n"
             "    unit: deg\n"
             "    value: 0\n"
             "  theta:\n"
             "    unit: deg\n"
             "    value: 4.\n"
             "  x:\n"
             "    unit: m\n"
             "    value: -53.319\n"
             "  y:\n"
             "    unit: m\n"
             "    value: -3.750\n"
             "  z:\n"
             "    unit: m\n"
             "    value: 6.799\n"
            ,"  phi:\n"
             "    unit: rad\n"
             "    value: 0\n"
             "  psi:\n"
             "    unit: deg\n"
             "    value: 0\n"
             "  theta:\n"
             "    unit: deg\n"
             "    value: 4.\n"
             "  x:\n"
             "    unit: m\n"
             "    value: 0\n"
             "  y:\n"
             "    unit: m\n"
             "    value: 0\n"
             "  z:\n"
             "    unit: m\n"
             "    value: 0\n");

    ListOfObservers observer(parse_output(yaml));
    ssc::data_source::DataSource command_listener;
    command_listener.set<double>("propeller(rpm)", 120);
    command_listener.set<double>("propeller(P/D)", 1);
    command_listener.set<double>("propeller(beta)", 0);
    command_listener.set<double>("propeller(rpm)", 120);
    command_listener.set<double>("propeller(P/D)", 1);
    command_listener.set<double>("propeller(beta)", 0);

    auto sys = get_system(input,test_ship_stl,0,command_listener);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, 0.1, 0.1, observer);
    const auto m = get_map(observer);
    ASSERT_EQ(6, m.size());
    const auto Fx = m.find("Fx(propeller,ship,ship)");
    ASSERT_NE(m.end(), Fx);
    ASSERT_EQ(2, Fx->second.size());
    ASSERT_NEAR(69961607.31273420155048*cos(4*PI/180.), Fx->second.back(), 1E-6);
    const auto Fy = m.find("Fy(propeller,ship,ship)");
    ASSERT_NE(m.end(), Fy);
    ASSERT_EQ(2, Fy->second.size());
    ASSERT_NEAR(0, Fy->second.back(), 1E-8);
    const auto Fz = m.find("Fz(propeller,ship,ship)");
    ASSERT_NE(m.end(), Fz);
    ASSERT_EQ(2, Fz->second.size());
    ASSERT_NEAR(-sin(4*PI/180.)*69961607.31273420155048, Fz->second.back(), 1E-6);

    ASSERT_NEAR(4*PI/180., std::atan2(-Fz->second.back(),Fx->second.back()), 1E-6);
}

TEST_F(SimTest, bug_3217_hdb_interpolation_in_direction_incorrect_LONG)
{
    const double t0 = 0;
    const double T = 0.1;
    const double dt = 0.1;
    std::ofstream hdb("v00_complet_FK_diffr.hdb");
    hdb << test_data::bug_3210();
    std::ofstream stl("test_ship.stl");
    stl << test_data::cube();
    const auto yaml = test_data::bug_3217();
    ListOfObservers observers(parse_output(yaml));
    ssc::data_source::DataSource command_listener;
    auto input = SimulatorYamlParser(yaml).parse();

    const auto original_yaml = input.environment.at(0).yaml;

    // Results for 0.001 deg
    auto sys = get_system(input,test_ship_stl,0,command_listener);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, t0, T, dt, observers);
    auto m = get_map(observers);
    ASSERT_EQ(6, m.size());
    ASSERT_EQ(2, m["Fx(diffraction,TestShip,TestShip)"].size());
    const double Fx_001 = m["Fx(diffraction,TestShip,TestShip)"].back();
    const double Fy_001 = m["Fy(diffraction,TestShip,TestShip)"].back();
    const double Fz_001 = m["Fz(diffraction,TestShip,TestShip)"].back();
    const double Mx_001 = m["Mx(diffraction,TestShip,TestShip)"].back();
    const double My_001 = m["My(diffraction,TestShip,TestShip)"].back();
    const double Mz_001 = m["Mz(diffraction,TestShip,TestShip)"].back();

    // Results for 0 deg
    boost::replace_all(input.environment.at(0).yaml, "value: 0.001", "value: 0");
    sys = get_system(input,test_ship_stl,0,command_listener);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, t0, T, dt, observers);
    m = get_map(observers);

    ASSERT_EQ(6, m.size());
    ASSERT_EQ(4, m["Fx(diffraction,TestShip,TestShip)"].size());
    const double Fx_0 = m["Fx(diffraction,TestShip,TestShip)"].back();
    const double Fy_0 = m["Fy(diffraction,TestShip,TestShip)"].back();
    const double Fz_0 = m["Fz(diffraction,TestShip,TestShip)"].back();
    const double Mx_0 = m["Mx(diffraction,TestShip,TestShip)"].back();
    const double My_0 = m["My(diffraction,TestShip,TestShip)"].back();
    const double Mz_0 = m["Mz(diffraction,TestShip,TestShip)"].back();

    // Results for 30 deg
    input.environment.at(0).yaml = original_yaml;
    boost::replace_all(input.environment.at(0).yaml, "value: 0.001", "value: 30");
    sys = get_system(input,test_ship_stl,0,command_listener);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, t0, T, dt, observers);
    m = get_map(observers);
    ASSERT_EQ(6, m.size());
    ASSERT_EQ(6, m["Fx(diffraction,TestShip,TestShip)"].size());
    const double Fx_30 = m["Fx(diffraction,TestShip,TestShip)"].back();
    const double Fy_30 = m["Fy(diffraction,TestShip,TestShip)"].back();
    const double Fz_30 = m["Fz(diffraction,TestShip,TestShip)"].back();
    const double Mx_30 = m["Mx(diffraction,TestShip,TestShip)"].back();
    const double My_30 = m["My(diffraction,TestShip,TestShip)"].back();
    const double Mz_30 = m["Mz(diffraction,TestShip,TestShip)"].back();

    // Results at 0.001 deg should be closer to results at 0 deg than to results at 30 deg
    ASSERT_LT(std::abs(Fx_0-Fx_001),std::abs(Fx_30-Fx_001));
    ASSERT_LT(std::abs(Fy_0-Fy_001),std::abs(Fy_30-Fy_001));
    ASSERT_LT(std::abs(Fz_0-Fz_001),std::abs(Fz_30-Fz_001));

    ASSERT_LT(std::abs(Mx_0-Mx_001),std::abs(Mx_30-Mx_001));
    ASSERT_LT(std::abs(My_0-My_001),std::abs(My_30-My_001));
    ASSERT_LT(std::abs(Mz_0-Mz_001),std::abs(Mz_30-Mz_001));
}


TEST_F(SimTest, bug_3227_wave_angle_mirroing_problem_for_diffraction_LONG)
{
    const double t0 = 0;
    const double T = 0.1;
    const double dt = 0.1;
    std::ofstream hdb("v00_complet_FK_diffr.hdb");
    hdb << test_data::bug_3210();
    std::ofstream stl("test_ship.stl");
    stl << test_data::cube();
    const auto yaml = test_data::bug_3227();

    ListOfObservers observers(parse_output(yaml));
    ssc::data_source::DataSource command_listener;
    auto input = SimulatorYamlParser(yaml).parse();

    const auto original_yaml = input.environment.at(0).yaml;

    // Results for 30 deg
    auto sys = get_system(input,test_ship_stl,0,command_listener);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, t0, T, dt, observers);
    auto m = get_map(observers);
    ASSERT_EQ(6, m.size());
    ASSERT_EQ(2, m["Fx(diffraction,TestShip,TestShip)"].size());
    const double Fx_for_waves_propagating_to_plus_30 = m["Fx(diffraction,TestShip,TestShip)"].back();
    const double Fy_for_waves_propagating_to_plus_30 = m["Fy(diffraction,TestShip,TestShip)"].back();
    const double Fz_for_waves_propagating_to_plus_30 = m["Fz(diffraction,TestShip,TestShip)"].back();
    const double Mx_for_waves_propagating_to_plus_30 = m["Mx(diffraction,TestShip,TestShip)"].back();
    const double My_for_waves_propagating_to_plus_30 = m["My(diffraction,TestShip,TestShip)"].back();
    const double Mz_for_waves_propagating_to_plus_30 = m["Mz(diffraction,TestShip,TestShip)"].back();

    // Results for -30 deg
    boost::replace_all(input.environment.at(0).yaml, "value: 30", "value: -30");
    sys = get_system(input,test_ship_stl,0,command_listener);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, t0, T, dt, observers);
    m = get_map(observers);

    ASSERT_EQ(6, m.size());
    ASSERT_EQ(4, m["Fx(diffraction,TestShip,TestShip)"].size());
    const double Fx_for_waves_propagating_to_minus_30 = m["Fx(diffraction,TestShip,TestShip)"].back();
    const double Fy_for_waves_propagating_to_minus_30 = m["Fy(diffraction,TestShip,TestShip)"].back();
    const double Fz_for_waves_propagating_to_minus_30 = m["Fz(diffraction,TestShip,TestShip)"].back();
    const double Mx_for_waves_propagating_to_minus_30 = m["Mx(diffraction,TestShip,TestShip)"].back();
    const double My_for_waves_propagating_to_minus_30 = m["My(diffraction,TestShip,TestShip)"].back();
    const double Mz_for_waves_propagating_to_minus_30 = m["Mz(diffraction,TestShip,TestShip)"].back();

    // No sign change for Fx, Fz & My when switching from waves propagating to +30 to waves propagating to -30
    ASSERT_GE(Fx_for_waves_propagating_to_plus_30*Fx_for_waves_propagating_to_minus_30,0);
    ASSERT_GE(Fz_for_waves_propagating_to_plus_30*Fz_for_waves_propagating_to_minus_30,0);
    ASSERT_GE(My_for_waves_propagating_to_plus_30*My_for_waves_propagating_to_minus_30,0);

    // Sign change for Fx, Fz & My when switching from waves propagating to +30 to waves propagating to -30
    ASSERT_LE(Fy_for_waves_propagating_to_plus_30*Fy_for_waves_propagating_to_minus_30,0);
    ASSERT_LE(Mx_for_waves_propagating_to_plus_30*Mx_for_waves_propagating_to_minus_30,0);
    ASSERT_LE(Mz_for_waves_propagating_to_plus_30*Mz_for_waves_propagating_to_minus_30,0);
}

TEST_F(SimTest, bug_3230_advance_speed_not_taken_into_account_properly_for_diffraction_forces_LONG)
{
    const double t0 = 0;
    const double T = 0.1;
    const double dt = 0.1;
    std::ofstream hdb("ship_2008.HDB");
    hdb << test_data::bug_3230_hdb();
    std::ofstream stl("ship_2008.stl");
    stl << test_data::cube();
    const auto yaml = test_data::yml_bug_3230();

    ListOfObservers observers(parse_output(yaml));
    ssc::data_source::DataSource command_listener;
    auto input = SimulatorYamlParser(yaml).parse();


    auto sys = get_system(input,test_ship_stl,0,command_listener);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, t0, T, dt, observers);
    auto m = get_map(observers);
    ASSERT_EQ(6, m.size());
    ASSERT_EQ(2, m["Fx(diffraction,ship,ship)"].size());
    ASSERT_DOUBLE_EQ(53167.137779674224, m["Fx(diffraction,ship,ship)"].at(1));
}

TEST_F(SimTest, bug_3207_radiation_damping_crashes_LONG)
{
    std::ofstream hdb("bug_3207.hdb");
    hdb << test_data::bug_3207_hdb();
    auto input = SimulatorYamlParser(test_data::bug_3207_yml()).parse();
    const double t0 = 0;
    const double T = 15;
    const double dt = 0.2;
    const auto res = simulate<ssc::solver::RK4Stepper>(input, test_ship_stl, t0, T, dt);
    ASSERT_EQ(76, res.size());
}

TEST_F(SimTest, bug_3241_blocked_dof_interpolation_problem_LONG)
{
    const double t0 = 0;
    const double T = 15;
    const double dt = 0.1;
    const auto yaml = test_data::bug_3241();
    ListOfObservers observers(parse_output(yaml));
    auto input = SimulatorYamlParser(yaml).parse();

    auto sys = get_system(input,test_ship_stl,0);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, t0, T, dt, observers);
    auto m = get_map(observers);
    ASSERT_EQ(1, m.size());
    ASSERT_EQ(151, m["u(dtmb)"].size());
    ASSERT_NE(m["u(dtmb)"].at(0),m["u(dtmb)"].at(150));
    ASSERT_DOUBLE_EQ(1.531,m["u(dtmb)"].at(0));
    ASSERT_DOUBLE_EQ(1,m["u(dtmb)"].at(50));
    ASSERT_DOUBLE_EQ(1.5,m["u(dtmb)"].at(100));
    ASSERT_DOUBLE_EQ(0,m["u(dtmb)"].at(150));
}

TEST_F(SimTest, issue_20_constant_force)
{
    const double t0 = 0;
    const double T = 15;
    const double dt = 0.1;
    const auto yaml = test_data::issue_20();
    ListOfObservers observers(parse_output(yaml));
    auto input = SimulatorYamlParser(yaml).parse();

    auto sys = get_system(input,test_ship_stl,0);
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, t0, T, dt, observers);
}
