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

TEST_F(HistoryTest, throws_if_retrieving_value_too_far_in_the_past)
{
    const double Tmax = a.random<double>().greater_than(0);
    const double t_lower_than_Tmax = a.random<double>().between(0,Tmax);
    const double t_greater_than_Tmax = a.random<double>().greater_than(Tmax);
    History<double> h(Tmax);
    h.record(a.random<double>(), a.random<double>());
    h.get(t_lower_than_Tmax);
    ASSERT_NO_THROW(h.get(t_lower_than_Tmax));
    ASSERT_NO_THROW(h.get(Tmax));
    ASSERT_THROW(h.get(t_greater_than_Tmax), HistoryException);
}

TEST_F(HistoryTest, constructor_should_throw_if_Tmax_is_negative)
{
    const double positive_Tmax = a.random<double>().greater_than(0);
    ASSERT_NO_THROW(History<double> h(positive_Tmax));
    const double negative_Tmax = a.random<double>().no().greater_than(0);
    ASSERT_THROW(History<double> h(negative_Tmax), HistoryException);
    ASSERT_THROW(History<double> h(0), HistoryException);
}

TEST_F(HistoryTest, should_throw_if_recording_the_same_instant_twice)
{
    History<double> h(a.random<double>().greater_than(0));
    const double t = a.random<double>().greater_than(0);
    ASSERT_NO_THROW(h.record(t, a.random<double>()));
    ASSERT_THROW(h.record(t, a.random<double>()), HistoryException);
    ASSERT_NO_THROW(h.record(t+1, a.random<double>()));
    ASSERT_THROW(h.record(t+1, a.random<double>()), HistoryException);
}

TEST_F(HistoryTest, cannot_retrieve_anything_if_history_is_empty)
{
    History<double> h(a.random<double>().greater_than(0));
    ASSERT_THROW(History<double> h(0), HistoryException);
}

TEST_F(HistoryTest, can_retrieve_initial_values)
{
    History<double> h(3);
    const double t0 = a.random<double>();
    h.record(t0-2, 1);
    h.record(t0-1, 2);
    h.record(t0, 3);

    ASSERT_DOUBLE_EQ(3, h.get(0));
    ASSERT_DOUBLE_EQ(2, h.get(1));
    ASSERT_DOUBLE_EQ(1, h.get(2));
}

TEST_F(HistoryTest, cannot_retrieve_value_in_the_future)
{
    History<double> h(3);
    const double t0 = a.random<double>();
    h.record(t0-2, 1);
    h.record(t0-1, 2);
    h.record(t0, 3);
    ASSERT_THROW(h.get(-1), HistoryException);
}

TEST_F(HistoryTest, linear_interpolation_should_be_accurate)
{
    //! [HistoryTest example]
    History<double> h(516);
    h.record(421, 1);
    h.record(216, 277);
    h.record(420, 73);
    h.record(540, 239);
    h.record(24, 1);
    //! [HistoryTest example]
    //! [HistoryTest expected output]
    ASSERT_DOUBLE_EQ(1,  h.get(516));
    ASSERT_DOUBLE_EQ(73, h.get(120));
    ASSERT_DOUBLE_EQ(1,  h.get(119));
    ASSERT_DOUBLE_EQ(239,h.get(0));
    //! [HistoryTest expected output]
    ASSERT_DOUBLE_EQ(277,h.get(324));
    ASSERT_DOUBLE_EQ(24, h.get(500));
    ASSERT_DOUBLE_EQ(193,h.get(240));
    ASSERT_DOUBLE_EQ(37, h.get(119.5));
    ASSERT_DOUBLE_EQ(3,  h.get(118));
}

TEST_F(HistoryTest, can_get_size_of_history)
{
    const size_t N = a.random<size_t>().between(2,1000);
    History<double> h((double)N);
    for (size_t i = 0 ; i < N ; ++i) h.record((double)i, a.random<double>());
    ASSERT_EQ(N, h.size());
}

TEST_F(HistoryTest, should_erase_old_elements)
{
    const size_t N = a.random<size_t>().between(2,1000);
    History<double> h((double)N);
    for (size_t i = 0 ; i < N ; ++i) h.record((double)i, a.random<double>());
    for (size_t i = N ; i < 2*N ; ++i)
    {
        h.record((double)i, a.random<double>());
        ASSERT_EQ(N+1, h.size());
    }
}

TEST_F(HistoryTest, should_shift_history)
{
    History<double> h(13.5);
    h.record(10, 10);
    ASSERT_EQ(1, h.size());

    h.record(15, 20);
    ASSERT_EQ(2, h.size());

    h.record(20, 12);
    ASSERT_EQ(3, h.size());

    h.record(21, 14);
    ASSERT_EQ(4, h.size());

    h.record(22, 13);
    ASSERT_EQ(5, h.size());

    h.record(24, 16);
    ASSERT_EQ(6, h.size());
    ASSERT_DOUBLE_EQ(11, h.get(13.5));

    h.record(30, 25);
    ASSERT_EQ(6, h.size());

    h.record(31, 26);
    ASSERT_EQ(7, h.size());
    ASSERT_DOUBLE_EQ(16, h.get(13.5));

    h.record(33, 27);
    ASSERT_EQ(8, h.size());
    ASSERT_DOUBLE_EQ(12.8, h.get(13.5));
}
