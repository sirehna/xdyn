/*
 * SimStepperTest.cpp
 *
 *  Created on: 13 oct. 2015
 *      Author: sirehna
 */


#include "SimStepperTest.hpp"
#include "SimStepper.hpp"
#include "yaml_data.hpp"
#include "ConfBuilder.hpp"
#include "ssc/macros/SerializeMapsSetsAndVectors.hpp"
#include <ssc/macros.hpp>
#define EPS 1E-8

SimStepperTest::SimStepperTest() : a(ssc::random_data_generator::DataGenerator(1234))
{
}

SimStepperTest::~SimStepperTest()
{
}

void SimStepperTest::SetUp()
{
}

void SimStepperTest::TearDown()
{
}

TEST_F(SimStepperTest, example)
{
//! [SimStepperTest example]
    const double g = 9.81;
    const std::string solver = "euler";
    const double dt = 1.0;
    const double t_start = 0;
    const double Dt = 10;
    const double t_end = t_start+Dt;
    std::map<std::string,double> u;

    ConfBuilder builder(test_data::falling_ball_example(), u);
    SimStepper simstepper(builder, solver, dt); // SimStepper's creator controls the stability of the numerical integration
    const double x0=4;
    const double y0=8;
    const double z0=12;
    const double u0 = 1;
    const double v0 = 0;
    const double w0 = 0;

    const State X0(AbstractStates<double>(x0, y0 ,z0 ,u0 ,v0 ,w0 ,0 ,0 ,0 ,1 ,0 ,0 ,0) ,t_start);

    const State next_X = simstepper.step(X0, t_start, Dt);
//! [SimStepperTest example]

//! [SimStepperTest expected output]
    ASSERT_NEAR(x0+u0*t_end,              next_X.x(), EPS);
    ASSERT_NEAR(y0,                       next_X.y(), EPS);
    ASSERT_NEAR(z0+g*t_end*(t_end-1.)/2., next_X.z(), EPS);
    ASSERT_NEAR(u0,                       next_X.u(), EPS);
    ASSERT_NEAR(v0,                       next_X.v(), EPS);
    ASSERT_NEAR(w0+g*t_end,               next_X.w(), EPS);
    ASSERT_NEAR(0,                        next_X.p(), EPS);
    ASSERT_NEAR(0,                        next_X.q(), EPS);
    ASSERT_NEAR(0,                        next_X.r(), EPS);
    ASSERT_NEAR(1,                        next_X.qr(), EPS);
    ASSERT_NEAR(0,                        next_X.qi(), EPS);
    ASSERT_NEAR(0,                        next_X.qj(), EPS);
    ASSERT_NEAR(0,                        next_X.qk(), EPS);
//! [SimStepperTest expected output]
}
