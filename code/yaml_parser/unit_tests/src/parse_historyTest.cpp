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
//! [parse_historyTest example]
    double tau_list[5] = {0.0, 0.1, 0.2, 0.3, 0.4};
    double x0=10, y0=11, z0=12, u0=13, v0=14, w0=15, p0=16, q0=17, r0=18, qr0=19, qi0=20, qj0=21, qk0=22;
    double step_test = 20;
    size_t size_test = 5;

    YamlState history = parse_history_yaml(test_data::dummy_history());
    //! [parse_historyTest example]

    //! [parse_historyTest expected output]
    ASSERT_EQ(1234.5, history.t);
    ASSERT_EQ(size_test, history.x.values.size());
    ASSERT_EQ(size_test, history.y.values.size());
    ASSERT_EQ(size_test, history.z.values.size());
    ASSERT_EQ(size_test, history.u.values.size());
    ASSERT_EQ(size_test, history.v.values.size());
    ASSERT_EQ(size_test, history.w.values.size());
    ASSERT_EQ(size_test, history.p.values.size());
    ASSERT_EQ(size_test, history.q.values.size());
    ASSERT_EQ(size_test, history.r.values.size());
    ASSERT_EQ(size_test, history.qr.values.size());
    ASSERT_EQ(size_test, history.qi.values.size());
    ASSERT_EQ(size_test, history.qj.values.size());
    ASSERT_EQ(size_test, history.qk.values.size());

    for(size_t it = 0; it<size_test ; it++)
    {
        ASSERT_EQ(x0+((double)it)*step_test, history.x.values[it]);
        ASSERT_EQ(y0+((double)it)*step_test, history.y.values[it]);
        ASSERT_EQ(z0+((double)it)*step_test, history.z.values[it]);
        ASSERT_EQ(u0+((double)it)*step_test, history.u.values[it]);
        ASSERT_EQ(v0+((double)it)*step_test, history.v.values[it]);
        ASSERT_EQ(w0+((double)it)*step_test, history.w.values[it]);
        ASSERT_EQ(p0+((double)it)*step_test, history.p.values[it]);
        ASSERT_EQ(q0+((double)it)*step_test, history.q.values[it]);
        ASSERT_EQ(r0+((double)it)*step_test, history.r.values[it]);
        ASSERT_EQ(qr0+((double)it)*step_test, history.qr.values[it]);
        ASSERT_EQ(qi0+((double)it)*step_test, history.qi.values[it]);
        ASSERT_EQ(qj0+((double)it)*step_test, history.qj.values[it]);
        ASSERT_EQ(qk0+((double)it)*step_test, history.qk.values[it]);

        ASSERT_EQ(tau_list[it], history.x.tau[it]);
        ASSERT_EQ(tau_list[it], history.y.tau[it]);
        ASSERT_EQ(tau_list[it], history.z.tau[it]);
        ASSERT_EQ(tau_list[it], history.u.tau[it]);
        ASSERT_EQ(tau_list[it], history.v.tau[it]);
        ASSERT_EQ(tau_list[it], history.w.tau[it]);
        ASSERT_EQ(tau_list[it], history.p.tau[it]);
        ASSERT_EQ(tau_list[it], history.q.tau[it]);
        ASSERT_EQ(tau_list[it], history.r.tau[it]);
        ASSERT_EQ(tau_list[it], history.qr.tau[it]);
        ASSERT_EQ(tau_list[it], history.qi.tau[it]);
        ASSERT_EQ(tau_list[it], history.qj.tau[it]);
        ASSERT_EQ(tau_list[it], history.qk.tau[it]);
    }

//! [parse_historyTest expected output]
}
