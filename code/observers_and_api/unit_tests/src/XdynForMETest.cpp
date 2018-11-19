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


TEST_F(XdynForMETest, complete_test_with_commands_and_delay)
{
    const std::string yaml = test_data::simserver_test_with_commands_and_delay();
    XdynForME xdyn_for_me(yaml);
    const std::string input_yaml =
                "{\"Dt\": 10.0,\n"
                "\"states\":\n"
                "[ {\"t\": 0.0, \"x\": 4.0,  \"y\": 8.0, \"z\": 12.0, \"u\": 1.0, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 1.0, \"x\": 5.0,  \"y\": 7.0, \"z\": 13.0, \"u\": 1.1, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 2.0, \"x\": 6.0,  \"y\": 6.0, \"z\": 14.0, \"u\": 1.2, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 3.0, \"x\": 7.0,  \"y\": 5.0, \"z\": 15.0, \"u\": 1.3, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 4.0, \"x\": 8.0,  \"y\": 4.0, \"z\": 16.0, \"u\": 1.4, \"v\": 0.23,\"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 5.0, \"x\": 9.0,  \"y\": 3.0, \"z\": 17.0, \"u\": 1.5, \"v\": 0.0, \"w\": 4.4, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 6.0, \"x\": 10.0, \"y\": 2.0, \"z\": 18.0, \"u\": 1.6, \"v\": 0.0, \"w\": 0.0, \"p\": 12.0,\"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 7.0, \"x\": 11.0, \"y\": 1.0, \"z\": 19.0, \"u\": 1.7, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.123, \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 8.0, \"x\": 12.0, \"y\": 0.0, \"z\": 12.1, \"u\": 1.8, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 9.0, \"x\": 13.0, \"y\": 1.0, \"z\": 12.2, \"u\": 1.9, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 10,  \"x\": 14.0, \"y\": 2.0, \"z\": 12.3, \"u\": 0.0, \"v\": 0.0, \"w\": 0.0, \"p\": 0,   \"q\": 0,     \"r\": 0,   \"qr\": 1.1, \"qi\": 2.2, \"qj\": 3.3, \"qk\": 4.4}\n"
                "],\n"
                "\"commands\": {\"F1(command1)\": 20, \"F1(a)\": 4.5, \"F1(b)\": 5.7}}";
    SimServerInputs server_inputs = parse_SimServerInputs(input_yaml, xdyn_for_me.get_Tmax());
    const std::vector<double> dx_dt = xdyn_for_me.calculate_dx_dt(server_inputs);

    ASSERT_EQ(13, dx_dt.size());
    ASSERT_NEAR(0,              dx_dt[0], EPS); // dx/dt = u
    ASSERT_NEAR(0,            dx_dt[1], EPS); // dy/dt = v
    ASSERT_NEAR(0,            dx_dt[2], EPS); // dz/dt = w
    ASSERT_NEAR(14,             dx_dt[3], EPS); // m du/dt = Fx
    ASSERT_NEAR(8,              dx_dt[4], EPS); // m dv/dt = Fy
    ASSERT_NEAR(20*12.3,        dx_dt[5], EPS); // m dw/dt = Fz
    ASSERT_NEAR(5.7*1.4,        dx_dt[6], EPS); // m OGx dp/dt = Mx
    ASSERT_NEAR(0.23 + 20*4.4 + 2*5.7*12 + 0.123/4.5,          dx_dt[7], EPS); // m OGy dq/dt = My
    ASSERT_NEAR(0,          dx_dt[8], EPS); // m OGz dr/dt = Mz
}

