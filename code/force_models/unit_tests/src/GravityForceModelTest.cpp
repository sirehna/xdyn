/*
 * GravityForceModelTest.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "EnvironmentAndFrames.hpp"
#include "GravityForceModel.hpp"
#include "GravityForceModelTest.hpp"
#include "generate_body_for_tests.hpp"
#include <ssc/kinematics.hpp>

#define BODY "body 1"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

GravityForceModelTest::GravityForceModelTest() : a(ssc::random_data_generator::DataGenerator(45454))
{
}

GravityForceModelTest::~GravityForceModelTest()
{
}

void GravityForceModelTest::SetUp()
{
}

void GravityForceModelTest::TearDown()
{
}

TEST_F(GravityForceModelTest, example)
{
//! [GravityForceModelTest example]
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), BODY));
    GravityForceModel F(BODY, env);
    BodyStates states = get_body(BODY)->get_states();
    states.m = 100;

    const ssc::kinematics::Wrench f = F(states, a.random<double>());
//! [GravityForceModelTest example]
//! [GravityForceModelTest expected output]
    ASSERT_EQ(BODY, f.get_frame());
    ASSERT_DOUBLE_EQ(0, f.K());
    ASSERT_DOUBLE_EQ(0, f.M());
    ASSERT_DOUBLE_EQ(0, f.N());
    ASSERT_DOUBLE_EQ(0, f.X());
    ASSERT_DOUBLE_EQ(0, f.Y());
    ASSERT_DOUBLE_EQ(981, f.Z());
//! [GravityForceModelTest expected output]
}

/**
 * \code
    print("Maxima comands")$
    declare(alpha,scalar)$
    declare(phi,scalar)$
    declare(theta,scalar)$
    declare(psi,scalar)$
    declare(Fz,scalar)$
    RotX(alpha):=matrix([1,0,0],[0,cos(alpha),-sin(alpha)],[0,sin(alpha),cos(alpha)])$
    RotY(alpha):=matrix([cos(alpha),0,sin(alpha)],[0,1,0],[-sin(alpha),0,cos(alpha)])$
    RotZ(alpha):=matrix([cos(alpha),-sin(alpha),0],[sin(alpha),cos(alpha),0],[0,0,1])$
    CtmX(alpha):=transpose(RotX(alpha))$
    CtmY(alpha):=transpose(RotY(alpha))$
    CtmZ(alpha):=transpose(RotZ(alpha))$
    Euler321Ctm(phi,theta,psi):=trigsimp(CtmX(phi).CtmY(theta).CtmZ(psi));
    res=Euler321Ctm(%pi/2,%pi/6,%pi/3).matrix([0],[0],[Fz]);
    \endcode
 */
TEST_F(GravityForceModelTest, example_with_an_orientation)
{
    using namespace ssc::kinematics;
    const double EPS = 1e-11;
    const double cosPhi   = 0.0;
    const double cosTheta = 0.5*sqrt(3);
    const double cosPsi   = 0.5;
    const double sinPhi   = 1.0;
    const double sinTheta = 0.5;
    const double sinPsi   = 0.5*sqrt(3);
    RotationMatrix rot;
    rot << cosTheta*cosPsi, sinPhi*sinTheta*cosPsi-cosPhi*sinPsi, cosPhi*sinTheta*cosPsi+sinPhi*sinPsi,
           cosTheta*sinPsi, sinPhi*sinTheta*sinPsi+cosPhi*cosPsi, cosPhi*sinTheta*sinPsi-sinPhi*cosPsi,
           -sinTheta,       sinPhi*cosTheta,                      cosPhi*cosTheta;
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    env.k->add(Transform(rot, "NED", BODY));
    GravityForceModel F(BODY, env);
    BodyStates states = get_body(BODY)->get_states();
    states.m = 100;

    const Wrench f = F(states,a.random<double>());
    ASSERT_EQ(BODY, f.get_frame());
    ASSERT_NEAR(0, f.K(),EPS);
    ASSERT_NEAR(0, f.M(),EPS);
    ASSERT_NEAR(0, f.N(),EPS);
    ASSERT_NEAR(-0.5*981, f.X(),EPS);
    ASSERT_NEAR(+0.5*sqrt(3.0)*981, f.Y(),EPS);
    ASSERT_NEAR(0.0, f.Z(),EPS);
}

TEST_F(GravityForceModelTest, potential_energy)
{
    std::vector<double> x(3,0);
    using namespace ssc::kinematics;
    const double cosPhi   = 0.0;
    const double cosTheta = 0.5*sqrt(3);
    const double cosPsi   = 0.5;
    const double sinPhi   = 1.0;
    const double sinTheta = 0.5;
    const double sinPsi   = 0.5*sqrt(3);
    RotationMatrix rot;
    rot << cosTheta*cosPsi, sinPhi*sinTheta*cosPsi-cosPhi*sinPsi, cosPhi*sinTheta*cosPsi+sinPhi*sinPsi,
           cosTheta*sinPsi, sinPhi*sinTheta*sinPsi+cosPhi*cosPsi, cosPhi*sinTheta*sinPsi-sinPhi*cosPsi,
           -sinTheta,       sinPhi*cosTheta,                      cosPhi*cosTheta;
    EnvironmentAndFrames env;
    env.g = 9.81;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), BODY));
    GravityForceModel F(BODY, env);
    BodyStates states = get_body(BODY)->get_states();
    states.m = 100;

    ASSERT_DOUBLE_EQ(0, F.potential_energy(states, x));
    x[2] = 123.;
    ASSERT_DOUBLE_EQ(-120663., F.potential_energy(states, x));
}
