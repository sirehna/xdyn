/*
 * WageningenControlledForceModelTest.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "BodyStates.hpp"
#include "yaml_data.hpp"
#include "WageningenControlledForceModel.hpp"
#include "WageningenControlledForceModelTest.hpp"
#include "InvalidInputException.hpp"

#define EPS 1E-2
#define NB_TRIALS 100

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define DEG2RAD (PI/180.)


WageningenControlledForceModelTest::WageningenControlledForceModelTest() : a(ssc::random_data_generator::DataGenerator(7))
{
}

WageningenControlledForceModelTest::~WageningenControlledForceModelTest()
{
}

void WageningenControlledForceModelTest::SetUp()
{
}

void WageningenControlledForceModelTest::TearDown()
{
}

TEST_F(WageningenControlledForceModelTest, parser)
{
    const auto w = WageningenControlledForceModel::parse(test_data::wageningen());
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
}

TEST_F(WageningenControlledForceModelTest, should_throw_if_blade_area_ratio_is_outside_bounds)
{
    auto input  = WageningenControlledForceModel::parse(test_data::wageningen());
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        input.blade_area_ratio = a.random<double>().between(0,0.3);
        ASSERT_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()), InvalidInputException);
        input.blade_area_ratio = a.random<double>().between(1.05,10);
        ASSERT_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()), InvalidInputException);
        input.blade_area_ratio = a.random<double>().outside(0.3,1.05);
        ASSERT_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()), InvalidInputException);
        input.blade_area_ratio = a.random<double>().between(0.3,1.05);
        ASSERT_NO_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()));
    }
}

TEST_F(WageningenControlledForceModelTest, should_throw_if_number_of_blades_is_outside_bounds)
{
    auto input  = WageningenControlledForceModel::parse(test_data::wageningen());
    input.number_of_blades = 0;
    ASSERT_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()), InvalidInputException);
    input.number_of_blades = 1;
    ASSERT_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()), InvalidInputException);
    input.number_of_blades = 8;
    ASSERT_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()), InvalidInputException);
    input.number_of_blades = 2;
    ASSERT_NO_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()));
    input.number_of_blades = 3;
    ASSERT_NO_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()));
    input.number_of_blades = 4;
    ASSERT_NO_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()));
    input.number_of_blades = 5;
    ASSERT_NO_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()));
    input.number_of_blades = 6;
    ASSERT_NO_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()));
    input.number_of_blades = 7;
    ASSERT_NO_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()));
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        input.number_of_blades = a.random<size_t>().greater_than(7);
        ASSERT_THROW(WageningenControlledForceModel w(input, "", EnvironmentAndFrames()), InvalidInputException);
    }
}

TEST_F(WageningenControlledForceModelTest, Kt_should_issue_a_warning_if_P_D_is_outside_bounds)
{
    auto input  = WageningenControlledForceModel::parse(test_data::wageningen());
    const WageningenControlledForceModel w(WageningenControlledForceModel::parse(test_data::wageningen()), "", EnvironmentAndFrames());
    size_t Z;
    double AE_A0, P_D, J;
    std::stringstream error;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(error.rdbuf());

    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        w.Kt(Z=a.random<size_t>().between(2,7),
               AE_A0=a.random<double>().between(0.3,1.05),
               P_D=a.random<double>().between(0,0.5),
               J = a.random<double>().between(0,1.5));
        EXPECT_FALSE(error.str().empty());
        error.str("");
        w.Kt(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().between(1.4,10),
             J = a.random<double>().between(0,1.5));
        EXPECT_FALSE(error.str().empty());
        error.str("");
        w.Kt(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().outside(0.5,1.4),
             J = a.random<double>().between(0,1.5));
        EXPECT_FALSE(error.str().empty());
        error.str("");
        w.Kt(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().between(0.5,1.4),
             J = a.random<double>().between(0,1.5));
        EXPECT_TRUE(error.str().empty());
    }
    std::cerr.rdbuf(orig);
}

TEST_F(WageningenControlledForceModelTest, Kt_should_throw_if_J_is_outside_bounds)
{
    auto input  = WageningenControlledForceModel::parse(test_data::wageningen());
    const WageningenControlledForceModel w(WageningenControlledForceModel::parse(test_data::wageningen()), "", EnvironmentAndFrames());
    size_t Z;
    double AE_A0, P_D, J;
    std::stringstream error;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(error.rdbuf());
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        w.Kt(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().between(0.5,1.4),
             J = a.random<double>().no().greater_than(0));
        EXPECT_FALSE(error.str().empty());
        error.str("");
        w.Kt(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().between(0.5,1.4),
             J = a.random<double>().between(1.5,15));
        EXPECT_FALSE(error.str().empty());
        error.str("");
        w.Kt(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().between(0.5,1.4),
             J = a.random<double>().greater_than(1.5));
        EXPECT_FALSE(error.str().empty());
        error.str("");
        w.Kt(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().between(0.5,1.4),
             J = a.random<double>().between(0,1.5));
        EXPECT_TRUE(error.str().empty());
    }
    std::cerr.rdbuf(orig);
}

TEST_F(WageningenControlledForceModelTest, Kq_should_throw_if_P_D_is_outside_bounds)
{
    auto input  = WageningenControlledForceModel::parse(test_data::wageningen());
    const WageningenControlledForceModel w(WageningenControlledForceModel::parse(test_data::wageningen()), "", EnvironmentAndFrames());
    size_t Z;
    double AE_A0, P_D, J;
    std::stringstream error;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(error.rdbuf());
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        w.Kq(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().between(0,0.5),
             J = a.random<double>().between(0,1.5));
        EXPECT_FALSE(error.str().empty());
        error.str("");
        w.Kq(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().between(1.4,10),
             J = a.random<double>().between(0,1.5));
        EXPECT_FALSE(error.str().empty());
        error.str("");
        w.Kq(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().outside(0.5,1.4),
             J = a.random<double>().between(0,1.5));
        EXPECT_FALSE(error.str().empty());
        error.str("");
        w.Kq(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().between(0.5,1.4),
             J = a.random<double>().between(0,1.5));
        EXPECT_TRUE(error.str().empty());
    }
    std::cerr.rdbuf(orig);
}

TEST_F(WageningenControlledForceModelTest, Kq_should_throw_if_J_is_outside_bounds)
{
    auto input  = WageningenControlledForceModel::parse(test_data::wageningen());
    const WageningenControlledForceModel w(WageningenControlledForceModel::parse(test_data::wageningen()), "", EnvironmentAndFrames());
    size_t Z;
    double AE_A0, P_D, J;
    std::stringstream error;
    // Redirect cerr to our stringstream buffer or any other ostream
    std::streambuf* orig =std::cerr.rdbuf(error.rdbuf());
    for (size_t i = 0 ; i < NB_TRIALS ; ++i)
    {
        w.Kq(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().between(0.5,1.4),
             J = a.random<double>().no().greater_than(0));
        EXPECT_FALSE(error.str().empty());
        error.str("");
        w.Kq(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().between(0.5,1.4),
             J = a.random<double>().between(1.5,15));
        EXPECT_FALSE(error.str().empty());
        error.str("");
        w.Kq(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().between(0.5,1.4),
             J = a.random<double>().greater_than(1.5));
        EXPECT_FALSE(error.str().empty());
        error.str("");
        w.Kq(Z=a.random<size_t>().between(2,7),
             AE_A0=a.random<double>().between(0.3,1.05),
             P_D=a.random<double>().between(0.5,1.4),
             J = a.random<double>().between(0,1.5));
        EXPECT_TRUE(error.str().empty());
    }
    std::cerr.rdbuf(orig);
}

TEST_F(WageningenControlledForceModelTest, KT)
{
//! [WageningenControlledForceModelTest KT_example]
    const WageningenControlledForceModel w(WageningenControlledForceModel::parse(test_data::wageningen()), "", EnvironmentAndFrames());
    ASSERT_EQ("wageningen B-series", w.model_name());
    size_t Z;
    double AE_A0, P_D, J;
    // B6-65 (cf. The Wageningen Propeller Series, 1992, Gert Kuiper, Marin publication 92-001 page 128
    EXPECT_NEAR(0.603363,    w.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=0), EPS);
    EXPECT_NEAR(0.544592015, w.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=0.3), EPS);
    EXPECT_NEAR(0.569237985, w.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=0.2), EPS);
    EXPECT_NEAR(0.588950654, w.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=0.1), EPS);
    EXPECT_NEAR(0.161314404, w.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=1.2), EPS);
    EXPECT_NEAR(0.10751365,  w.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=1.3), EPS);
    EXPECT_NEAR(0.05281524,  w.Kt(Z=6, AE_A0=0.65, P_D=1.4, J=1.4), EPS);
    // B2-30 (cf. The Wageningen Propeller Series, 1992, Gert Kuiper, Marin publication 92-001 page 111
    EXPECT_NEAR(0.242745716, w.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0), EPS);
    EXPECT_NEAR(0.21922492, w.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0.1), EPS);
    EXPECT_NEAR(0.193313888, w.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0.2), EPS);
    EXPECT_NEAR(0.165272939, w.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0.3), EPS);
    EXPECT_NEAR(0.13535988, w.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0.4), EPS);
    EXPECT_NEAR(0.103833149, w.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0.5), EPS);
    EXPECT_NEAR(0.070951181, w.Kt(Z=2, AE_A0=0.30, P_D=0.7, J=0.6), EPS);
    // B3-40
    EXPECT_NEAR(0.188195882, w.Kt(Z=3, AE_A0=0.4, P_D=0.5, J=0.0), EPS);
    EXPECT_NEAR(0.163443634, w.Kt(Z=3, AE_A0=0.4, P_D=0.5, J=0.1), EPS);
    EXPECT_NEAR(0.135394546, w.Kt(Z=3, AE_A0=0.4, P_D=0.5, J=0.2), EPS);
    EXPECT_NEAR(0.104372086, w.Kt(Z=3, AE_A0=0.4, P_D=0.5, J=0.3), EPS);
//! [WageningenControlledForceModelTest KT_example]
}

TEST_F(WageningenControlledForceModelTest, KQ)
{
    //! [WageningenControlledForceModelTest KQ_example]
    const WageningenControlledForceModel w(WageningenControlledForceModel::parse(test_data::wageningen()), "", EnvironmentAndFrames());
    ASSERT_EQ("wageningen B-series", w.model_name());
    size_t Z;
    double AE_A0, P_D, J;

    EXPECT_NEAR(0.47,    10*w.Kq(Z=2, AE_A0=0.30, P_D=1.4, J=0.8), 1E-2);
    EXPECT_NEAR(0.51,    10*w.Kq(Z=3, AE_A0=0.65, P_D=1.2, J=0.7), 1E-2);
    EXPECT_NEAR(0.31,    10*w.Kq(Z=3, AE_A0=0.65, P_D=0.8, J=0.3), 1E-2);
    EXPECT_NEAR(0.51,    10*w.Kq(Z=6, AE_A0=0.65, P_D=1.2, J=0.8), 1E-2);
    EXPECT_NEAR(0.99,    10*w.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=0.5), 1E-2);
    EXPECT_NEAR(0.20,    10*w.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=1), 1E-2);

    // B6-65 (cf. The Wageningen Propeller Series, 1992, Gert Kuiper, Marin publication 92-001 page 128
    EXPECT_NEAR(1.209493726, 10*w.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=0), EPS);
    EXPECT_NEAR(1.1769086,   10*w.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=0.1), EPS);
    EXPECT_NEAR(1.138594465, 10*w.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=0.2), EPS);
    EXPECT_NEAR(1.094459956, 10*w.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=0.3), EPS);
    EXPECT_NEAR(0.420257329, 10*w.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=1.2), EPS);
    EXPECT_NEAR(0.312894007, 10*w.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=1.3), EPS);
    EXPECT_NEAR(0.198705297, 10*w.Kq(Z=6, AE_A0=0.65, P_D=1.4, J=1.4), EPS);

    // B2-30 (cf. The Wageningen Propeller Series, 1992, Gert Kuiper, Marin publication 92-001 page 111
    EXPECT_NEAR(0.654180539, 10*w.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0), EPS);
    EXPECT_NEAR(0.618341564, 10*w.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0.1), EPS);
    EXPECT_NEAR(0.580406412, 10*w.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0.2), EPS);
    EXPECT_NEAR(0.540388943, 10*w.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0.3), EPS);
    EXPECT_NEAR(0.498303012, 10*w.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0.4), EPS);
    EXPECT_NEAR(0.454162477, 10*w.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0.5), EPS);
    EXPECT_NEAR(0.407981197, 10*w.Kq(Z=2, AE_A0=0.30, P_D=1.2, J=0.6), EPS);

    // B3-40
    EXPECT_NEAR(0.160531235, 10*w.Kq(Z=3, AE_A0=0.4, P_D=0.5, J=0.0), EPS);
    EXPECT_NEAR(0.143676942, 10*w.Kq(Z=3, AE_A0=0.4, P_D=0.5, J=0.1), EPS);
    EXPECT_NEAR(0.125396067, 10*w.Kq(Z=3, AE_A0=0.4, P_D=0.5, J=0.2), EPS);
    EXPECT_NEAR(0.105448718, 10*w.Kq(Z=3, AE_A0=0.4, P_D=0.5, J=0.3), EPS);
//! [WageningenControlledForceModelTest KQ_example]
}

TEST_F(WageningenControlledForceModelTest, can_calculate_advance_ratio)
{
    const WageningenControlledForceModel w(WageningenControlledForceModel::parse(test_data::wageningen()), "", EnvironmentAndFrames());
    BodyStates states;
    states.u.record(0, 3);
    std::map<std::string,double> commands;
    commands["rpm"] = 20*2*PI;
    ASSERT_DOUBLE_EQ(3./400., w.advance_ratio(states, commands));
}

TEST_F(WageningenControlledForceModelTest, force)
{
    auto input = WageningenControlledForceModel::parse(test_data::wageningen());
    input.blade_area_ratio = 0.4;
    EnvironmentAndFrames env;
    env.rho = 1024;
    const WageningenControlledForceModel w(input, "", env);
    BodyStates states;
    states.u.record(0, 1);

    std::map<std::string,double> commands;
    commands["rpm"] = 5*(2*PI);
    commands["P/D"] = 0.5;

    ASSERT_NEAR(0.3*1024*25*16*0.18587823151195928539, w.get_force(states, a.random<double>(),commands)(0), EPS);
    ASSERT_EQ(0, w.get_force(states, a.random<double>(),commands)(1));
    ASSERT_EQ(0, w.get_force(states, a.random<double>(),commands)(2));
    ASSERT_EQ(0, w.get_force(states, a.random<double>(),commands)(4));
    ASSERT_EQ(0, w.get_force(states, a.random<double>(),commands)(5));
}

TEST_F(WageningenControlledForceModelTest, torque)
{
    auto input = WageningenControlledForceModel::parse(test_data::wageningen());
    input.blade_area_ratio = 0.4;
    EnvironmentAndFrames env;
    env.rho = 1024;
    const WageningenControlledForceModel w(input, "", env);
    BodyStates states;
    states.u.record(0, 1);

    std::map<std::string,double> commands;
    commands["rpm"] = 5*(2*PI);
    commands["P/D"] = 0.5;

    ASSERT_NEAR(-1024*25*32*0.015890316523410611543, w.get_force(states, a.random<double>(),commands)(3), EPS);
}

TEST_F(WageningenControlledForceModelTest, torque_should_have_sign_corresponding_to_rotation)
{
    auto input = WageningenControlledForceModel::parse(test_data::wageningen());
    BodyStates states;
    states.u.record(0, a.random<double>().greater_than(0));
    EnvironmentAndFrames env;
    env.rho = a.random<double>().greater_than(0);

    const WageningenControlledForceModel w_clockwise(input, "", env);
    input.rotating_clockwise = false;
    const WageningenControlledForceModel w_anti_clockwise(input, "", env);

    std::map<std::string,double> commands;
    commands["rpm"] = a.random<double>().between(states.u(),2*states.u());
    commands["P/D"] = a.random<double>().between(0.5,1.4);
    ASSERT_GT(0, w_clockwise.get_force(states, a.random<double>(),commands)(3));
    ASSERT_LT(0, w_anti_clockwise.get_force(states, a.random<double>(),commands)(3));
}

TEST_F(WageningenControlledForceModelTest, bug_2825_can_use_propeller_with_rpm_zero)
{
    auto input = WageningenControlledForceModel::parse(test_data::wageningen());
    input.blade_area_ratio = 0.4;
    EnvironmentAndFrames env;
    env.rho = 1024;
    const WageningenControlledForceModel w(input, "", env);
    BodyStates states;
    states.u.record(0, 1);

    std::map<std::string,double> commands;
    commands["rpm"] = 0;
    commands["P/D"] = 0.5;

    ASSERT_NEAR(0, w.get_force(states, a.random<double>(),commands)(0), EPS);
    ASSERT_NEAR(0, w.get_force(states, a.random<double>(),commands)(1), EPS);
    ASSERT_NEAR(0, w.get_force(states, a.random<double>(),commands)(2), EPS);
    ASSERT_NEAR(0, w.get_force(states, a.random<double>(),commands)(3), EPS);
    ASSERT_NEAR(0, w.get_force(states, a.random<double>(),commands)(4), EPS);
    ASSERT_NEAR(0, w.get_force(states, a.random<double>(),commands)(5), EPS);
}

TEST_F(WageningenControlledForceModelTest, bug_2825_can_use_propeller_with_rpm_near_zero)
{
    auto input = WageningenControlledForceModel::parse(test_data::wageningen());
    input.blade_area_ratio = 0.4;
    EnvironmentAndFrames env;
    env.rho = 1024;
    const WageningenControlledForceModel w(input, "", env);
    BodyStates states;
    states.u.record(0, 1);

    std::map<std::string,double> commands;
    commands["rpm"] = 1e-16;
    commands["P/D"] = 0.5;

    ASSERT_NEAR(0, w.get_force(states, a.random<double>(),commands)(0), EPS);
    ASSERT_NEAR(0, w.get_force(states, a.random<double>(),commands)(1), EPS);
    ASSERT_NEAR(0, w.get_force(states, a.random<double>(),commands)(2), EPS);
    ASSERT_NEAR(0, w.get_force(states, a.random<double>(),commands)(3), EPS);
    ASSERT_NEAR(0, w.get_force(states, a.random<double>(),commands)(4), EPS);
    ASSERT_NEAR(0, w.get_force(states, a.random<double>(),commands)(5), EPS);
}
