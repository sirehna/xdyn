/*
 * ManeuveringForceModelTest.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: cady
 */
#include "gmock/gmock.h"

#include "EnvironmentAndFrames.hpp"
#include "generate_body_for_tests.hpp"
#include "ManeuveringForceModelTest.hpp"
#include "ManeuveringForceModel.hpp"
#include "ManeuveringInternal.hpp"
#include "yaml_data.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

using namespace maneuvering;
using namespace testing; // So we can use 'ElementsAre' unqualified

ManeuveringForceModelTest::ManeuveringForceModelTest() : a(ssc::random_data_generator::DataGenerator(87542))
{
}

ManeuveringForceModelTest::~ManeuveringForceModelTest()
{
}

void ManeuveringForceModelTest::SetUp()
{
}

void ManeuveringForceModelTest::TearDown()
{
}

TEST_F(ManeuveringForceModelTest, can_parse_frame_of_reference)
{
//! [ManeuveringForceModelTest example]
    const auto data = ManeuveringForceModel::parse(test_data::maneuvering());
//! [ManeuveringForceModelTest example]
//! [ManeuveringForceModelTest expected output]
    ASSERT_DOUBLE_EQ(0.696,         data.frame_of_reference.coordinates.x);
    ASSERT_DOUBLE_EQ(0,             data.frame_of_reference.coordinates.y);
    ASSERT_DOUBLE_EQ(1.418,         data.frame_of_reference.coordinates.z);
    ASSERT_DOUBLE_EQ(0.7,           data.frame_of_reference.angle.phi);
    ASSERT_DOUBLE_EQ(-166.*PI/180., data.frame_of_reference.angle.theta);
    ASSERT_DOUBLE_EQ(125.*PI/180.,  data.frame_of_reference.angle.psi);
//! [ManeuveringForceModelTest expected output]
}

TEST_F(ManeuveringForceModelTest, can_parse_X_Y_Z_K_M_N)
{
    auto data = ManeuveringForceModel::parse(test_data::maneuvering());
    ASSERT_EQ("0.5*rho*Vs^2*L^2*X_", data.var2expr["X"]);
    ASSERT_EQ("0.5*rho*Vs^2*L^2*Y_", data.var2expr["Y"]);
    ASSERT_EQ("0", data.var2expr["Z"]);
    ASSERT_EQ("0", data.var2expr["K"]);
    ASSERT_EQ("0", data.var2expr["M"]);
    ASSERT_EQ("0.5*rho*Vs^2*L^3*N_", data.var2expr["N"]);
}

TEST_F(ManeuveringForceModelTest, can_parse_model_name)
{
    auto data = ManeuveringForceModel::parse(test_data::maneuvering());
    ASSERT_EQ("test", data.name);
}

TEST_F(ManeuveringForceModelTest, can_parse_commands)
{
    auto data = ManeuveringForceModel::parse(test_data::maneuvering());
    ASSERT_THAT(data.commands, ElementsAre("a","b","c"));
}

