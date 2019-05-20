/*
 * listenersTest.cpp
 *
 *  Created on: Oct 21, 2014
 *      Author: cady
 */

#include "listenersTest.hpp"
#include "listeners.hpp"
#include "parse_commands.hpp"
#include "yaml_data.hpp"

#define EPS (1E-14)
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

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
    auto ds = make_command_listener(parse_command_yaml(test_data::controlled_forces()));
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
    const std::string commands =
          "  - name: controller\n"
          "    t: [0,1,3,10]\n"
          "    psi_co: {unit: rad, values: [0.25, 0.30, 0.40, 0]}\n";
    auto ds = make_command_listener(parse_command_yaml(test_data::controlled_forces()+commands));
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

TEST_F(listenersTest, bug_2961_can_have_a_single_value_for_commands)
{
    auto ds = make_command_listener(parse_command_yaml(test_data::bug_2961()));
    ds.check_in("listenersTest (bug_2961_can_have_a_single_value_for_commands)");
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        ds.set<double>("t", a.random<double>());
        ASSERT_NEAR(30*PI/180., ds.get<double>("controller(psi_co)"),EPS);
    }
    ds.check_out();
}
