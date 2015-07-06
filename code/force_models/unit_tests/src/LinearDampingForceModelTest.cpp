/*
 * LinearDampingForceModelTest.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: cady
 */
#include "generate_body_for_tests.hpp"
#include "LinearDampingForceModelTest.hpp"
#include "LinearDampingForceModel.hpp"
#include "EnvironmentAndFrames.hpp"

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

TEST_F(LinearDampingForceModelTest, parser)
{
    const std::string yaml = "        damping matrix at the center of gravity projected in the body frame:\n"
                             "            frame: body 1\n"
                             "            row 1: [  2,   3,   5,   7,  11,  13]\n"
                             "            row 2: [ 17,  19,  23,  29,  31,  37]\n"
                             "            row 3: [ 41,  43,  47,  53,  59,  61]\n"
                             "            row 4: [ 67,  71,  73,  79,  83,  89]\n"
                             "            row 5: [ 97, 101, 103, 107, 109, 113]\n"
                             "            row 6: [127, 131, 137, 139, 149, 151]\n";
    const Eigen::Matrix<double,6,6> D = LinearDampingForceModel::parse(yaml);
    ASSERT_DOUBLE_EQ(2.,(double)D(0,0));
    ASSERT_DOUBLE_EQ(3.,(double)D(0,1));
    ASSERT_DOUBLE_EQ(5.,(double)D(0,2));
    ASSERT_DOUBLE_EQ(7.,(double)D(0,3));
    ASSERT_DOUBLE_EQ(11.,(double)D(0,4));
    ASSERT_DOUBLE_EQ(13.,(double)D(0,5));
    ASSERT_DOUBLE_EQ(17.,(double)D(1,0));
    ASSERT_DOUBLE_EQ(19.,(double)D(1,1));
    ASSERT_DOUBLE_EQ(23.,(double)D(1,2));
    ASSERT_DOUBLE_EQ(29.,(double)D(1,3));
    ASSERT_DOUBLE_EQ(31.,(double)D(1,4));
    ASSERT_DOUBLE_EQ(37.,(double)D(1,5));
    ASSERT_DOUBLE_EQ(41.,(double)D(2,0));
    ASSERT_DOUBLE_EQ(43.,(double)D(2,1));
    ASSERT_DOUBLE_EQ(47.,(double)D(2,2));
    ASSERT_DOUBLE_EQ(53.,(double)D(2,3));
    ASSERT_DOUBLE_EQ(59.,(double)D(2,4));
    ASSERT_DOUBLE_EQ(61.,(double)D(2,5));
    ASSERT_DOUBLE_EQ(67.,(double)D(3,0));
    ASSERT_DOUBLE_EQ(71.,(double)D(3,1));
    ASSERT_DOUBLE_EQ(73.,(double)D(3,2));
    ASSERT_DOUBLE_EQ(79.,(double)D(3,3));
    ASSERT_DOUBLE_EQ(83.,(double)D(3,4));
    ASSERT_DOUBLE_EQ(89.,(double)D(3,5));
    ASSERT_DOUBLE_EQ(97.,(double)D(4,0));
    ASSERT_DOUBLE_EQ(101.,(double)D(4,1));
    ASSERT_DOUBLE_EQ(103.,(double)D(4,2));
    ASSERT_DOUBLE_EQ(107.,(double)D(4,3));
    ASSERT_DOUBLE_EQ(109.,(double)D(4,4));
    ASSERT_DOUBLE_EQ(113.,(double)D(4,5));
    ASSERT_DOUBLE_EQ(127.,(double)D(5,0));
    ASSERT_DOUBLE_EQ(131.,(double)D(5,1));
    ASSERT_DOUBLE_EQ(137.,(double)D(5,2));
    ASSERT_DOUBLE_EQ(139.,(double)D(5,3));
    ASSERT_DOUBLE_EQ(149.,(double)D(5,4));
    ASSERT_DOUBLE_EQ(151.,(double)D(5,5));
}

TEST_F(LinearDampingForceModelTest, example)
{
    //! [LinearDampingForceModelTest example]
    const double EPS = 1e-9;
    Eigen::Matrix<double,6,6> D;
    double u,v,w,p,q,r;
    EnvironmentAndFrames env;
    BodyStates states = get_body(BODY)->get_states();
    D <<  2,   3,   5,   7,  11,  13,
         17,  19,  23,  29,  31,  37,
         41,  43,  47,  53,  59,  61,
         67,  71,  73,  79,  83,  89,
         97, 101, 103, 107, 109, 113,
        127, 131, 137, 139, 149, 151;
    LinearDampingForceModel F(D,BODY, env);
    ASSERT_EQ("linear damping",F.model_name);
    for (size_t i=0;i<100;++i)
    {
        states.u.record(0, u = a.random<double>().between(-10.0,+10.0));
        states.v.record(0, v = a.random<double>().between(-10.0,+10.0));
        states.w.record(0, w = a.random<double>().between(-10.0,+10.0));
        states.p.record(0, p = a.random<double>().between(-10.0,+10.0));
        states.q.record(0, q = a.random<double>().between(-10.0,+10.0));
        states.r.record(0, r = a.random<double>().between(-10.0,+10.0));
        const ssc::kinematics::Wrench f = F(states,a.random<double>());
        ASSERT_EQ(BODY, f.get_frame());
        for (int j=0;j<3;++j)
        {
            const int k = j+3;
            ASSERT_NEAR(D(j,0)*states.u()+D(j,1)*states.v()+D(j,2)*states.w()+D(j,3)*states.p()+D(j,4)*states.q()+D(j,5)*states.r(),-f.force[j],EPS)<<" row: "<<i << ", col:"<<j;
            ASSERT_NEAR(D(k,0)*states.u()+D(k,1)*states.v()+D(k,2)*states.w()+D(k,3)*states.p()+D(k,4)*states.q()+D(k,5)*states.r(),-f.torque[j],EPS)<<" row: "<<i << ", col:"<<k;
        }
    }
    //! [LinearDampingForceModelTest example]
}
