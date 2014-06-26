/*
 * HydrostaticForceModuleTest.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "DefaultWaveModel.hpp"
#include "HydrostaticForceModel.hpp"
#include "HydrostaticForceModelTest.hpp"
#include "Kinematics.hpp"
#include "Transform.hpp"
#include "generate_body_for_tests.hpp"
#include "TriMeshTestData.hpp"

#define BODY "body 1"
#include "test_macros.hpp"
HydrostaticForceModelTest::HydrostaticForceModelTest() : a(DataGenerator(212154))
{
}

HydrostaticForceModelTest::~HydrostaticForceModelTest()
{
}

void HydrostaticForceModelTest::SetUp()
{
}

void HydrostaticForceModelTest::TearDown()
{
}

TEST_F(HydrostaticForceModelTest, example)
{
    HydrostaticForceModel::Input input;
    input.g = 9.81;
    input.rho = 1024;
    input.k = KinematicsPtr(new Kinematics());
    const Point G("NED",0,2,2./3.);
    input.k->add(kinematics::Transform(Point("NED"), "mesh(" BODY ")"));
    input.k->add(kinematics::Transform(Point("NED"), BODY));
    input.w = WaveModelPtr(new DefaultWaveModel(0));

    auto points = two_triangles();
    for (size_t i = 0 ; i < 2 ; ++i)
    {
        for (size_t j = 0 ; j < 3 ; ++j)
        {
            double x = points[i][j][0];
            points[i][j][0] = points[i][j][2];
            points[i][j][2] = points[i][j][1];
            points[i][j][1] = x;

        }
    }

    Body body = get_body(BODY, points);
    body.G = G;

    HydrostaticForceModel F(input);
    const Wrench Fhs = F(body);
//! [HydrostaticModuleTest example]
//! [HydrostaticModuleTest expected output]
    const double dz = 2./3;
    const double dS = 4;
    ASSERT_DOUBLE_EQ(-input.rho*input.g*dz*dS, Fhs.X());
    ASSERT_DOUBLE_EQ(0, Fhs.Y());
    ASSERT_DOUBLE_EQ(0, Fhs.Z());
    ASSERT_DOUBLE_EQ(0, Fhs.K());
    ASSERT_DOUBLE_EQ(0, Fhs.M());
    ASSERT_DOUBLE_EQ(0, Fhs.N());
}



