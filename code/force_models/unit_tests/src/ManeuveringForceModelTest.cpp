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
    const auto c = maneuvering::make_constant(4);
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_DOUBLE_EQ(4, f(states, ds, t));
}

TEST_F(ManeuveringForceModelTest, internal_cos)
{
    const auto c = make_cos(maneuvering::make_constant(PI/3.));
    const auto f = c->get_lambda();
    BodyStates states;
    ssc::data_source::DataSource ds;
    const double t = a.random<double>();
    ASSERT_DOUBLE_EQ(0.5, f(states, ds, t));
}
