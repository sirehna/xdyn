/*
 * listenersTest.cpp
 *
 *  Created on: Oct 21, 2014
 *      Author: cady
 */

#include "listenersTest.hpp"
#include "listeners.hpp"
#include "yaml_data.hpp"

#define EPS (1E-14)

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
    ASSERT_DOUBLE_EQ(3, ds.get<double>("propeller(rpm)"));
    ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("propeller(P/D)"));

    ds.set<double>("t", 0.5);
    ASSERT_DOUBLE_EQ(16.5, ds.get<double>("propeller(rpm)"));
    ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("propeller(P/D)"));

    ds.set<double>("t", 1);
    ASSERT_DOUBLE_EQ(30, ds.get<double>("propeller(rpm)"));
    ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("propeller(P/D)"));

    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        ds.set<double>("t", a.random<double>().greater_than(10));
        ASSERT_DOUBLE_EQ(40, ds.get<double>("propeller(rpm)"));
        ASSERT_DOUBLE_EQ(1.064935, ds.get<double>("propeller(P/D)"));
    }
    ds.check_out();
    //! [listenersTest listen_to_file_example]
}

TEST_F(listenersTest, can_parse_simple_track_keeping_commands)
{
    auto ds = listen_to_file(test_data::controlled_forces());
    ds.check_in("listenersTest (can_parse_simple_track_keeping_commands)");
    ds.set<double>("t", 0);
    ASSERT_NEAR(0.25, ds.get<double>("controller(psi_co)"),EPS);

    ds.set<double>("t", 0.5);
    ASSERT_NEAR(0.275, ds.get<double>("controller(psi_co)"),EPS);

    ds.set<double>("t", 1);
    ASSERT_NEAR(0.3, ds.get<double>("controller(psi_co)"),EPS);

    ds.set<double>("t", 2);
    ASSERT_NEAR(0.35, ds.get<double>("controller(psi_co)"),EPS);

    ds.set<double>("t", 3);
    ASSERT_NEAR(0.4, ds.get<double>("controller(psi_co)"),EPS);

    ds.set<double>("t", 5);
    ASSERT_NEAR(2./7., ds.get<double>("controller(psi_co)"),EPS);

    ds.set<double>("t", 10);
    ASSERT_NEAR(0, ds.get<double>("controller(psi_co)"), EPS);

    ds.set<double>("t", 100);
    ASSERT_NEAR(0, ds.get<double>("controller(psi_co)"), EPS);
    ds.check_out();
}
