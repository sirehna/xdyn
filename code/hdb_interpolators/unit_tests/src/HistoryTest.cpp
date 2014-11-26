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
    ASSERT_NO_THROW(h.get(Tmax));
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

TEST_F(HistoryTest, should_throw_if_recording_the_same_instant_twice)
{
    History h(a.random<double>().greater_than(0));
    const double t = a.random<double>().greater_than(0);
    ASSERT_NO_THROW(h.record(t, a.random<double>()));
    ASSERT_THROW(h.record(t, a.random<double>()), HistoryException);
}

TEST_F(HistoryTest, can_retrieve_initial_values)
{
    History h(3);
    const double t0 = a.random<double>();
    h.record(t0-2, 1);
    h.record(t0-1, 2);
    h.record(t0, 3);
}
