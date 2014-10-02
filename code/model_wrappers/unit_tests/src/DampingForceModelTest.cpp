/*
 * DampingForceModelTest.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "DampingForceModel.hpp"
#include "DampingForceModelTest.hpp"
#include "generate_body_for_tests.hpp"
#include <ssc/kinematics.hpp>

#define BODY "body 1"

DampingForceModelTest::DampingForceModelTest() : a(ssc::random_data_generator::DataGenerator(666))
{
}

DampingForceModelTest::~DampingForceModelTest()
{
}

void DampingForceModelTest::SetUp()
{
}

void DampingForceModelTest::TearDown()
{
}

TEST_F(DampingForceModelTest, example)
{
//! [DampingForceModelTest example]
    DampingForceModel F;
    Body b = get_body(BODY);
    const ssc::kinematics::Wrench f = F(b,0.0);
//! [DampingForceModelTest example]
//! [DampingForceModelTest expected output]
    ASSERT_EQ(BODY, f.get_frame());
    ASSERT_DOUBLE_EQ(0, f.K());
    ASSERT_DOUBLE_EQ(0, f.M());
    ASSERT_DOUBLE_EQ(0, f.N());
    ASSERT_DOUBLE_EQ(0, f.X());
    ASSERT_DOUBLE_EQ(0, f.Y());
    ASSERT_DOUBLE_EQ(0, f.Z());
//! [DampingForceModelTest expected output]
}
/*
TEST_F(DampingForceModelTest, example_with_an_orientation)
{
    using namespace ssc::kinematics;
    const double EPS = 1e-11;


    Eigen::Matrix<double,6,6> Mrb = convert(rigid_body_inertia);
    Eigen::Matrix<double,6,6> Ma = convert(added_mass);
    if (fabs((Mrb+Ma).determinant())<1E-10)
    {
        std::stringstream ss;
        ss << "Unable to compute the inverse of the total inertia matrix (rigid body inertia + added mass): " << std::endl
           << "Mrb = " << std::endl
           << Mrb << std::endl
           << "Ma = " << std::endl
           << Ma << std::endl
           << "Mrb+Ma = " << std::endl
           << Mrb+Ma << std::endl;
        THROW(__PRETTY_FUNCTION__, BodyBuilderException, ss.str());
    }
    Eigen::Matrix<double,6,6> M_inv = (Mrb+Ma).inverse();
    body.inverse_of_the_total_inertia = MatrixPtr(new Eigen::Matrix<double,6,6>(M_inv));
    body.solid_body_inertia = MatrixPtr(new Eigen::Matrix<double,6,6>(Mrb));
    body.total_inertia = MatrixPtr(new Eigen::Matrix<double,6,6>(Mrb+Ma));

    input.g = 9.81;
    input.k = ssc::kinematics::KinematicsPtr(new Kinematics());
    input.k->add(Transform(rot, "NED", BODY));
    DampingForceModel F(input);
    Body b = get_body(BODY);
    b.m = 100;
    const Wrench f = F(b,a.random<double>());
    ASSERT_EQ(BODY, f.get_frame());
    ASSERT_NEAR(0, f.K(),EPS);
    ASSERT_NEAR(0, f.M(),EPS);
    ASSERT_NEAR(0, f.N(),EPS);
    ASSERT_NEAR(-0.5*981, f.X(),EPS);
    ASSERT_NEAR(+0.5*sqrt(3.0)*981, f.Y(),EPS);
    ASSERT_NEAR(0.0, f.Z(),EPS);
}
*/
