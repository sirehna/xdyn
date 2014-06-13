/*
 * TransformTest.cpp
 *
 *  Created on: 8 avr. 2014
 *      Author: cec
 */

#include "TransformTest.hpp"
#include "extra_test_assertions.hpp"
#include "KinematicsException.hpp"
#include "random_kinematics.hpp"
#include "rotation_matrix_builders.hpp"

#include <cmath> // For atan

#define PI (4.*atan(1.))

#define EPS 1E-13

TransformTest::TransformTest() : a(DataGenerator(1215))
{
}

TransformTest::~TransformTest()
{
}

void TransformTest::SetUp()
{
}

void TransformTest::TearDown()
{
}

TEST_F(TransformTest, can_translate_a_point)
{
//! [TransformTest example]
    for (size_t i = 0 ; i < 20 ; ++i)
    {
        const Point P1 = random_point(a);
        const Point P2 = random_point_in_frame(a, P1.get_frame());
        const kinematics::Transform T(P1, a.random<std::string>());
        const Point Q = T*P2;

        ASSERT_SMALL_RELATIVE_ERROR(P1.x+P2.x,Q.x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(P1.y+P2.y,Q.y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(P1.z+P2.z,Q.z,EPS);
    }
//! [TransformTest example]
//! [TransformTest expected output]
//! [TransformTest expected output]
}

TEST_F(TransformTest, can_compose_two_translations)
{
    for (size_t i = 0 ; i < 20 ; ++i)
    {
        const std::string middle_frame = a.random<std::string>();
        const Point P1 = random_point(a);
        const Point P2 = random_point_in_frame(a, middle_frame);
        const kinematics::Transform T1(P1,middle_frame);
        const kinematics::Transform T2(P2,a.random<std::string>());
        const Point Q = T2*T1*P1;

        ASSERT_SMALL_RELATIVE_ERROR(2*P1.x+P2.x,Q.x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(2*P1.y+P2.y,Q.y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(2*P1.z+P2.z,Q.z,EPS);
    }
}

TEST_F(TransformTest, can_compose_two_translations_for_a_point_matrix)
{
    for (size_t i = 0 ; i < 20 ; ++i)
    {
        const std::string middle_frame = a.random<std::string>();
        const Point P1 = random_point(a);
        const Point P2 = random_point_in_frame(a, middle_frame);
        const kinematics::Transform T1(P1,middle_frame);
        const kinematics::Transform T2(P2,a.random<std::string>());
        const PointMatrix PC1 = random_point_matrix_in_frame(a, P1.get_frame());
        const PointMatrix Q = T2*T1*PC1;
        ASSERT_EQ(T2.get_to_frame(),Q.get_frame());
        ASSERT_EQ(3,Q.m.rows());
        ASSERT_EQ(PC1.m.cols(),Q.m.cols());
        for (size_t i=0;i<(size_t)(Q.m.cols());++i)
        {
            ASSERT_SMALL_RELATIVE_ERROR(P1.x+P2.x+PC1.m(0,i),Q.m(0,i),EPS);
            ASSERT_SMALL_RELATIVE_ERROR(P1.y+P2.y+PC1.m(1,i),Q.m(1,i),EPS);
            ASSERT_SMALL_RELATIVE_ERROR(P1.z+P2.z+PC1.m(2,i),Q.m(2,i),EPS);
        }
    }
}

TEST_F(TransformTest, can_compose_two_translations_and_two_rotations)
{
    const double beta = PI/3;
    for (size_t i = 0 ; i < 20 ; ++i)
    {
        const std::string A = a.random<std::string>();
        const std::string B = a.random<std::string>();
        const std::string C = a.random<std::string>();
        const Point P = random_point_in_frame(a, A);
        const Point ta = random_point_in_frame(a, A);
        const auto Ra = kinematics::rot(1,0,0,beta);
        const Point tb = random_point_in_frame(a, B);
        const auto Rb = kinematics::rot(0,1,0,beta);
        const kinematics::Transform bTa(ta,Ra,B);
        const kinematics::Transform cTb(tb,Rb,C);
        const Point Pc = cTb*bTa*P;
        ASSERT_EQ(C, Pc.get_frame());
        ASSERT_SMALL_RELATIVE_ERROR((P.x+ta.x)/2.0+(3.0*P.y+sqrt(3.0)*P.z)/4.0+sqrt(3.0)*ta.z/2.0+tb.x,Pc.x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR((P.y-sqrt(3.0)*P.z)/2.0+ta.y+tb.y,Pc.y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR((P.y*sqrt(3.0)+P.z)/4.0+ta.z/2.0-(sqrt(3.0)/2.0)*(P.x+ta.x)+tb.z,Pc.z,EPS);
    }
}

TEST_F(TransformTest, can_rotate_a_point)
{
    for (size_t i = 0 ; i < 20 ; ++i)
    {
        const double beta = a.random<double>().between(-PI,PI);
        const RotationMatrix R = kinematics::rot(0,0,1, beta);
        const Point P = random_point(a);
        const kinematics::Transform T(R, P.get_frame(), a.random<std::string>());
        const Point Q = T*P;

        ASSERT_SMALL_RELATIVE_ERROR(cos(beta)*P.x-sin(beta)*P.y,Q.x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(sin(beta)*P.x+cos(beta)*P.y,Q.y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(P.z,Q.z,EPS);
    }
}

TEST_F(TransformTest, can_compose_two_rotations)
{
    for (size_t i = 0 ; i < 20 ; ++i)
    {
        const double beta1 = a.random<double>().between(-PI,PI);
        const double beta2 = a.random<double>().between(-PI,PI);
        const RotationMatrix R1 = kinematics::rot(0,0,1, beta1);
        const RotationMatrix R2 = kinematics::rot(0,0,1, beta2);
        const Point P = random_point(a);
        const std::string middle_frame = a.random<std::string>();
        const kinematics::Transform T1(R1, P.get_frame(), middle_frame);
        const kinematics::Transform T2(R2, middle_frame, a.random<std::string>());
        const Point Q = T2*T1*P;

        ASSERT_SMALL_RELATIVE_ERROR(cos(beta1+beta2)*P.x-sin(beta1+beta2)*P.y,Q.x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(sin(beta1+beta2)*P.x+cos(beta1+beta2)*P.y,Q.y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(P.z,Q.z,EPS);
    }
}

TEST_F(TransformTest, can_rotate_and_translate_a_point)
{
    for (size_t i = 0 ; i < 20 ; ++i)
    {
        const Point O = random_point(a);
        const Point P = random_point_in_frame(a, O.get_frame());
        const double beta = a.random<double>().between(-PI,PI);
        const RotationMatrix R = kinematics::rot(0,0,1, beta);
        const kinematics::Transform T(O,R,a.random<std::string>());
        const Point Q = T*P;

        ASSERT_SMALL_RELATIVE_ERROR(O.x+cos(beta)*P.x-sin(beta)*P.y,Q.x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(O.y+sin(beta)*P.x+cos(beta)*P.y,Q.y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(O.z+P.z,Q.z,EPS);
    }
}

TEST_F(TransformTest, should_throw_if_applying_transform_to_a_point_in_wrong_frame)
{
    for (size_t i = 0 ; i < 20 ; ++i)
    {
        const std::string F1 = a.random<std::string>();
        const std::string F2 = a.random<std::string>();
        const Point P1 = random_point_in_frame(a, F1);
        const Point P2 = random_point_in_frame(a, F2);
        const RotationMatrix R = a.random<RotationMatrix>();
        const kinematics::Transform T1(P1,R,a.random<std::string>());
        const kinematics::Transform T2(P2,R,a.random<std::string>());
        ASSERT_NO_THROW(T1*P1);
        ASSERT_THROW(T1*P2, KinematicsException);
        ASSERT_NO_THROW(T2*P2);
        ASSERT_THROW(T2*P1, KinematicsException);
    }
}

TEST_F(TransformTest, should_throw_if_composing_transforms_in_wrong_frame)
{
    for (size_t i = 0 ; i < 20 ; ++i)
    {
        const std::string F1 = a.random<std::string>();
        const std::string F2 = a.random<std::string>();
        const Point O1 = random_point_in_frame(a, F1);
        const Point O2 = random_point_in_frame(a, F2);
        const RotationMatrix R = a.random<RotationMatrix>();
        const kinematics::Transform T1(O1,R,F2);
        const kinematics::Transform T2(O2,R,a.random<std::string>());
        ASSERT_NO_THROW(T2*T1);
        ASSERT_THROW(T1*T1, KinematicsException);
        ASSERT_THROW(T1*T2, KinematicsException);
        ASSERT_THROW(T2*T2, KinematicsException);
    }
}

TEST_F(TransformTest, should_throw_if_transforming_velocity_from_wrong_frame)
{
    for (size_t i = 0 ; i < 20 ; ++i)
    {
        const std::string F1 = a.random<std::string>();
        const std::string F2 = a.random<std::string>();
        const Point P = random_point_in_frame(a, F1);
        const Point Q = random_point_in_frame(a, F2);
        const AngularVelocityVector w = random_point_in_frame(a, F1);
        const RotationMatrix R = a.random<RotationMatrix>();
        const kinematics::Transform T_same(P,R,a.random<std::string>());
        const kinematics::Transform T_different(Q,R,a.random<std::string>());
        const Velocity V(P, w);
        ASSERT_NO_THROW(T_same*V);
        ASSERT_THROW(T_different*V, KinematicsException);
    }
}

TEST_F(TransformTest, can_project_velocity_in_another_frame)
{
    const std::string F1 = a.random<std::string>();
    const std::string F2 = a.random<std::string>();
    const Point P(F1, 1, 2, 3);
    const double beta = a.random<double>().between(-PI,PI);
    const AngularVelocityVector w(F1, 7, 1, 89);
    const TranslationVelocityVector t(F1, 10,11,12);
    const Point Q(F1, 5,2,7);
    const RotationMatrix R = kinematics::rot(1,0,0, beta);
    const kinematics::Transform T(Q,R,F2);
    const Velocity V1(P, t, w);
    const Velocity V2 = T*V1;

    ASSERT_SMALL_RELATIVE_ERROR(Q.x+V1.u, V2.u, EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Q.y+cos(beta)*V1.v-sin(beta)*V1.w, V2.v, EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Q.z+sin(beta)*V1.v+cos(beta)*V1.w, V2.w, EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Q.x+V1.p, V2.p, EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Q.y+cos(beta)*V1.q-sin(beta)*V1.r, V2.q, EPS);
    ASSERT_SMALL_RELATIVE_ERROR(Q.z+sin(beta)*V1.q+cos(beta)*V1.r, V2.r, EPS);
}

TEST_F(TransformTest, can_compute_the_inverse_transform)
{
    for (size_t i = 0 ; i < 20 ; ++i)
    {
        const std::string frame_a = a.random<std::string>();
        const std::string frame_b = a.random<std::string>();
        const Point Pa = random_point_in_frame(a, frame_a);
        const Point Pb = random_point_in_frame(a, frame_b);
        const auto bTa = random_transform(a, frame_a, frame_b);
        const kinematics::Transform aTb = bTa.inverse();
        ASSERT_SMALL_RELATIVE_ERROR(Pa.x,((aTb*bTa)*Pa).x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(Pa.y,((aTb*bTa)*Pa).y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(Pa.z,((aTb*bTa)*Pa).z,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(Pb.x,((bTa*aTb)*Pb).x,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(Pb.y,((bTa*aTb)*Pb).y,EPS);
        ASSERT_SMALL_RELATIVE_ERROR(Pb.z,((bTa*aTb)*Pb).z,EPS);
    }
}
