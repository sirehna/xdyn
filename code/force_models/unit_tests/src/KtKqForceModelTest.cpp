/*
 * KtKqForceModelTest.cpp
 *
 *  Created on: Jun 27, 2015
 *      Author: cady
 */

#include "NumericalErrorException.hpp"
#include "KtKqForceModel.hpp"
#include "KtKqForceModelTest.hpp"
#include "yaml_data.hpp"

#include "gmock/gmock.h"
using ::testing::_;
using ::testing::ElementsAreArray;

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define DEG2RAD (atan(1)/45.)
#define EPS 1E-6

KtKqForceModelTest::KtKqForceModelTest() : a(ssc::random_data_generator::DataGenerator(9876))
{
}

KtKqForceModelTest::~KtKqForceModelTest()
{
}

void KtKqForceModelTest::SetUp()
{
}

void KtKqForceModelTest::TearDown()
{
}

TEST_F(KtKqForceModelTest, parser)
{
    const auto k = KtKqForceModel::parse(test_data::kt_kq());
    ASSERT_EQ("port side propeller", k.name);
    ASSERT_DOUBLE_EQ(0, k.position_of_propeller_frame.angle.phi);
    ASSERT_DOUBLE_EQ(-10*DEG2RAD, k.position_of_propeller_frame.angle.theta);
    ASSERT_DOUBLE_EQ(-1*DEG2RAD, k.position_of_propeller_frame.angle.psi);
    ASSERT_DOUBLE_EQ(-4, k.position_of_propeller_frame.coordinates.x);
    ASSERT_DOUBLE_EQ(-2, k.position_of_propeller_frame.coordinates.y);
    ASSERT_DOUBLE_EQ(2, k.position_of_propeller_frame.coordinates.z);
    ASSERT_EQ("mesh(body 1)", k.position_of_propeller_frame.frame);
    ASSERT_DOUBLE_EQ(1, k.relative_rotative_efficiency);
    ASSERT_TRUE(k.rotating_clockwise);
    ASSERT_DOUBLE_EQ(0.7, k.thrust_deduction_factor);
    ASSERT_DOUBLE_EQ(0.9, k.wake_coefficient);
    ASSERT_DOUBLE_EQ(2, k.diameter);
    ASSERT_THAT(k.J, ElementsAreArray({-1.00000E+00,-8.00000E-01,-5.00000E-01,-2.50000E-01,-1.00000E-03,1.00000E-03, 2.00000E-01, 4.00000E-01, 6.00000E-01, 7.00000E-01, 8.00000E-01,1.00000E+00}));
    ASSERT_THAT(k.Kt, ElementsAreArray({-4.50000E-01,-2.50000E-01,-1.90000E-01,-2.00000E-01,-2.00000E-01,3.25000E-01, 2.80000E-01, 2.33000E-01, 1.85000E-01, 1.62000E-01,1.36000E-01,8.50000E-02}));
    ASSERT_THAT(k.Kq, ElementsAreArray({-4.80000E-02,-3.30000E-02,-2.20000E-02,-2.50000E-02,-2.80000E-02,3.40000E-02, 3.26000E-02, 2.97000E-02, 2.55000E-02, 2.30000E-02, 2.040000E-02,1.50000E-02}));
}

TEST_F(KtKqForceModelTest, force)
{
    auto input = KtKqForceModel::parse(test_data::kt_kq());
    EnvironmentAndFrames env;
    env.rho = 1024;
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    const KtKqForceModel w(input, "", env);
    ASSERT_EQ("Kt(J) & Kq(J)", w.model_name());
    BodyStates states;
    states.u.record(0, 1);

    std::map<std::string,double> commands;
    commands["rpm"] = 5*(2*PI);

    ASSERT_NEAR(306063.03332753148, w.get_force(states, a.random<double>(),commands)(0), EPS);
    ASSERT_EQ(0, w.get_force(states, a.random<double>(),commands)(1));
    ASSERT_EQ(0, w.get_force(states, a.random<double>(),commands)(2));
    ASSERT_EQ(0, w.get_force(states, a.random<double>(),commands)(4));
    ASSERT_EQ(0, w.get_force(states, a.random<double>(),commands)(5));
}

TEST_F(KtKqForceModelTest, clarify_exception_message_for_Kt_Kq_interpolation_errors)
{
    auto input = KtKqForceModel::parse(test_data::kt_kq());
    EnvironmentAndFrames env;
    env.rho = 1024;
    env.rot = YamlRotation("angle", {"z","y'","x''"});
    const KtKqForceModel w(input, "", env);
    BodyStates states;
    states.u.record(0, 1);

    std::map<std::string,double> commands;
    commands["rpm"] = 2*PI*0.025;
    EXPECT_THROW( w.get_force(states, a.random<double>(),commands)(0), NumericalErrorException);
}

