/*
 * listenersTest.cpp
 *
 *  Created on: Oct 21, 2014
 *      Author: cady
 */

#include "listenersTest.hpp"
#include "listeners.hpp"
#include "yaml_data.hpp"

listenersTest::listenersTest() : a(ssc::random_data_generator::DataGenerator(54545))
{
}

listenersTest::~listenersTest()
{
}

void listenersTest::SetUp()
{
}

void listenersTest::TearDown()
{
}

TEST_F(listenersTest, example)
{
//! [listenersTest listen_to_file_example]
    auto ds = listen_to_file(test_data::controlled_forces());
    ds.check_in("listenersTest (example)");
    ds.set<double>("t", 0);
    ASSERT_DOUBLE_EQ(2500, ds.get<double>("port side propeller(rpm)"));
    ASSERT_DOUBLE_EQ(0.7, ds.get<double>("port side propeller(P/D)"));

    ds.set<double>("t", 0.5);
    ASSERT_DOUBLE_EQ(2750, ds.get<double>("port side propeller(rpm)"));
    ASSERT_DOUBLE_EQ(0.7, ds.get<double>("port side propeller(P/D)"));

    ds.set<double>("t", 1);
    ASSERT_DOUBLE_EQ(3000, ds.get<double>("port side propeller(rpm)"));
    ASSERT_DOUBLE_EQ(0.7, ds.get<double>("port side propeller(P/D)"));

    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        ds.set<double>("t", a.random<double>().greater_than(10));
        ASSERT_DOUBLE_EQ(4000, ds.get<double>("port side propeller(rpm)"));
        ASSERT_DOUBLE_EQ(0.7, ds.get<double>("port side propeller(P/D)"));
    }
    ds.check_out();
    //! [listenersTest listen_to_file_example]
}

