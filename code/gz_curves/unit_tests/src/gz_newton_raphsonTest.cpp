/*
 * gz_newton_raphsonTest.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: cady
 */


#include "gz_newton_raphsonTest.hpp"
#include "gz_newton_raphson.hpp"

#define SQ(X) (X*X)
#define CB(X) (X*X*X)

#define EPS 1E-16

gz_newton_raphsonTest::gz_newton_raphsonTest() : a(ssc::random_data_generator::DataGenerator(54555))
{
}

gz_newton_raphsonTest::~gz_newton_raphsonTest()
{
}

void gz_newton_raphsonTest::SetUp()
{
}

void gz_newton_raphsonTest::TearDown()
{
}
GZ::Resultant f(const GZ::State& X);
GZ::Resultant f(const GZ::State& X)
{
    GZ::Resultant ret;
    ret.state(0) = SQ(X(0)) - 2;
    ret.state(1) = CB(X(1)) - 3;
    ret.state(2) = SQ(X(2))*SQ(X(2)) - 4;
    return ret;
}

Eigen::Matrix3d K(const Eigen::Vector3d& X);
Eigen::Matrix3d K(const Eigen::Vector3d& X)
{
    Eigen::Matrix3d ret = Eigen::Matrix3d::Zero();
    ret(0,0) = 2*X(0);
    ret(1,1) = 3*SQ(X(1));
    ret(2,2) = 4*CB(X(2));
    return ret;
}

TEST_F(gz_newton_raphsonTest, example)
{
//! [gz_newton_raphsonTest example]
    const GZ::State X0(4,5,6);
//! [gz_newton_raphsonTest example]
//! [gz_newton_raphsonTest expected output]
    const auto Xeq = GZ::newton_raphson(X0, f, K, 10, 1000*std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(sqrt(2), Xeq(0), EPS);
    ASSERT_NEAR(std::pow(3, 1./3.), Xeq(1), EPS);
    ASSERT_NEAR(std::pow(4, 1./4.), Xeq(2), EPS);
//! [gz_newton_raphsonTest expected output]
}

