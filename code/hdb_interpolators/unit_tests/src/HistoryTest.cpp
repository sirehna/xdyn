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
    const double Tmax = a.random<double>().between(0,100);
    const double t_lower_than_Tmax = a.random<double>().between(0,Tmax);
    const double t_greater_than_Tmax = a.random<double>().greater_than(Tmax);
    History h(Tmax);
    const double t0 = a.random<double>().between(Tmax,10*Tmax);
    h.record(t0, a.random<double>());
    h.record(t0+Tmax, a.random<double>());
    ASSERT_NO_THROW(h(t_lower_than_Tmax));
    ASSERT_NO_THROW(h(Tmax));
    ASSERT_THROW(h(t_greater_than_Tmax), HistoryException);
}

TEST_F(HistoryTest, cannot_retrieve_anything_if_history_is_empty)
{
    History h;
    ASSERT_THROW(h(0), HistoryException);
}

TEST_F(HistoryTest, can_retrieve_initial_values)
{
    History h(2);
    const double t0 = a.random<double>();
    h.record(t0-2, 1);
    h.record(t0-1, 2);
    h.record(t0, 3);

    ASSERT_DOUBLE_EQ(3, h(0));
    ASSERT_DOUBLE_EQ(2, h(1));
    ASSERT_DOUBLE_EQ(1, h(2));
}

TEST_F(HistoryTest, cannot_retrieve_value_in_the_future)
{
    History h;
    const double t0 = a.random<double>();
    h.record(t0-2, 1);
    h.record(t0-1, 2);
    h.record(t0, 3);
    ASSERT_THROW(h(-1), HistoryException);
}

TEST_F(HistoryTest, linear_interpolation_should_be_accurate)
{
    //! [HistoryTest example]
    History h(1e3);
    h.record(421, 1);
    h.record(216, 277);
    h.record(420, 73);
    h.record(540, 239);
    h.record(24, 1);
    //! [HistoryTest example]
    //! [HistoryTest expected output]
    ASSERT_DOUBLE_EQ(1,  h(516));
    ASSERT_DOUBLE_EQ(73, h(120));
    ASSERT_DOUBLE_EQ(1,  h(119));
    ASSERT_DOUBLE_EQ(239,h(0));
    //! [HistoryTest expected output]
    ASSERT_DOUBLE_EQ(277,h(324));
    ASSERT_DOUBLE_EQ(24, h(500));
    ASSERT_DOUBLE_EQ(193,h(240));
    ASSERT_DOUBLE_EQ(37, h(119.5));
    ASSERT_DOUBLE_EQ(3,  h(118));
}

TEST_F(HistoryTest, can_get_size_of_history)
{
    const size_t N = a.random<size_t>().between(2,1000);
    History h(1e10);
    for (size_t i = 0 ; i < N ; ++i) h.record((double)i, a.random<double>());
    ASSERT_EQ(N, h.size());
}

TEST_F(HistoryTest, should_shift_history)
{
    History h(1E10);
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
    ASSERT_DOUBLE_EQ(11, h(13.5));

    h.record(30, 25);
    ASSERT_EQ(7, h.size());

    h.record(31, 26);
    ASSERT_EQ(8, h.size());
    ASSERT_DOUBLE_EQ(16, h(13.5));

    h.record(33, 27);
    ASSERT_EQ(9, h.size());
    ASSERT_DOUBLE_EQ(12.8, h(13.5));
}

TEST_F(HistoryTest, can_get_history_max_length)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        //! [HistoryTest get_Tmax_example]
        const double Tmax = a.random<double>().greater_than(0);
        History h(Tmax);
        const double t = a.random<double>().between(0,10);
        h.record(t, a.random<double>());
        h.record(t+Tmax, a.random<double>());
        ASSERT_DOUBLE_EQ(Tmax, h.get_Tmax());
        //! [HistoryTest get_Tmax_example]
    }
}

