/*
 * parse_historyTest.cpp
 *
 *  Created on: 23 oct. 2015
 *      Author: sirehna
 */


#include "parse_historyTest.hpp"
#include "YamlState.hpp"
#include "yaml_data.hpp"
#include "parse_history.hpp"
#include <vector>
#include <sstream>
#include <ssc/macros.hpp>

parse_historyTest::parse_historyTest() : a(ssc::random_data_generator::DataGenerator(42))
{
}

parse_historyTest::~parse_historyTest()
{
}

void parse_historyTest::SetUp()
{
}

void parse_historyTest::TearDown()
{
}


TEST_F(parse_historyTest, can_parse_simple_history)
{
//! [parse_historyTest can_parse_simple_history]
    double t_list[5] = {0,0.1,0.2,0.3,0.4};
    double x0=90, y0=91, z0=92, u0=93, v0=94, w0=95, p0=96, q0=97, r0=98, qr0=99, qi0=100, qj0=101, qk0=102;
    double state_step = -20;
    size_t size_test = 5;

    YamlSimServerInputs history = decode_YamlSimServerInputs(test_data::dummy_history());
    //! [parse_historyTest can_parse_simple_history]

    //! [parse_historyTest expected output]
    ASSERT_EQ(1234.5, history.Dt);
    ASSERT_EQ(size_test, history.states.size());

    for(size_t i = 0; i < size_test ; ++i)
    {
        ASSERT_EQ(t_list[i], history.states[i].t);
        ASSERT_EQ(x0+((double)i)*state_step, history.states[i].x);
        ASSERT_EQ(y0+((double)i)*state_step, history.states[i].y);
        ASSERT_EQ(z0+((double)i)*state_step, history.states[i].z);
        ASSERT_EQ(u0+((double)i)*state_step, history.states[i].u);
        ASSERT_EQ(v0+((double)i)*state_step, history.states[i].v);
        ASSERT_EQ(w0+((double)i)*state_step, history.states[i].w);
        ASSERT_EQ(p0+((double)i)*state_step, history.states[i].p);
        ASSERT_EQ(q0+((double)i)*state_step, history.states[i].q);
        ASSERT_EQ(r0+((double)i)*state_step, history.states[i].r);
        ASSERT_EQ(qr0+((double)i)*state_step, history.states[i].qr);
        ASSERT_EQ(qi0+((double)i)*state_step, history.states[i].qi);
        ASSERT_EQ(qj0+((double)i)*state_step, history.states[i].qj);
        ASSERT_EQ(qk0+((double)i)*state_step, history.states[i].qk);
    }

//! [parse_historyTest expected output]
}

TEST_F(parse_historyTest, translation_loop)
{
    ASSERT_EQ("[{\"t\": 0, \"x\": 90, \"y\": 91, \"z\": 92, \"u\": 93, \"v\": 94, \"w\": 95, \"p\": 96, \"q\": 97, \"r\": 98, \"qr\": 99, \"qi\": 100, \"qj\": 101, \"qk\": 102, \"phi\": 0, \"theta\": 0, \"psi\": 0, \"extra_observations\": {}},\n"
               "{\"t\": 0.10000000000000001, \"x\": 70, \"y\": 71, \"z\": 72, \"u\": 73, \"v\": 74, \"w\": 75, \"p\": 76, \"q\": 77, \"r\": 78, \"qr\": 79, \"qi\": 80, \"qj\": 81, \"qk\": 82, \"phi\": 0, \"theta\": 0, \"psi\": 0, \"extra_observations\": {}},\n"
               "{\"t\": 0.20000000000000001, \"x\": 50, \"y\": 51, \"z\": 52, \"u\": 53, \"v\": 54, \"w\": 55, \"p\": 56, \"q\": 57, \"r\": 58, \"qr\": 59, \"qi\": 60, \"qj\": 61, \"qk\": 62, \"phi\": 0, \"theta\": 0, \"psi\": 0, \"extra_observations\": {}},\n"
               "{\"t\": 0.29999999999999999, \"x\": 30, \"y\": 31, \"z\": 32, \"u\": 33, \"v\": 34, \"w\": 35, \"p\": 36, \"q\": 37, \"r\": 38, \"qr\": 39, \"qi\": 40, \"qj\": 41, \"qk\": 42, \"phi\": 0, \"theta\": 0, \"psi\": 0, \"extra_observations\": {}},\n"
               "{\"t\": 0.40000000000000002, \"x\": 10, \"y\": 11, \"z\": 12, \"u\": 13, \"v\": 14, \"w\": 15, \"p\": 16, \"q\": 17, \"r\": 18, \"qr\": 19, \"qi\": 20, \"qj\": 21, \"qk\": 22, \"phi\": 0, \"theta\": 0, \"psi\": 0, \"extra_observations\": {}}\n]"
            , encode_YamlStates(decode_YamlSimServerInputs(test_data::dummy_history()).states));
}


TEST_F(parse_historyTest, can_parse_commands)
{
    YamlSimServerInputs yinfos = decode_YamlSimServerInputs(test_data::complete_yaml_message_from_gui());
    ASSERT_EQ(yinfos.commands.find("RPM")->second, 1.2);
    ASSERT_EQ(yinfos.commands.find("B1")->second, 0.1);
}

TEST_F(parse_historyTest, Dt_should_not_be_compulsory)
{
    YamlSimServerInputs yinfos = decode_YamlSimServerInputs(test_data::simserver_message_without_Dt());
    ASSERT_EQ(yinfos.Dt, 0);
}
