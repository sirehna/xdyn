/*
 * DampingForceModelTest.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#include "QuadraticDampingForceModel.hpp"
#include "QuadraticDampingForceModelTest.hpp"
#include "generate_body_for_tests.hpp"
#include "EnvironmentAndFrames.hpp"

#include <ssc/kinematics.hpp>

#define BODY "body 1"

QuadraticDampingForceModelTest::QuadraticDampingForceModelTest() : a(ssc::random_data_generator::DataGenerator(666))
{
}

QuadraticDampingForceModelTest::~QuadraticDampingForceModelTest()
{
}

void QuadraticDampingForceModelTest::SetUp()
{
}

void QuadraticDampingForceModelTest::TearDown()
{
}

namespace ssc
{
    namespace random_data_generator
    {
        template <> Eigen::Matrix<double,6,6> TypedScalarDataGenerator<Eigen::Matrix<double,6,6> >::get() const
        {
            Eigen::Matrix<double,6,6> ret;
            for (int i = 0 ; i < 6 ; ++i)
            {
                for (int j = 0 ; j < 6 ; ++j)
                {
                    ret(i,j) = random<double>();
                }
            }
            return ret;
        }
    }
}

TEST_F(QuadraticDampingForceModelTest, example_with_null_velocities)
{
//! [DampingForceModelTest example]
    const QuadraticDampingForceModel F(a.random<Eigen::Matrix<double,6,6> >(), EnvironmentAndFrames());
    Body b = get_body(BODY);
    const double t = a.random<double>();
    const ssc::kinematics::Wrench f = F(b.states,t);
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

TEST_F(QuadraticDampingForceModelTest, example_with_random_positive_velocities_and_identity_damping_matrix)
{
    const double EPS = 1e-11;
    const Eigen::Matrix<double,6,6> D = Eigen::Matrix<double,6,6>::Identity();
    QuadraticDampingForceModel F(D, EnvironmentAndFrames());
    double u,v,w,p,q,r;
    Body b = get_body(BODY);
    for (size_t i=0;i<100;++i)
    {
        b.states.u = u = a.random<double>().greater_than(0.0);
        b.states.v = v = a.random<double>().greater_than(0.0);
        b.states.w = w = a.random<double>().greater_than(0.0);
        b.states.p = p = a.random<double>().greater_than(0.0);
        b.states.q = q = a.random<double>().greater_than(0.0);
        b.states.r = r = a.random<double>().greater_than(0.0);
        const ssc::kinematics::Wrench f = F(b.states,a.random<double>());
        ASSERT_EQ(BODY, f.get_frame());
        ASSERT_NEAR(-u*u, f.X(),EPS);
        ASSERT_NEAR(-v*v, f.Y(),EPS);
        ASSERT_NEAR(-w*w, f.Z(),EPS);
        ASSERT_NEAR(-p*p, f.K(),EPS);
        ASSERT_NEAR(-q*q, f.M(),EPS);
        ASSERT_NEAR(-r*r, f.N(),EPS);
    }
}

TEST_F(QuadraticDampingForceModelTest, example_with_dense_damping_matrix)
{
    const double EPS = 1e-9;
    Eigen::Matrix<double,6,6> D;
    double u,v,w,p,q,r;
    double uu,vv,ww,pp,qq,rr;
    Body b = get_body(BODY);
    D <<  2,   3,   5,   7,  11,  13,
         17,  19,  23,  29,  31,  37,
         41,  43,  47,  53,  59,  61,
         67,  71,  73,  79,  83,  89,
         97, 101, 103, 107, 109, 113,
        127, 131, 137, 139, 149, 151;
    QuadraticDampingForceModel F(D, EnvironmentAndFrames());
    for (int i=0;i<100;++i)
    {
        b.states.u = u = a.random<double>().between(-10.0,+10.0);
        b.states.v = v = a.random<double>().between(-10.0,+10.0);
        b.states.w = w = a.random<double>().between(-10.0,+10.0);
        b.states.p = p = a.random<double>().between(-10.0,+10.0);
        b.states.q = q = a.random<double>().between(-10.0,+10.0);
        b.states.r = r = a.random<double>().between(-10.0,+10.0);
        uu = fabs(u)*u;
        vv = fabs(v)*v;
        ww = fabs(w)*w;
        pp = fabs(p)*p;
        qq = fabs(q)*q;
        rr = fabs(r)*r;
        const ssc::kinematics::Wrench f = F(b.states,a.random<double>());
        ASSERT_EQ(BODY, f.get_frame());
        for (int j=0;j<3;++j)
        {
            const int k = j+3;
            ASSERT_NEAR(D(j,0)*uu+D(j,1)*vv+D(j,2)*ww+D(j,3)*pp+D(j,4)*qq+D(j,5)*rr,-f.force[j],EPS)<<" row: "<<i << ", col:"<<j;
            ASSERT_NEAR(D(k,0)*uu+D(k,1)*vv+D(k,2)*ww+D(k,3)*pp+D(k,4)*qq+D(k,5)*rr,-f.torque[j],EPS)<<" row: "<<i << ", col:"<<k;
        }
    }
}

