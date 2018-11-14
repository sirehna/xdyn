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
    double tau_list[5] = {0.4, 0.3, 0.2, 0.1, 0.0};
    double x0=10, y0=11, z0=12, u0=13, v0=14, w0=15, p0=16, q0=17, r0=18, qr0=19, qi0=20, qj0=21, qk0=22;
    double step_test = 20;
    size_t size_test = 5;

    YamlState history = parse_history_yaml(test_data::dummy_history());
    //! [parse_historyTest can_parse_simple_history]

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

TEST_F(parse_historyTest, can_produce_yaml_from_YamlHistory)
{
    YamlState state;

    state.t = 123.45;
    const std::string expected_yaml_for_t("t: 123.45");

    state.x.tau = {0.4, 0.3, 0.2, 0.1, 0.0};
    state.x.values = {100,200,300,400,500};
    const std::string expected_yaml_for_x("x: [[0.4, 100], [0.3, 200], [0.2, 300], [0.1, 400], [0, 500]]");

    state.y.tau = {0.4, 0.3, 0.2, 0.1, 0.0};
    state.y.values = {101,201,301,401,501};
    const std::string expected_yaml_for_y= "y: [[0.4, 101], [0.3, 201], [0.2, 301], [0.1, 401], [0, 501]]";

    state.z.tau = {0.4, 0.3, 0.2, 0.1, 0.0};
    state.z.values = {102,202,302,402,502};
    const std::string expected_yaml_for_z("z: [[0.4, 102], [0.3, 202], [0.2, 302], [0.1, 402], [0, 502]]");

    state.u.tau = {0.4, 0.3, 0.2, 0.1, 0.0};
    state.u.values = {103,203,303,403,503};
    const std::string expected_yaml_for_u("u: [[0.4, 103], [0.3, 203], [0.2, 303], [0.1, 403], [0, 503]]");

    state.v.tau = {0.4, 0.3, 0.2, 0.1, 0.0};
    state.v.values = {104,204,304,404,504};
    const std::string expected_yaml_for_v("v: [[0.4, 104], [0.3, 204], [0.2, 304], [0.1, 404], [0, 504]]");

    state.w.tau = {0.4, 0.3, 0.2, 0.1, 0.0};
    state.w.values = {105,205,305,405,505};
    const std::string expected_yaml_for_w("w: [[0.4, 105], [0.3, 205], [0.2, 305], [0.1, 405], [0, 505]]");

    state.p.tau = {0.4, 0.3, 0.2, 0.1, 0.0};
    state.p.values = {106,206,306,406,506};
    const std::string expected_yaml_for_p("p: [[0.4, 106], [0.3, 206], [0.2, 306], [0.1, 406], [0, 506]]");

    state.q.tau = {0.4, 0.3, 0.2, 0.1, 0.0};
    state.q.values = {107,207,307,407,507};
    const std::string expected_yaml_for_q("q: [[0.4, 107], [0.3, 207], [0.2, 307], [0.1, 407], [0, 507]]");

    state.r.tau = {0.4, 0.3, 0.2, 0.1, 0.0};
    state.r.values = {108,208,308,408,508};
    const std::string expected_yaml_for_r("r: [[0.4, 108], [0.3, 208], [0.2, 308], [0.1, 408], [0, 508]]");

    state.qr.tau = {0.4, 0.3, 0.2, 0.1, 0.0};
    state.qr.values = {109,209,309,409,509};
    const std::string expected_yaml_for_qr("qr: [[0.4, 109], [0.3, 209], [0.2, 309], [0.1, 409], [0, 509]]");

    state.qi.tau = {0.4, 0.3, 0.2, 0.1, 0.0};
    state.qi.values = {110,210,310,410,510};
    const std::string expected_yaml_for_qi("qi: [[0.4, 110], [0.3, 210], [0.2, 310], [0.1, 410], [0, 510]]");

    state.qj.tau = {0.4, 0.3, 0.2, 0.1, 0.0};
    state.qj.values = {111,211,311,411,511};
    const std::string expected_yaml_for_qj("qj: [[0.4, 111], [0.3, 211], [0.2, 311], [0.1, 411], [0, 511]]");

    state.qk.tau = {0.4, 0.3, 0.2, 0.1, 0.0};
    state.qk.values = {112,212,312,412,512};
    const std::string expected_yaml_for_qk("qk: [[0.4, 112], [0.3, 212], [0.2, 312], [0.1, 412], [0, 512]]");

    std::stringstream sstream;
    std::string expected_yaml;
    sstream<<"{"<<expected_yaml_for_t<<", "
                <<expected_yaml_for_x<<", "
                <<expected_yaml_for_y<<", "
                <<expected_yaml_for_z<<", "
                <<expected_yaml_for_u<<", "
                <<expected_yaml_for_v<<", "
                <<expected_yaml_for_w<<", "
                <<expected_yaml_for_p<<", "
                <<expected_yaml_for_q<<", "
                <<expected_yaml_for_r<<", "
                <<expected_yaml_for_qr<<", "
                <<expected_yaml_for_qi<<", "
                <<expected_yaml_for_qj<<", "
                <<expected_yaml_for_qk<<"}";
    expected_yaml = sstream.str();


    //! [parse_historyTest can_parse_simple_history]

    std::string result(generate_history_yaml(state));

    //! [parse_historyTest can_parse_simple_history]

    //! [parse_historyTest expected output]

        ASSERT_EQ(expected_yaml,result);

    //! [parse_historyTest expected output]
}

TEST_F(parse_historyTest, translation_loop)
{
    const YamlState history = parse_history_yaml(test_data::dummy_history());
    const std::string yaml = generate_history_yaml(history);
    const YamlState new_history = parse_history_yaml(yaml);

    ASSERT_EQ(history, new_history);
}

TEST_F(parse_historyTest, get_info_from_gui)
{
    YamlSimServerInputs yinfos = parse_YamlSimServerInputs(test_data::complete_yaml_message_from_gui());
    ASSERT_EQ(yinfos.commands.find("RPM")->second, 1.2);
    ASSERT_EQ(yinfos.commands.find("B1")->second, 0.1);
    ASSERT_EQ(yinfos.state.t, 1234.5);
    ASSERT_EQ(yinfos.state.x.values[0], 90);
    ASSERT_EQ(yinfos.state.y.values[0], 91);
    ASSERT_EQ(yinfos.state.z.values[0], 92);
    ASSERT_EQ(yinfos.state.u.values[0], 93);
    ASSERT_EQ(yinfos.state.v.values[0], 94);
    ASSERT_EQ(yinfos.state.w.values[0], 95);
    ASSERT_EQ(yinfos.state.p.values[0], 96);
    ASSERT_EQ(yinfos.state.q.values[0], 97);
    ASSERT_EQ(yinfos.state.r.values[0], 98);
    ASSERT_EQ(yinfos.state.qr.values[0], 99);
    ASSERT_EQ(yinfos.state.qi.values[0], 100);
    ASSERT_EQ(yinfos.state.qj.values[0], 101);
    ASSERT_EQ(yinfos.state.qk.values[0], 102);
}
