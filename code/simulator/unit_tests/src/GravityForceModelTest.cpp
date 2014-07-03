/*
 * GravityForceModelTest.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "GravityForceModel.hpp"
#include "GravityForceModelTest.hpp"
#include "generate_body_for_tests.hpp"
#include "Transform.hpp"
#include "RotationMatrix.hpp"

#define BODY "body 1"
#define PI (4.0*atan(1.0))

GravityForceModelTest::GravityForceModelTest() : a(DataGenerator(45454))
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
    GravityForceModel::Input input;
    input.g = 9.81;
    input.k = KinematicsPtr(new Kinematics());
    input.k->add(kinematics::Transform(Point("NED"), BODY));
    GravityForceModel F(input);
    Body b = get_body(BODY);
    b.m = 100;
    const Wrench f = F(b);
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
    using namespace kinematics;
    const double EPS = 1e-11;
    const double cPhi   = 0.0;
    const double cTheta = 0.5*sqrt(3);
    const double cPsi   = 0.5;
    const double sPhi   = 1.0;
    const double sTheta = 0.5;
    const double sPsi   = 0.5*sqrt(3);
    RotationMatrix rot;
    rot << cTheta*cPsi, sPhi*sTheta*cPsi-cPhi*sPsi, cPhi*sTheta*cPsi+sPhi*sPsi,
           cTheta*sPsi, sPhi*sTheta*sPsi+cPhi*cPsi, cPhi*sTheta*sPsi-sPhi*cPsi,
           -sTheta,     sPhi*cTheta,                cPhi*cTheta;
    GravityForceModel::Input input;
    input.g = 9.81;
    input.k = KinematicsPtr(new Kinematics());
    input.k->add(kinematics::Transform(rot, "NED", BODY));
    GravityForceModel F(input);
    Body b = get_body(BODY);
    b.m = 100;
    const Wrench f = F(b);
    ASSERT_EQ(BODY, f.get_frame());
    ASSERT_DOUBLE_EQ(0, f.K());
    ASSERT_DOUBLE_EQ(0, f.M());
    ASSERT_DOUBLE_EQ(0, f.N());
    ASSERT_NEAR(-0.5*981, f.X(),EPS);
    ASSERT_NEAR(+0.5*sqrt(3.0)*981, f.Y(),EPS);
    ASSERT_DOUBLE_EQ(0.0, f.Z());
}
