/*
 * WaveNumberFunctorTest.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#include "WaveNumberFunctorTest.hpp"
#include "WaveNumberFunctor.hpp"
#include <tuple>

WaveNumberFunctorTest::WaveNumberFunctorTest() : a(ssc::random_data_generator::DataGenerator(54545))
{
}

WaveNumberFunctorTest::~WaveNumberFunctorTest()
{
}

void WaveNumberFunctorTest::SetUp()
{
}

void WaveNumberFunctorTest::TearDown()
{
}

TEST_F(WaveNumberFunctorTest, example)
{
    const double EPS = 1E-10;
//! [WaveNumberFunctorTest example]
    const WaveNumberFunctor F1(10, 0.3);
    std::tuple<double, double, double> T1 = F1(10);
    const WaveNumberFunctor F2(45, 7);
    std::tuple<double, double, double> T2 = F2(3);
//! [WaveNumberFunctorTest example]
//! [WaveNumberFunctorTest expected output]

    ASSERT_NEAR(98.10*tanh(100)-0.9e-1,std::get<0>(T1), EPS);
    ASSERT_NEAR(9.81*tanh(100)+981.00-981.00*tanh(100)*tanh(100),std::get<1>(T1), EPS);
    ASSERT_NEAR(196.20-196.20*tanh(100)*tanh(100)-19620.00*tanh(100)*(1-tanh(100)*tanh(100)),std::get<2>(T1), EPS);
    ASSERT_NEAR(0.2943e2 * tanh(0.135e3) - 0.49e2,std::get<0>(T2), EPS);
    ASSERT_NEAR(0.981e1 * tanh(0.135e3) + 0.132435e4 - 0.132435e4 * pow(tanh(0.135e3), 0.2e1),std::get<1>(T2), EPS);
    ASSERT_NEAR(0.88290e3 - 0.88290e3 * pow(tanh(0.135e3), 0.2e1) - 0.1191915000e6 * tanh(0.135e3) * (0.1e1 - pow(tanh(0.135e3), 0.2e1)),std::get<2>(T2), EPS);
//! [WaveNumberFunctorTest expected output]
}

