/*
 * ManeuveringForceModelTest.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: cady
 */


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
