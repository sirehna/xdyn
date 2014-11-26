/*
 * HistoryTest.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: cady
 */


#include "History.hpp"
#include "HistoryTest.hpp"
#include "HistoryException.hpp"

HistoryTest::HistoryTest() : a(ssc::random_data_generator::DataGenerator(5422))
{
}

HistoryTest::~HistoryTest()
{
}

void HistoryTest::SetUp()
{
}

void HistoryTest::TearDown()
{
}

TEST_F(HistoryTest, example)
{
//! [HistoryTest example]
    History h(a.random<double>());
//! [HistoryTest example]
//! [HistoryTest expected output]
//! [HistoryTest expected output]
}

TEST_F(HistoryTest, throws_if_retrieving_value_too_far_in_the_past)
{
    const double Tmax = a.random<double>().greater_than(0);
    const double t_lower_than_Tmax = a.random<double>().no().greater_than(Tmax);
    const double t_greater_than_Tmax = a.random<double>().greater_than(Tmax);
    const History h(Tmax);
    ASSERT_NO_THROW(h.get(t_lower_than_Tmax));
    ASSERT_THROW(h.get(t_greater_than_Tmax), HistoryException);
}

TEST_F(HistoryTest, constructor_should_throw_if_Tmax_is_negative)
{
    const double positive_Tmax = a.random<double>().greater_than(0);
    ASSERT_NO_THROW(History h(positive_Tmax));
    const double negative_Tmax = a.random<double>().no().greater_than(0);
    ASSERT_THROW(History h(negative_Tmax), HistoryException);
    ASSERT_THROW(History h(0), HistoryException);
}
