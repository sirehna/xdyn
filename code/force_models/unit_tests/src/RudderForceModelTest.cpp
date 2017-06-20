/*
 * RudderForceModelTest.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define DEG2RAD (PI/180.)

#include "Airy.hpp"
#include "BodyWithoutSurfaceForces.hpp"
#include "DiscreteDirectionalWaveSpectrum.hpp"
#include "DiracSpectralDensity.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "discretize.hpp"
#include "env_for_tests.hpp"
#include "RudderForceModelTest.hpp"
#include "RudderForceModel.hpp"
#include "yaml_data.hpp"
#include "YamlWaveModelInput.hpp"
#include "Stretching.hpp"

namespace ssc
{
    namespace random_data_generator
    {
        template <> RudderForceModel::Yaml TypedScalarDataGenerator<RudderForceModel::Yaml>::get() const
        {
            RudderForceModel::Yaml ret;
            ret.Ar = random<double>().between(0,1e5);
            ret.b = random<double>();
            ret.blade_area_ratio = random<double>();
            ret.diameter = random<double>();
            ret.drag_coeff = random<double>();
            ret.effective_aspect_ratio_factor = random<double>();
            ret.lift_coeff = random<double>();
            return ret;
        }
    }
}

RudderForceModelTest::RudderForceModelTest() : a(ssc::random_data_generator::DataGenerator(21212))
{
}

RudderForceModelTest::~RudderForceModelTest()
{
}

void RudderForceModelTest::SetUp()
{
}

void RudderForceModelTest::TearDown()
{
}

TEST_F(RudderForceModelTest, angle_of_attack)
{
//! [RudderForceModelTest get_alpha_example]
    RudderForceModel::RudderModel riw(a.random<RudderForceModel::Yaml>(),a.random<double>(),a.random<double>());
//! [RudderForceModelTest get_alpha_example]
//! [RudderForceModelTest get_alpha_example output]
    ASSERT_DOUBLE_EQ(-1,riw.get_angle_of_attack(1, 2));
    ASSERT_DOUBLE_EQ(1,riw.get_angle_of_attack(2, 1));
//! [RudderForceModelTest get_alpha_example output]
}

TEST_F(RudderForceModelTest, get_Cd)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.Ar = 3;
    parameters.b = 4;
    parameters.effective_aspect_ratio_factor = 0.5;
    RudderForceModel::RudderModel riw(parameters,1024,2);

    const double Vs = 6;
    const double Cl = 7;
    ASSERT_DOUBLE_EQ(6.5028916553907399, riw.get_Cd(Vs, Cl));
}

TEST_F(RudderForceModelTest, get_Cl)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.Ar = 3;
    parameters.b = 4;
    parameters.effective_aspect_ratio_factor = 0.5;
    RudderForceModel::RudderModel riw(parameters,1024,2);
    const double alpha = 0.4;
    ASSERT_DOUBLE_EQ(1.0985577009852809, riw.get_Cl(alpha));
}

TEST_F(RudderForceModelTest, get_lift)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.Ar = 10;
    parameters.lift_coeff = 2;
    RudderForceModel::RudderModel riw(parameters,1024,a.random<double>());

    const double Vs = 12;
    const double Cl = 1.3;
    const double alpha = PI/4;
    const double Ar = 1.4;
    ASSERT_DOUBLE_EQ(189766.19029849127, riw.get_lift(Vs,Cl,alpha,Ar));
}

TEST_F(RudderForceModelTest, get_drag)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.Ar = 10;
    parameters.drag_coeff = 2;
    RudderForceModel::RudderModel riw(parameters,1024,a.random<double>());

    const double Vs = 12;
    const double Cl = 1.3;
    const double alpha = PI/4;
    const double Ar = 1.4;
    ASSERT_DOUBLE_EQ(268369.91999999998, riw.get_drag(Vs,Cl,alpha,Ar));
}

TEST_F(RudderForceModelTest, get_force)
{
//! [RudderForceModelTest get_alpha_example]
    RudderForceModel::RudderModel riw(a.random<RudderForceModel::Yaml>(),a.random<double>(),a.random<double>());
    const double drag = 10;
    const double lift = 200;
    const double angle = -PI/3;
    ssc::kinematics::Vector6d v = riw.get_force(drag, lift, angle);
    ASSERT_DOUBLE_EQ(-91.339745962155646, (double)v(0));
    ASSERT_DOUBLE_EQ(178.20508075688772, (double)v(1));
    ASSERT_DOUBLE_EQ(0, (double)v(2));
    ASSERT_DOUBLE_EQ(0, (double)v(3));
    ASSERT_DOUBLE_EQ(0, (double)v(4));
    ASSERT_DOUBLE_EQ(0, (double)v(5));
}

TEST_F(RudderForceModelTest, get_wrench)
{
//! [RudderForceModelTest get_alpha_example]
    RudderForceModel::Yaml parameters;
    parameters.Ar = 10;
    parameters.drag_coeff = 2;
    parameters.lift_coeff = 5;
    parameters.b = 99;
    parameters.effective_aspect_ratio_factor = 2.3;
    RudderForceModel::RudderModel riw(parameters,1024,0.75);
    const double drag = 10;
    const double lift = 200;
    const double angle = -PI/3;
    const double area = 1.467;
    ssc::kinematics::Vector6d v = riw.get_wrench(3,4,0.5,area);
    ASSERT_DOUBLE_EQ(-2021.4412785509464, (double)v(0));
    ASSERT_DOUBLE_EQ(1757.2988992064641, (double)v(1));
    ASSERT_DOUBLE_EQ(0, (double)v(2));
    ASSERT_DOUBLE_EQ(0, (double)v(3));
    ASSERT_DOUBLE_EQ(0, (double)v(4));
    ASSERT_DOUBLE_EQ(0, (double)v(5));
}

TEST_F(RudderForceModelTest, get_Ar)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    RudderForceModel::RudderModel riw(parameters,a.random<double>(),a.random<double>());
    const double CTh = a.random<double>().between(-1,1000);
    const auto ar = riw.get_Ar(CTh);
    //ar.outside_wake = parameters.Ar-ar.in_wake;
    ASSERT_SMALL_RELATIVE_ERROR(parameters.Ar, ar.in_wake+ar.outside_wake, 1E-10);
}

TEST_F(RudderForceModelTest, get_Ar2)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.Ar = 10;
    parameters.b = 4;
    parameters.position_of_propeller_frame.coordinates.x = 0;
    parameters.position_of_propeller_frame.coordinates.y = 0;
    parameters.position_of_propeller_frame.coordinates.z = 0;
    parameters.position_of_the_rudder_frame_in_the_body_frame.x = 2.47;
    parameters.position_of_the_rudder_frame_in_the_body_frame.y = 0;
    parameters.position_of_the_rudder_frame_in_the_body_frame.z = 0;
    parameters.diameter = 1.67;
    const double CTh = 5.3;
    RudderForceModel::RudderModel riw(parameters,a.random<double>(),a.random<double>());
    const auto ar = riw.get_Ar(CTh);
    ASSERT_DOUBLE_EQ(3.5404447215261827, ar.in_wake);
    ASSERT_DOUBLE_EQ(6.4595552784738173, ar.outside_wake);
}

TEST_F(RudderForceModelTest, get_Vs)
{
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.diameter = 3.6;
    RudderForceModel::RudderModel riw(parameters,1024,a.random<double>());
    const auto vs = riw.get_vs(1.5,12,6,12e4);
    ASSERT_DOUBLE_EQ(12.007932248435861, (double)vs.in_wake.v.norm());
    ASSERT_DOUBLE_EQ(13.416407864998739, (double)vs.outside_wake.v.norm());
}

TEST_F(RudderForceModelTest, get_fluid_angle)
{
    const RudderForceModel::RudderModel riw(a.random<RudderForceModel::Yaml>(),a.random<double>(),a.random<double>());
    RudderForceModel::InOutWake<ssc::kinematics::Point> V;
    V.in_wake.x() = 1;
    V.in_wake.y() = 2;
    V.in_wake.z() = a.random<double>();
    V.outside_wake.x() = -4;
    V.outside_wake.y() = -4;
    V.outside_wake.z() = a.random<double>();
    const auto vs = riw.get_fluid_angle(V);
    ASSERT_DOUBLE_EQ(1.1071487177940904, vs.in_wake);
    ASSERT_DOUBLE_EQ(-3*PI/4, vs.outside_wake);
}

TR1(shared_ptr)<WaveModel> RudderForceModelTest::get_wave_model() const
{
    const double Hs = 0.1;
    const double Tp = 5;
    const double omega0 = 2*PI/Tp;
    double psi = 0;
    double phi = 5.8268;
    double t = 0;

    const double omega_min = a.random<double>().greater_than(0);
    const double omega_max = a.random<double>().greater_than(omega_min);
    const size_t nfreq = a.random<size_t>().between(2,100);
    YamlStretching ys;
    ys.h = 0;
    ys.delta = 1;
    const Stretching ss(ys);
    const DiscreteDirectionalWaveSpectrum A = discretize(DiracSpectralDensity(omega0, Hs), DiracDirectionalSpreading(psi), omega_min, omega_max, nfreq, ss);

    return TR1(shared_ptr)<WaveModel>(new Airy(A, phi));
}

TEST_F(RudderForceModelTest, DISABLED_ship_speed_relative_to_the_fluid)
{
    EnvironmentAndFrames env = get_environment_and_frames(get_wave_model());
    RudderForceModel::Yaml parameters = a.random<RudderForceModel::Yaml>();
    parameters.number_of_blades = 3;
    parameters.blade_area_ratio = 0.5;
    parameters.position_of_propeller_frame.frame = "body";
    const RudderForceModel F(parameters, a.random<std::string>(), env);
    BodyStates states;
    std::vector<double> s = {1,2,3,4,5,6,0,0,0,1,0,0,0};
    const double t = 24;
    states.u.record(t, s[2]);
    states.v.record(t, s[3]);
    states.w.record(t, s[4]);
    states.name = "body";
    BodyWithoutSurfaceForces b(states,0,BlockedDOF(""));
    b.update_kinematics(s, env.k);

    ssc::kinematics::Point Vship_water = F.get_ship_speed(states, t);

    ASSERT_DOUBLE_EQ(3.0621974344648351, Vship_water.x());
    ASSERT_DOUBLE_EQ(4, Vship_water.y());
    ASSERT_DOUBLE_EQ(5.0089062285187547, Vship_water.z());
}

TEST_F(RudderForceModelTest, parser)
{
    const auto w = RudderForceModel::parse(test_data::rudder());
    ASSERT_DOUBLE_EQ(0.5, w.blade_area_ratio);
    ASSERT_EQ("port side propeller", w.name);
    ASSERT_EQ(3, w.number_of_blades);
    ASSERT_DOUBLE_EQ(0, w.position_of_propeller_frame.angle.phi);
    ASSERT_DOUBLE_EQ(-10*DEG2RAD, w.position_of_propeller_frame.angle.theta);
    ASSERT_DOUBLE_EQ(-1*DEG2RAD, w.position_of_propeller_frame.angle.psi);
    ASSERT_DOUBLE_EQ(-4, w.position_of_propeller_frame.coordinates.x);
    ASSERT_DOUBLE_EQ(-2, w.position_of_propeller_frame.coordinates.y);
    ASSERT_DOUBLE_EQ(2, w.position_of_propeller_frame.coordinates.z);
    ASSERT_EQ("mesh(body 1)", w.position_of_propeller_frame.frame);
    ASSERT_DOUBLE_EQ(1, w.relative_rotative_efficiency);
    ASSERT_TRUE(w.rotating_clockwise);
    ASSERT_DOUBLE_EQ(0.7, w.thrust_deduction_factor);
    ASSERT_DOUBLE_EQ(0.9, w.wake_coefficient);
    ASSERT_DOUBLE_EQ(2, w.diameter);
    ASSERT_DOUBLE_EQ(2.2, w.Ar);
    ASSERT_DOUBLE_EQ(2,w.b);
    ASSERT_DOUBLE_EQ(1,w.drag_coeff);
    ASSERT_DOUBLE_EQ(2.1,w.lift_coeff);
    ASSERT_DOUBLE_EQ(1.7,w.effective_aspect_ratio_factor);
    ASSERT_DOUBLE_EQ(-5.1, w.position_of_the_rudder_frame_in_the_body_frame.x);
    ASSERT_DOUBLE_EQ(-2, w.position_of_the_rudder_frame_in_the_body_frame.y);
    ASSERT_DOUBLE_EQ(2, w.position_of_the_rudder_frame_in_the_body_frame.z);
}

TEST_F(RudderForceModelTest, force_and_torque)
{
    EnvironmentAndFrames env = get_environment_and_frames(get_wave_model());
    const RudderForceModel rudder(RudderForceModel::parse(test_data::rudder()), a.random<std::string>(), env);
    ASSERT_EQ("propeller+rudder", rudder.model_name());
    BodyStates states;
    std::vector<double> s = {1,2,3,4,5,6,0,0,0,1,0,0,0};
    const double t = 24;
    states.u.record(t, s[2]);
    states.v.record(t, s[3]);
    states.w.record(t, s[4]);
    states.name = "body";
    BodyWithoutSurfaceForces b(states,0,BlockedDOF(""));
    b.update_kinematics(s, env.k);
    std::map<std::string,double> commands;
    commands["rpm"] = 200;
    commands["P/D"] = 1.2;
    commands["beta"] = PI/6;

    const auto F = rudder.get_force(states, t, commands);
    ASSERT_DOUBLE_EQ(2208573.9553180891, (double)F(0));
    ASSERT_DOUBLE_EQ(777997.67996840423, (double)F(1));
    ASSERT_DOUBLE_EQ(0, (double)F(2));
    ASSERT_DOUBLE_EQ(-2793416.1021430148, (double)F(3));
    ASSERT_DOUBLE_EQ(0, (double)F(4));
    ASSERT_DOUBLE_EQ(-855797.44796524453, (double)F(5));
}