TEST_F(HistoryTest, can_get_history_length)
{
    //! [HistoryTest get_length_example]
    History h(1e10);
    h.record(421, 1);
    ASSERT_DOUBLE_EQ(0, h.get_duration());
    h.record(216, 277);
    ASSERT_DOUBLE_EQ(421-216, h.get_duration());
    h.record(420, 73);
    ASSERT_DOUBLE_EQ(421-216, h.get_duration());
    h.record(540, 239);
    ASSERT_DOUBLE_EQ(540-216, h.get_duration());
    h.record(24, 1);
    ASSERT_DOUBLE_EQ(516, h.get_duration());
    h.record(2400, 1);
    ASSERT_DOUBLE_EQ(2376, h.get_duration());
    //! [HistoryTest get_length_example]
}
#include <ssc/macros.hpp>
TEST_F(HistoryTest, history_should_not_grow_indefinitely)
{
    History h(4);
    const double t = 12;
    h.record(t-12, 1);
    h.record(t-9, 3);
    h.record(t-8, 2);
    ASSERT_DOUBLE_EQ(1, h(4));
    ASSERT_EQ(3, h.size());
    ASSERT_DOUBLE_EQ(4, h.get_duration());
    ASSERT_THROW(h(4.1), HistoryException);
    h.record(t-5, 4);
    ASSERT_EQ(3, h.size());
    ASSERT_DOUBLE_EQ(4, h.get_duration());
    ASSERT_DOUBLE_EQ(3, h(4));
    h.record(t-4, 2);
    h.record(t-2, 7);
    h.record(t-1, 6);
    ASSERT_EQ(4, h.size());
    ASSERT_DOUBLE_EQ(4, h.get_duration());
    h.record(t, 8);
    ASSERT_DOUBLE_EQ(2, h(4));
    ASSERT_EQ(4, h.size());
    ASSERT_DOUBLE_EQ(4, h.get_duration());
}

TEST_F(HistoryTest, interpolation_should_be_OK_after_shift)
{
    History h(3);
    h.record(-10, 13);
    h.record(0, 23);
    ASSERT_DOUBLE_EQ(20., h(3));
}

TEST_F(HistoryTest, can_calculate_average_value_for_one_point)
{
    History h(2);
    h.record(0,1);
    ASSERT_DOUBLE_EQ(1, h.average(1));
}

TEST_F(HistoryTest, should_throw_if_length_of_average_is_negative)
{
    History h(2);
    h.record(0,1);
    h.record(2,1);
    ASSERT_THROW(h.average(-1), HistoryException);
}

TEST_F(HistoryTest, can_retrieve_average_for_two_values)
{
    History h(2);
    h.record(0,0);
    h.record(2,2);
    ASSERT_NEAR(1, h.average(2), 1E-3);
}

TEST_F(HistoryTest, full_average_test)
{
    History h(4);
    const double t = 12;
    h.record(t-12, 1);
    h.record(t-9, 3);
    h.record(t-8, 2);
    h.record(t-5, 4);
    ASSERT_DOUBLE_EQ(11.5/4., h.average(4));

    h.record(t-4, 2);
    h.record(t-4+4./5., 4);
    ASSERT_DOUBLE_EQ(3, h.average(1+4./5.));
    h.record(t-2, 7);
    h.record(t-1, 6);
    h.record(t, 8);
    ASSERT_DOUBLE_EQ(7, h.average(1));
    ASSERT_DOUBLE_EQ(7.5, h.average(0.5));
}

TEST_F(HistoryTest, average_is_instant_value_when_length_is_zero)
{
    History h(2);
    h.record(0,1);
    h.record(2,2);
    ASSERT_DOUBLE_EQ(2, h.average(0));
}

TEST_F(HistoryTest, can_ask_for_average_even_if_history_is_not_yet_long_enough)
{
    History h(4);
    const double t = 12;
    h.record(t-12, 1);
    ASSERT_DOUBLE_EQ(1, h.average(4));
    h.record(t-9, 3);
    ASSERT_DOUBLE_EQ(2, h.average(4));
    h.record(t-8, 2);
    ASSERT_DOUBLE_EQ(2.125, h.average(4));
}

TEST_F(HistoryTest, average_value_of_an_empty_history_is_simply_zero)
{
    ASSERT_DOUBLE_EQ(0, History().average(1));
}
