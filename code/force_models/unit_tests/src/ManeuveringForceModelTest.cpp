/*
 * ManeuveringForceModelTest.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: cady
 */

#include "EnvironmentAndFrames.hpp"
#include "ManeuveringForceModelTest.hpp"
#include "ManeuveringForceModel.hpp"
#include "ManeuveringInternal.hpp"
#include "yaml_data.hpp"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

using namespace maneuvering;

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

TEST_F(ManeuveringForceModelTest, can_parse_point_of_application)
{
//! [ManeuveringForceModelTest example]
    const auto data = ManeuveringForceModel::parse(test_data::maneuvering());
//! [ManeuveringForceModelTest example]
//! [ManeuveringForceModelTest expected output]
    ASSERT_DOUBLE_EQ(0.696, data.point_of_application.x);
    ASSERT_DOUBLE_EQ(0, data.point_of_application.y);
    ASSERT_DOUBLE_EQ(1.418, data.point_of_application.z);
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
    ASSERT_DOUBLE_EQ(x, f(states, ds, t));
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
    const auto xx = make_state_x(make_constant(5));
    const auto yy = make_state_y(make_constant(5));
    const auto zz = make_state_z(make_constant(5));
    const auto uu = make_state_u(make_constant(5));
    const auto vv = make_state_v(make_constant(5));
    const auto ww = make_state_w(make_constant(5));
    const auto pp = make_state_p(make_constant(5));
    const auto qq = make_state_q(make_constant(5));
    const auto rr = make_state_r(make_constant(5));
    const auto x = xx->get_lambda();
    const auto y = yy->get_lambda();
    const auto z = zz->get_lambda();
    const auto u = uu->get_lambda();
    const auto v = vv->get_lambda();
    const auto w = ww->get_lambda();
    const auto p = pp->get_lambda();
    const auto q = qq->get_lambda();
    const auto r = rr->get_lambda();
    BodyStates states;
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

TEST_F(ManeuveringForceModelTest, can_evaluate_simple_maneuvering_model)
{
    const std::string yaml = "point of application (in body frame):\n"
                             "    x: {value: 0.696, unit: m}\n"
                             "    y: {value: 0, unit: m}\n"
                             "    z: {value: 1.418, unit: m}\n"
                             "X: 2*Y+sqrt(x(t))\n"
                             "Y: y(t)^2\n"
                             "Z: 0\n"
                             "K: 0\n"
                             "M: 0\n"
                             "N: 0\n";
    EnvironmentAndFrames env;
    ManeuveringForceModel force(ManeuveringForceModel::parse(yaml),"some body", env);
    BodyStates states;
    const double t = 10;
    states.x.record(t, 1024);
    states.y.record(t, 400);
    const auto F = force(states, t);

    ASSERT_EQ("some body", F.get_frame());
    ASSERT_DOUBLE_EQ(320032, F.X());
    ASSERT_DOUBLE_EQ(160000, F.Y());
    ASSERT_DOUBLE_EQ(0, F.Z());
    ASSERT_DOUBLE_EQ(0, F.K());
    ASSERT_DOUBLE_EQ(0, F.M());
    ASSERT_DOUBLE_EQ(0, F.N());
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

TEST_F(ManeuveringForceModelTest, can_evaluate_full_maneuvering_model)
{
    const auto data = ManeuveringForceModel::parse(test_data::maneuvering());
    EnvironmentAndFrames env;
    ManeuveringForceModel force(data,"some body", env);

    BodyStates states;

    states.x.record(0, 1);
    states.y.record(0, 2);
    states.z.record(0, 3);
    states.u.record(0, 4);
    states.v.record(0, 5);
    states.w.record(0, 6);
    states.p.record(0, 7);
    states.q.record(0, 8);
    states.r.record(0, 9);

    const double t = 0;

    const auto F = force(states, t);

    ASSERT_EQ("some body", F.get_frame());
    ASSERT_DOUBLE_EQ(-93470409.32377005, F.X());
    ASSERT_DOUBLE_EQ(190870415.43062863, F.Y());
    ASSERT_DOUBLE_EQ(0, F.Z());
    ASSERT_DOUBLE_EQ(0, F.K());
    ASSERT_DOUBLE_EQ(0, F.M());
    ASSERT_DOUBLE_EQ(253134236.3875341, F.N());
}

TEST_F(ManeuveringForceModelTest, can_evaluate_full_maneuvering_model2)
{
    const auto data = ManeuveringForceModel::parse(test_data::maneuvering());
    EnvironmentAndFrames env;
    ManeuveringForceModel force(data,"some body", env);

    BodyStates states;

    states.x.record(0, 0.1);
    states.y.record(0, 2.04);
    states.z.record(0, 6.28);
    states.u.record(0, 0.45);
    states.v.record(0, 0.01);
    states.w.record(0, 5.869);
    states.p.record(0, 0.23);
    states.q.record(0, 0);
    states.r.record(0, 0.38);

    const double t = 0;

    const auto F = force(states, t);
    ASSERT_EQ("some body", F.get_frame());
    ASSERT_DOUBLE_EQ(-160307.53008106418, F.X());
    ASSERT_DOUBLE_EQ(349066.3153463915, F.Y());
    ASSERT_DOUBLE_EQ(0, F.Z());
    ASSERT_DOUBLE_EQ(0, F.K());
    ASSERT_DOUBLE_EQ(0, F.M());
    ASSERT_DOUBLE_EQ(-178317.02217866198, F.N());
}
