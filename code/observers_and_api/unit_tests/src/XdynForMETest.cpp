/*
 * XdynForMETest.cpp
 *
 *  Created on: Nov 15, 2018
 *      Author: cady
 */


#include "yaml_data.hpp"


#include "XdynForME.hpp"
#include "XdynForMETest.hpp"
#define EPS 1E-8
#include <ssc/macros.hpp>
XdynForMETest::XdynForMETest() : a(ssc::random_data_generator::DataGenerator(123456789))
{
}

XdynForMETest::~XdynForMETest()
{
}

void XdynForMETest::SetUp()
{
}

void XdynForMETest::TearDown()
{
}

TEST_F(XdynForMETest, test_falling_ball_with_yaml)
{
//! [XdynForMETest example]

    const double g = 9.81;
    const std::string yaml = test_data::falling_ball_example();

    XdynForME xdyn_for_me(yaml);
    const std::string input_yaml = test_data::complete_yaml_message_for_falling_ball();
    SimServerInputs server_inputs = parse_SimServerInputs(input_yaml, xdyn_for_me.get_Tmax());
    const std::vector<double> dx_dt = xdyn_for_me.calculate_dx_dt(server_inputs);


//! [XdynForMETest example]
//! [XdynForMETest expected output]
    COUT(dx_dt);
    ASSERT_EQ(13, dx_dt.size());
    ASSERT_NEAR(1,              dx_dt[0], EPS);
    ASSERT_NEAR(0,                       dx_dt[1], EPS);
    ASSERT_NEAR(0,dx_dt[2], EPS);
    ASSERT_NEAR(0,                       dx_dt[3], EPS);
    ASSERT_NEAR(0.0,                       dx_dt[4], EPS);
    ASSERT_NEAR(g,               dx_dt[5], EPS);
    ASSERT_NEAR(0,                         dx_dt[6], EPS);
    ASSERT_NEAR(0,                         dx_dt[7], EPS);
    ASSERT_NEAR(0,                         dx_dt[8], EPS);
    ASSERT_NEAR(0,                         dx_dt[9], EPS);
    ASSERT_NEAR(0,                         dx_dt[10], EPS);
    ASSERT_NEAR(0,                         dx_dt[11], EPS);
    ASSERT_NEAR(0,                         dx_dt[12], EPS);
//! [XdynForMETest expected output]
}
