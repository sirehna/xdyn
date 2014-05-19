/*
 * kahan_sumTest.cpp
 *
 *  Created on: May 19, 2014
 *      Author: cady
 */

#include "kahan_sumTest.hpp"
#include "kahan_sum.hpp"

#include <functional> // std::plus
#include <numeric>    // std::accumulate

kahan_sumTest::kahan_sumTest() : a(DataGenerator(54212))
{
}

kahan_sumTest::~kahan_sumTest()
{
}

void kahan_sumTest::SetUp()
{
}

void kahan_sumTest::TearDown()
{
}

TEST_F(kahan_sumTest, example)
{
//! [kahan_sumTest example]
    const std::vector<int> v = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
//! [kahan_sumTest example]
//! [kahan_sumTest expected output]
    ASSERT_EQ(105, sum::kahan(v));
//! [kahan_sumTest expected output]
}

TEST_F(kahan_sumTest, example_from_wikipedia)
{
    const std::vector<double> v = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7};
    const double naive = std::accumulate (v.begin(),v.end(),0,std::plus<double>());
    const double kahan = sum::kahan(v);
    ASSERT_NEAR(8,naive, 1E-1);
    ASSERT_NEAR(15.3,kahan, 1E-1);
}