TEST_F(ManeuveringForceModelTest, internal_constant)
{
    const auto c = make_constant(4);
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_DOUBLE_EQ(4, f(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, internal_cos)
{
    const auto c = make_cos(make_constant(PI/3.));
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_DOUBLE_EQ(0.5, f(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, internal_abs)
{
    const auto c = make_abs(make_cos(make_constant(PI)));
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_DOUBLE_EQ(1, f(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, internal_sin)
{
    const auto c = make_sin(make_constant(PI/6.));
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_DOUBLE_EQ(0.5, f(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, internal_log)
{
    const auto c = make_log(make_constant(exp(1)));
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_DOUBLE_EQ(1, f(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, internal_sqrt)
{
    const auto c = make_sqrt(make_constant(4));
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_DOUBLE_EQ(2, f(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, internal_sum)
{
    const auto c = make_sum(make_cos(make_constant(PI/3.)),make_sin(make_constant(PI/6.)));
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_DOUBLE_EQ(1, f(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, internal_pow)
{
    const double theta = a.random<double>().between(-PI,PI);
    const auto c = make_sum(make_pow(make_cos(make_constant(theta)), make_constant(2)),
                            make_pow(make_sin(make_constant(theta)), make_constant(2)));
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_DOUBLE_EQ(1, f(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, internal_exp)
{
    const double x = a.random<double>().between(-10,10);
    const auto c = make_log(make_exp(make_constant(x)));
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_NEAR(x, f(states, ds, t),1e-12);
}

TEST_F(ManeuveringForceModelTest, internal_difference)
{
    const double x = a.random<double>().between(-10,10);
    const double y = a.random<double>().between(-10,10);
    const auto c = make_difference(make_constant(x),make_constant(y));
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_DOUBLE_EQ(x-y, f(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, internal_divide)
{
    const double x = a.random<double>().between(-10,10);
    const double y = a.random<double>().between(10,20);
    const auto c = make_divide(make_constant(x),make_constant(y));
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_DOUBLE_EQ(x/y, f(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, internal_multiply)
{
    const double x = a.random<double>().between(-10,10);
    const double y = a.random<double>().between(10,20);
    const auto c = make_multiply(make_constant(x),make_constant(y));
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_DOUBLE_EQ(x*y, f(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, internal_state)
{
    const auto xx = make_state_x(make_constant(5),YamlRotation());
    const auto yy = make_state_y(make_constant(5),YamlRotation());
    const auto zz = make_state_z(make_constant(5),YamlRotation());
    const auto uu = make_state_u(make_constant(5),YamlRotation());
    const auto vv = make_state_v(make_constant(5),YamlRotation());
    const auto ww = make_state_w(make_constant(5),YamlRotation());
    const auto pp = make_state_p(make_constant(5),YamlRotation());
    const auto qq = make_state_q(make_constant(5),YamlRotation());
    const auto rr = make_state_r(make_constant(5),YamlRotation());
    const auto x = xx->get_lambda();
    const auto y = yy->get_lambda();
    const auto z = zz->get_lambda();
    const auto u = uu->get_lambda();
    const auto v = vv->get_lambda();
    const auto w = ww->get_lambda();
    const auto p = pp->get_lambda();
    const auto q = qq->get_lambda();
    const auto r = rr->get_lambda();
    BodyStates states(10);
    states.x.record(0, 1);
    states.y.record(0, 2);
    states.z.record(0, 3);
    states.u.record(0, 4);
    states.v.record(0, 5);
    states.w.record(0, 6);
    states.p.record(0, 7);
    states.q.record(0, 8);
    states.r.record(0, 9);
    states.x.record(10, 2*1);
    states.y.record(10, 2*2);
    states.z.record(10, 2*3);
    states.u.record(10, 2*4);
    states.v.record(10, 2*5);
    states.w.record(10, 2*6);
    states.p.record(10, 2*7);
    states.q.record(10, 2*8);
    states.r.record(10, 2*9);
    ssc::data_source::DataSource ds;
    const double t = 10;
    ASSERT_DOUBLE_EQ(1.5*1, x(states, ds, t));
    ASSERT_DOUBLE_EQ(1.5*2, y(states, ds, t));
    ASSERT_DOUBLE_EQ(1.5*3, z(states, ds, t));
    ASSERT_DOUBLE_EQ(1.5*4, u(states, ds, t));
    ASSERT_DOUBLE_EQ(1.5*5, v(states, ds, t));
    ASSERT_DOUBLE_EQ(1.5*6, w(states, ds, t));
    ASSERT_DOUBLE_EQ(1.5*7, p(states, ds, t));
    ASSERT_DOUBLE_EQ(1.5*8, q(states, ds, t));
    ASSERT_DOUBLE_EQ(1.5*9, r(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, internal_time)
{
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    const auto c = make_time();
    const auto f = c->get_lambda();
    ASSERT_DOUBLE_EQ(t, f(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, unknown_identifier)
{
    BodyStates states;
    ssc::data_source::DataSource ds;
    ds.set<double>("foo", 123.456);
    const double t = a.random<double>();
    const auto c = make_unknown_identifier("foo");
    const auto f = c->get_lambda();
    ASSERT_DOUBLE_EQ(123.456, f(states, ds, t));
}

EnvironmentAndFrames get_env_with_default_rotation_convention();
EnvironmentAndFrames get_env_with_default_rotation_convention()
{
    EnvironmentAndFrames env;
    env.rot.convention = {"z", "y'", "x''"};
    env.rot.order_by = "angle";
    env.rho = 1024;
    return env;
}

TEST_F(ManeuveringForceModelTest, can_evaluate_simple_maneuvering_model)
{
    const std::string yaml = "reference frame:\n"
                             "    frame: some body\n"
                             "    x: {value: 0, unit: m}\n"
                             "    y: {value: 0, unit: m}\n"
                             "    z: {value: 0, unit: m}\n"
                             "    phi: {value: 0, unit: rad}\n"
                             "    theta: {value: 0, unit: deg}\n"
                             "    psi: {value: 0, unit: deg}\n"
                             "name: something\n"
                             "X: 2*Y+sqrt(x(t))\n"
                             "Y: y(t)^2\n"
                             "Z: 0\n"
                             "K: 0\n"
                             "M: 0\n"
                             "N: 0\n";
    const auto env = get_env_with_default_rotation_convention();
    ManeuveringForceModel force(ManeuveringForceModel::parse(yaml),"some body", env);
    ASSERT_EQ("maneuvering", force.model_name());
    auto states = get_body("some body")->get_states();
    const double t = 10;
    states.x.record(t, 1024);
    states.y.record(t, 400);
    ssc::data_source::DataSource command_listener;
    const auto F = force(states, t, command_listener, env.k, states.G);

    ASSERT_EQ("some body", F.get_frame());
    ASSERT_DOUBLE_EQ(320032, F.X());
    ASSERT_DOUBLE_EQ(160000, F.Y());
    ASSERT_DOUBLE_EQ(0, F.Z());
    ASSERT_NEAR(0, F.K(), 1e-15);
    ASSERT_NEAR(0, F.M(), 1e-15);
    ASSERT_NEAR(0, F.N(), 1e-15);
}


/*  The following test data was generated using this Python code:

from math import fabs,sqrt
def man(x,y,z,u,v,w,p,q,r):
       Xu= 0
       Xuu= 0
       Xuuu= 0
       Xvv= -0.041
       Xrr= -0.01
       Xvr= -0.015
       Yv= -0.13
       Yvv= -0.18
       Yvvv= 0
       Yvrr= 0
       Yr= 0.015
       Yrr= 0.021
       Yrrr= 0
       Yrvv= 0
       Nv= -0.37
       Nvv= -0.12
       Nvvv= 0
       Nvrr= 0
       Nr= -0.1
       Nrr= 0.005
       Nrrr= 0
       Nrvv= 0
       rho= 1024
       Vs= sqrt(u**2+v**2)
       L= 21.569
       u_= u/Vs
       v_= v/Vs
       r_= r/Vs*L
       X_= Xu*u_ + Xuu*u_**2 + Xuuu*u_**3 + Xvv*v_**2 + Xrr*r_**2 + Xvr*fabs(v_)*fabs(r_)
       Y_= Yv*v_ + Yvv*v_*fabs(v_) + Yvvv*v_**3 + Yvrr*v_*r_**2 + Yr*r_ + Yrr*r_*fabs(r_) + Yrrr*r_**3 + Yrvv*r_*v_**2
       N_= Nv*v_ + Nvv*v_*fabs(v_) + Nvvv*v_**3 + Nvrr*v_*r_**2 + Nr*r_ + Nrr*r_*fabs(r_) + Nrrr*r_**3 + Nrvv*r_*v_**2
       X= 0.5*rho*Vs**2*L**2*X_
       Y= 0.5*rho*Vs**2*L**2*Y_
       Z= 0
       K= 0
       M= 0
       N= 0.5*rho*Vs**2*L**3*N_
       return {'X': X,\
               'Y': Y,\
               'Z': Z,\
               'K': K,\
               'M': M,\
               'N': N
              }

man(1,2,3,4,5,6,7,8,9)
man(0.1,2.04,6.28,0.45,0.01,5.869,0.23,0,0.38)
 */

TEST_F(ManeuveringForceModelTest, can_evaluate_full_maneuvering_model_with_same_frame_of_reference)
{
    auto data = ManeuveringForceModel::parse(test_data::maneuvering_with_same_frame_of_reference());
    const auto env = get_env_with_default_rotation_convention();
    const double t = 0;
    auto states = get_body("TestShip")->get_states();
    states.x.record(t, 1);
    states.y.record(t, 2);
    states.z.record(t, 3);
    states.u.record(t, 4);
    states.v.record(t, 5);
    states.w.record(t, 6);
    states.p.record(t, 7);
    states.q.record(t, 8);
    states.r.record(t, 9);

    ssc::data_source::DataSource command_listener;
    command_listener.set("test(a)", 0.);
    command_listener.set("test(b)", 0.);
    command_listener.set("test(c)", 0.);

    ManeuveringForceModel force(data,"TestShip", env);
    const auto F = force(states, t, command_listener, env.k, states.G);

    ASSERT_EQ("TestShip", F.get_frame());
    ASSERT_DOUBLE_EQ(-93470409.32377005, F.X());
    ASSERT_DOUBLE_EQ(190870415.43062863, F.Y());
    ASSERT_DOUBLE_EQ(0, F.Z());
    ASSERT_NEAR(0, F.K(), 1e-15);
    ASSERT_NEAR(0, F.M(), 1e-15);
    ASSERT_DOUBLE_EQ(253134236.3875341, F.N());
}

TEST_F(ManeuveringForceModelTest, can_evaluate_full_maneuvering_model_with_same_frame_of_reference2)
{
    auto data = ManeuveringForceModel::parse(test_data::maneuvering_with_same_frame_of_reference());
    const auto env = get_env_with_default_rotation_convention();
    const double t = 0;
    auto states = get_body("TestShip")->get_states();
    states.x.record(t, 0.1);
    states.y.record(t, 2.04);
    states.z.record(t, 6.28);
    states.u.record(t, 0.45);
    states.v.record(t, 0.01);
    states.w.record(t, 5.869);
    states.p.record(t, 0.23);
    states.q.record(t, 0);
    states.r.record(t, 0.38);

    ssc::data_source::DataSource command_listener;
    command_listener.set("test(a)", 0.);
    command_listener.set("test(b)", 0.);
    command_listener.set("test(c)", 0.);

    ManeuveringForceModel force(data,"TestShip", env);
    const auto F = force(states, t, command_listener, env.k, states.G);

    ASSERT_EQ("TestShip", F.get_frame());
    ASSERT_DOUBLE_EQ(-160307.53008106418, F.X());
    ASSERT_DOUBLE_EQ(349066.3153463915, F.Y());
    ASSERT_DOUBLE_EQ(0, F.Z());
    ASSERT_NEAR(0, F.K(), 1e-15);
    ASSERT_NEAR(0, F.M(), 1e-15);
    ASSERT_DOUBLE_EQ(-178317.02217866198, F.N());
}

/*  The following test data was generated adding this Python code
to the function defined above:

from math import cos, sin
import numpy as np

rotX = lambda alpha: np.array([[1.0, 0.0, 0.0], [0.0, +cos(alpha), -sin(alpha)], [0.0, +sin(alpha), +cos(alpha)]])
rotY = lambda alpha: np.array([[+cos(alpha), 0.0, +sin(alpha)], [0.0, 1.0, 0.0], [-sin(alpha), 0.0, +cos(alpha)]])
rotZ = lambda alpha: np.array([[+cos(alpha), -sin(alpha), 0.0], [+sin(alpha), +cos(alpha), 0.0], [0.0, 0.0, 1.0]])

def get_force_and_torque(x, y, z, u, v, w, p, q, r, phi, theta, psi, tx, ty, tz):
    man_results = man(x, y, z, u, v, w, p, q, r)
    F_local = np.array([[man_results['X']], [man_results['Y']], [man_results['Z']]])
    rot = rotZ(psi) @ rotY(theta) @ rotX(phi)
    F = rot @ F_local

    T_local = np.array([[man_results['K']], [man_results['M']], [man_results['N']]])
    OM = np.array([[tx], [ty], [tz]])
    T = np.cross(OM.T, F.T).T + (rot @ T_local)

    return {'F': F,\
            'T': T
           }
 */

TEST_F(ManeuveringForceModelTest, can_evaluate_full_maneuvering_model)
{
    auto data = ManeuveringForceModel::parse(test_data::maneuvering());
    const auto env = get_env_with_default_rotation_convention();
    const double t = 0;
    auto states = get_body("TestShip")->get_states();
    states.x.record(t, 1);
    states.y.record(t, 2);
    states.z.record(t, 3);
    states.u.record(t, 4);
    states.v.record(t, 5);
    states.w.record(t, 6);
    states.p.record(t, 7);
    states.q.record(t, 8);
    states.r.record(t, 9);

    ssc::data_source::DataSource command_listener;
    command_listener.set("test(a)", 0.);
    command_listener.set("test(b)", 0.);
    command_listener.set("test(c)", 0.);

    ManeuveringForceModel force(data,"TestShip", env);
    const auto F = force(states, t, command_listener, env.k, states.G);

    ASSERT_EQ("TestShip", F.get_frame());
    ASSERT_NEAR(-154542121.87051487, F.X(), 1e-7);
    ASSERT_NEAR(-33809357.80807002, F.Y(), 1e-7);
    ASSERT_NEAR(-141922136.39238492, F.Z(), 1e-7);
    ASSERT_NEAR(208388848.83699098, F.K(), 1e-7);
    ASSERT_NEAR(-65195179.1700744, F.M(), 1e-7);
    ASSERT_NEAR(-211388078.67544204, F.N(), 1e-7);
}

TEST_F(ManeuveringForceModelTest, can_evaluate_full_maneuvering_model2)
{
    auto data = ManeuveringForceModel::parse(test_data::maneuvering());
    const auto env = get_env_with_default_rotation_convention();
    const double t = 0;
    auto states = get_body("TestShip")->get_states();
    states.x.record(t, 0.1);
    states.y.record(t, 2.04);
    states.z.record(t, 6.28);
    states.u.record(t, 0.45);
    states.v.record(t, 0.01);
    states.w.record(t, 5.869);
    states.p.record(t, 0.23);
    states.q.record(t, 0);
    states.r.record(t, 0.38);

    ssc::data_source::DataSource command_listener;
    command_listener.set("test(a)", 0.);
    command_listener.set("test(b)", 0.);
    command_listener.set("test(c)", 0.);

    ManeuveringForceModel force(data,"TestShip", env);
    const auto F = force(states, t, command_listener, env.k, states.G);

    ASSERT_EQ("TestShip", F.get_frame());
    ASSERT_NEAR(-276711.3255011981, F.X(), 1e-9);
    ASSERT_NEAR(-70281.82055578004, F.Y(), 1e-9);
    ASSERT_NEAR(-256976.85644612607, F.Z(), 1e-9);
    ASSERT_NEAR(-13365.244892291332, F.K(), 1e-9);
    ASSERT_NEAR(-252382.944875907, F.M(), 1e-9);
    ASSERT_NEAR(83417.03517334405, F.N(), 1e-9);
}

TEST_F(ManeuveringForceModelTest, can_use_euler_angles_in_maneuvering_with_same_frame_of_reference)
{
    const std::string yaml = "reference frame:\n"
                             "    frame: TestShip\n"
                             "    x: {value: 0, unit: m}\n"
                             "    y: {value: 0, unit: m}\n"
                             "    z: {value: 0, unit: m}\n"
                             "    phi: {value: 0, unit: rad}\n"
                             "    theta: {value: 0, unit: deg}\n"
                             "    psi: {value: 0, unit: deg}\n"
                             "name: test\n"
                             "X: phi(t)\n"
                             "Y: theta(t)\n"
                             "Z: psi(t)\n"
                             "K: qr(t)\n"
                             "M: qi(t)\n"
                             "N: qj(t)+qk(t)\n";
    auto data = ManeuveringForceModel::parse(yaml);
    const auto env = get_env_with_default_rotation_convention();
    const double t = 0;
    auto states = get_body("TestShip")->get_states();
    states.x.record(t, 0.1);
    states.y.record(t, 2.04);
    states.z.record(t, 6.28);
    states.u.record(t, 0.45);
    states.v.record(t, 0.01);
    states.w.record(t, 5.869);
    states.p.record(t, 0.23);
    states.q.record(t, 0);
    states.r.record(t, 0.38);
    states.qr.record(t, 0.47978680878362095);
    states.qi.record(t, 0.4931142201387216);
    states.qj.record(t, 0.5064416314938222);
    states.qk.record(t, 0.5197690428489228);

    YamlRotation rot;
    rot.order_by = "angle";
    rot.convention.push_back("z");
    rot.convention.push_back("y'");
    rot.convention.push_back("x''");
    const auto angles = states.get_angles(rot);

    ssc::data_source::DataSource command_listener;

    ManeuveringForceModel force(data,"TestShip", env);
    const auto F = force(states, t, command_listener, env.k, states.G);
    ASSERT_DOUBLE_EQ(angles.phi, (double)F.X());
    ASSERT_DOUBLE_EQ(angles.theta, (double)F.Y());
    ASSERT_DOUBLE_EQ(angles.psi, (double)F.Z());
    ASSERT_DOUBLE_EQ(states.qr(), (double)F.K());
    ASSERT_DOUBLE_EQ(states.qi(), (double)F.M());
    ASSERT_DOUBLE_EQ(states.qj()+states.qk(), (double)F.N());
}

/*  The following test data was generated using this Python code (the input quaternion should be normalized):

from math import cos, sin
from scipy.spatial.transform import Rotation
import numpy as np

rotX = lambda alpha: np.array([[1.0, 0.0, 0.0], [0.0, +cos(alpha), -sin(alpha)], [0.0, +sin(alpha), +cos(alpha)]])
rotY = lambda alpha: np.array([[+cos(alpha), 0.0, +sin(alpha)], [0.0, 1.0, 0.0], [-sin(alpha), 0.0, +cos(alpha)]])
rotZ = lambda alpha: np.array([[+cos(alpha), -sin(alpha), 0.0], [+sin(alpha), +cos(alpha), 0.0], [0.0, 0.0, 1.0]])

def get_force_and_torque(x, y, z, u, v, w, p, q, r, qr, qi, qj, qk, phi, theta, psi, tx, ty, tz):
    rotation_fom_quaternions = Rotation.from_quat([qk, qj, qi, qr]).as_euler('ZYX')

    F_local = np.array([[rotation_fom_quaternions[0]], [rotation_fom_quaternions[1]], [rotation_fom_quaternions[2]]])
    rot = rotZ(psi) @ rotY(theta) @ rotX(phi)
    F = rot @ F_local

    T_local = np.array([[qr], [qi], [qj + qk]])
    OM = np.array([[tx], [ty], [tz]])
    T = np.cross(OM.T, F.T).T + (rot @ T_local)

    return {'F': F,\
            'T': T
           }
 */

TEST_F(ManeuveringForceModelTest, can_use_euler_angles_in_maneuvering)
{
    const std::string yaml = "reference frame:\n"
                             "    frame: TestShip\n"
                             "    x: {value: 0.696, unit: m}\n"
                             "    y: {value: 0, unit: m}\n"
                             "    z: {value: 1.418, unit: m}\n"
                             "    phi: {value: 0.7, unit: rad}\n"
                             "    theta: {value: -166, unit: deg}\n"
                             "    psi: {value: 125, unit: deg}\n"
                             "name: test\n"
                             "X: phi(t)\n"
                             "Y: theta(t)\n"
                             "Z: psi(t)\n"
                             "K: qr(t)\n"
                             "M: qi(t)\n"
                             "N: qj(t)+qk(t)\n";
    auto data = ManeuveringForceModel::parse(yaml);
    const auto env = get_env_with_default_rotation_convention();
    const double t = 0;
    auto states = get_body("TestShip")->get_states();
    states.x.record(t, 0.1);
    states.y.record(t, 2.04);
    states.z.record(t, 6.28);
    states.u.record(t, 0.45);
    states.v.record(t, 0.01);
    states.w.record(t, 5.869);
    states.p.record(t, 0.23);
    states.q.record(t, 0);
    states.r.record(t, 0.38);
    states.qr.record(t, 0.47978680878362095);
    states.qi.record(t, 0.4931142201387216);
    states.qj.record(t, 0.5064416314938222);
    states.qk.record(t, 0.5197690428489228);

    ssc::data_source::DataSource command_listener;

    ManeuveringForceModel force(data,"TestShip", env);
    const auto F = force(states, t, command_listener, env.k, states.G);
    ASSERT_NEAR(1.917565962059328, (double)F.X(), 1e-14);
    ASSERT_NEAR(-0.8788836899394064, (double)F.Y(), 1e-14);
    ASSERT_NEAR(-0.8088077018359574, (double)F.Z(), 1e-14);
    ASSERT_NEAR(1.8988663216207913, (double)F.K(), 1e-14);
    ASSERT_NEAR(2.8450652134487004, (double)F.M(), 1e-14);
    ASSERT_NEAR(-1.5654434261669063, (double)F.N(), 1e-14);
}

TEST_F(ManeuveringForceModelTest, can_get_Tmax)
{
    const auto data = ManeuveringForceModel::parse(test_data::man_with_delay());
    const auto env = get_env_with_default_rotation_convention();
    ManeuveringForceModel force(data,"ball", env);
    ASSERT_DOUBLE_EQ(10, force.get_Tmax());
}
