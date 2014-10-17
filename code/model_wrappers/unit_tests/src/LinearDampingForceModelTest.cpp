/*
 * LinearDampingForceModelTest.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */
#include "generate_body_for_tests.hpp"
#include "LinearDampingForceModelTest.hpp"
#include "LinearDampingForceModel.hpp"

#define BODY "body 1"

LinearDampingForceModelTest::LinearDampingForceModelTest() : a(ssc::random_data_generator::DataGenerator(82512))
{
}

LinearDampingForceModelTest::~LinearDampingForceModelTest()
{
}

void LinearDampingForceModelTest::SetUp()
{
}

void LinearDampingForceModelTest::TearDown()
{
}

TEST_F(LinearDampingForceModelTest, example)
{
    //! [LinearDampingForceModelTest example]
    const double EPS = 1e-9;
    Eigen::Matrix<double,6,6> D;
    double u,v,w,p,q,r;
    Body b = get_body(BODY);
    D <<  2,   3,   5,   7,  11,  13,
         17,  19,  23,  29,  31,  37,
         41,  43,  47,  53,  59,  61,
         67,  71,  73,  79,  83,  89,
         97, 101, 103, 107, 109, 113,
        127, 131, 137, 139, 149, 151;
    LinearDampingForceModel F(D);
    for (size_t i=0;i<100;++i)
    {
        b.u = u = a.random<double>().between(-10.0,+10.0);
        b.v = v = a.random<double>().between(-10.0,+10.0);
        b.w = w = a.random<double>().between(-10.0,+10.0);
        b.p = p = a.random<double>().between(-10.0,+10.0);
        b.q = q = a.random<double>().between(-10.0,+10.0);
        b.r = r = a.random<double>().between(-10.0,+10.0);
        const ssc::kinematics::Wrench f = F(b,a.random<double>());
        ASSERT_EQ(BODY, f.get_frame());
        for (size_t j=0;j<3;++j)
        {
            const size_t k = j+3;
            ASSERT_NEAR(D(j,0)*b.u+D(j,1)*b.v+D(j,2)*b.w+D(j,3)*b.p+D(j,4)*b.q+D(j,5)*b.r,-f.force[j],EPS)<<" row: "<<i << ", col:"<<j;
            ASSERT_NEAR(D(k,0)*b.u+D(k,1)*b.v+D(k,2)*b.w+D(k,3)*b.p+D(k,4)*b.q+D(k,5)*b.r,-f.torque[j],EPS)<<" row: "<<i << ", col:"<<k;
        }
    }
    //! [LinearDampingForceModelTest example]
}



