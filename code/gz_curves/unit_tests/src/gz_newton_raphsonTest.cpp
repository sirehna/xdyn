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

#define EPS 1E-15

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
    ret.state(1) = SQ(X(1))*SQ(X(1)) - 4;
    ret.state(2) = CB(X(2)) - 3;
    return ret;
}

Eigen::Matrix2d K(const Eigen::Vector3d& X);
Eigen::Matrix2d K(const Eigen::Vector3d& X)
{
    Eigen::Matrix2d ret = Eigen::Matrix2d::Zero();
    ret(0,0) = 2*X(0);
    ret(1,1) = 3*SQ(X(2));
    return ret;
}
#include <ssc/macros.hpp>
TEST_F(gz_newton_raphsonTest, example)
{
//! [gz_newton_raphsonTest example]
    const double phi_eq = a.random<double>();
    const GZ::State X0(4,phi_eq,6);
//! [gz_newton_raphsonTest example]
//! [gz_newton_raphsonTest expected output]
    const auto Xeq = GZ::newton_raphson(X0, f, K, 10, 1000*std::numeric_limits<double>::epsilon());
    ASSERT_NEAR(sqrt(2), (double)Xeq(0), EPS);
    ASSERT_DOUBLE_EQ(phi_eq, (double)Xeq(1));
    ASSERT_NEAR(std::pow(3, 1./3.), (double)Xeq(2), EPS);
//! [gz_newton_raphsonTest expected output]
}