TEST_F(XdynForMETest, complete_test_with_commands_and_delay_just_test_quaternions)
{
//! [XdynForMETest example]

    const std::string yaml = test_data::simserver_test_with_commands_and_delay();
    XdynForME xdyn_for_me(yaml);
    const std::string input_yaml =
                "{\"Dt\": 10.0,\n"
                "\"states\":\n"
                "[ {\"t\": 0.0, \"x\": 4.0,  \"y\": 8.0, \"z\": 12.0, \"u\": 1.0, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 1.0, \"x\": 5.0,  \"y\": 7.0, \"z\": 13.0, \"u\": 1.1, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 2.0, \"x\": 6.0,  \"y\": 6.0, \"z\": 14.0, \"u\": 1.2, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 3.0, \"x\": 7.0,  \"y\": 5.0, \"z\": 15.0, \"u\": 1.3, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 4.0, \"x\": 8.0,  \"y\": 4.0, \"z\": 16.0, \"u\": 1.4, \"v\": 0.23,\"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 5.0, \"x\": 9.0,  \"y\": 3.0, \"z\": 17.0, \"u\": 1.5, \"v\": 0.0, \"w\": 4.4, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 6.0, \"x\": 10.0, \"y\": 2.0, \"z\": 18.0, \"u\": 1.6, \"v\": 0.0, \"w\": 0.0, \"p\": 12.0,\"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 7.0, \"x\": 11.0, \"y\": 1.0, \"z\": 19.0, \"u\": 1.7, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.123, \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 8.0, \"x\": 12.0, \"y\": 0.0, \"z\": 12.1, \"u\": 1.8, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 9.0, \"x\": 13.0, \"y\": 1.0, \"z\": 12.2, \"u\": 1.9, \"v\": 0.0, \"w\": 0.0, \"p\": 0.0, \"q\": 0.0,   \"r\": 0.0, \"qr\": 1.0, \"qi\": 0.0, \"qj\": 0.0, \"qk\": 0.0}\n"
                ", {\"t\": 10,  \"x\": 14.0, \"y\": 2.0, \"z\": 12.3, \"u\": 0.0, \"v\": 0.0, \"w\": 0.0, \"p\": 11,  \"q\": 22,    \"r\": 33,  \"qr\": 1.1, \"qi\": 2.2, \"qj\": 3.3, \"qk\": 4.4}\n"
                "],\n"
                "\"commands\": {\"F1(command1)\": 20, \"F1(a)\": 4.5, \"F1(b)\": 5.7}}";
    SimServerInputs server_inputs = parse_SimServerInputs(input_yaml, xdyn_for_me.get_Tmax());
    const std::vector<double> dx_dt = xdyn_for_me.calculate_dx_dt(server_inputs);

    // q1 = qr r + qi i + qj j + qk k
    // q2 =         p i +  q j +  r k
    // q1 = a1 r + b1 i + c1 j + d1 k
    // q2 = a2 r + b2 i + c2 j + d2 k
    // q1*q2 = (a1*a2 - b1*b2 - c1*c2 - d1*d2) r
    //       + (a1*b2 + b1*a2 + c1*d2 - d1*c2) i
    //       + (a1*c2 - b1*d2 + c1*a2 + d1*b2) j
    //       + (a1*d2 + b1*c2 - c1*b2 + d1*a2) k
    // dqr/dt = 0.5*(q1*q2)k
    // dqi/dt = 0.5*(q1*q2)r
    // dqj/dt = 0.5*(q1*q2)i
    // dqk/dt = 0.5*(q1*q2)j
    const double a1 = 1.1;
    const double b1 = 2.2;
    const double c1 = 3.3;
    const double d1 = 4.4;
    const double a2 = 0;
    const double b2 = 11;
    const double c2 = 22;
    const double d2 = 33;

    const double dqk_dt = 0.5*(a1*d2 + b1*c2 - c1*b2 + d1*a2);
    const double dqr_dt = 0.5*(a1*a2 - b1*b2 - c1*c2 - d1*d2);
    const double dqi_dt = 0.5*(a1*b2 + b1*a2 + c1*d2 - d1*c2);
    const double dqj_dt = 0.5*(a1*c2 - b1*d2 + c1*a2 + d1*b2);

    EXPECT_NEAR(dqr_dt,          dx_dt[9], EPS);
    EXPECT_NEAR(dqi_dt,          dx_dt[10], EPS);
    EXPECT_NEAR(dqj_dt,          dx_dt[11], EPS);
    EXPECT_NEAR(dqk_dt,          dx_dt[12], EPS);
}
