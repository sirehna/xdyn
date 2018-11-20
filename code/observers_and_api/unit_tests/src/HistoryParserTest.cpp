#include "HistoryParserTest.hpp"
#include "HistoryParser.hpp"
#include "yaml_data.hpp"
#include <ssc/macros.hpp>

HistoryParserTest::HistoryParserTest() : a(ssc::random_data_generator::DataGenerator(4567)), yaml(
        "{\"Dt\": 12\n"
        ",\"states\":\n"
        "    [ {\"t\": 9.123 , \"x\": 1.123, \"y\": 2.123, \"z\": 3.123, \"u\": 4.123, \"v\": 5.123, \"w\": 6.123, \"p\": 7.123, \"q\": 8.123, \"r\": 9.123, \"qr\": 10.123, \"qi\": 11.123, \"qj\": 12.123, \"qk\": 13.132}\n"
        "    , {\"t\": 10.123, \"x\": 33.98, \"y\": 34.98, \"z\": 35.98, \"u\": 36.98, \"v\": 37.98, \"w\": 38.98, \"p\": 39.98, \"q\": 40.98, \"r\": 41.98, \"qr\": 42.98, \"qi\": 43.98, \"qj\": 44.98, \"qk\": 45.132}\n"
        "    , {\"t\": 51.123, \"x\": 52.32, \"y\": 53.32, \"z\": 54.32, \"u\": 55.32, \"v\": 56.32, \"w\": 57.32, \"p\": 58.32, \"q\": 59.32, \"r\": 60.32, \"qr\": 61.32, \"qi\": 62.32, \"qj\": 63.32, \"qk\": 64.132}]\n"
        ",\"commands\":\n"
        " {   \"a\": 3,\n"
        "    \"b\": 5,\n"
        "    \"c\": -12.3}}")
{
}

HistoryParserTest::~HistoryParserTest()
{
}

void HistoryParserTest::SetUp()
{
}

void HistoryParserTest::TearDown()
{
}

TEST_F(HistoryParserTest, can_emit_state_history_yaml)
{
//! [HistoryParserTest example]
    const std::vector<Res> states = {Res(0,    {1,2,3,4,5,6,7,8,9,10,11,12,13})
                                    ,Res(0.123,{21,22,23,24,25,26,27,28,29,30,31,32,33})
                                    ,Res(0.456,{41,42,43,44,45,46,47,48,49,50,51,52,53})
                                    };
    const std::string expected_yaml =
            "- {t: 0, x: 1, y: 2, z: 3, u: 4, v: 5, w: 6, p: 7, q: 8, r: 9, qr: 10, qi: 11, qj: 12, qk: 13}\n"
            "- {t: 0.123, x: 21, y: 22, z: 23, u: 24, v: 25, w: 26, p: 27, q: 28, r: 29, qr: 30, qi: 31, qj: 32, qk: 33}\n"
            "- {t: 0.456, x: 41, y: 42, z: 43, u: 44, v: 45, w: 46, p: 47, q: 48, r: 49, qr: 50, qi: 51, qj: 52, qk: 53}";

    ASSERT_EQ(expected_yaml, emit_state_history_json(states));
//! [HistoryParserTest expected output]
}

TEST_F(HistoryParserTest, can_parse_Dt)
{
    const SimServerInputs s = parse_SimServerInputs(yaml, 100);
    ASSERT_DOUBLE_EQ(12, s.Dt);
}

TEST_F(HistoryParserTest, can_parse_commands)
{
    const SimServerInputs s = parse_SimServerInputs(yaml, 100);
    ASSERT_DOUBLE_EQ(3, s.commands.at("a"));
    ASSERT_DOUBLE_EQ(5, s.commands.at("b"));
    ASSERT_DOUBLE_EQ(-12.3, s.commands.at("c"));
}

TEST_F(HistoryParserTest, can_parse_state_at_t)
{
    const SimServerInputs s = parse_SimServerInputs(yaml, 100);
    ASSERT_EQ(13, s.state_at_t.size());
    ASSERT_EQ(52.32, s.state_at_t.at(0));
    ASSERT_EQ(53.32, s.state_at_t.at(1));
    ASSERT_EQ(54.32, s.state_at_t.at(2));
    ASSERT_EQ(55.32, s.state_at_t.at(3));
    ASSERT_EQ(56.32, s.state_at_t.at(4));
    ASSERT_EQ(57.32, s.state_at_t.at(5));
    ASSERT_EQ(58.32, s.state_at_t.at(6));
    ASSERT_EQ(59.32, s.state_at_t.at(7));
    ASSERT_EQ(60.32, s.state_at_t.at(8));
    ASSERT_EQ(61.32, s.state_at_t.at(9));
    ASSERT_EQ(62.32, s.state_at_t.at(10));
    ASSERT_EQ(63.32, s.state_at_t.at(11));
    ASSERT_EQ(64.132, s.state_at_t.at(12));
}

TEST_F(HistoryParserTest, can_parse_all_states_before_t)
{
    const SimServerInputs s = parse_SimServerInputs(yaml, 100);
    ASSERT_EQ(2, s.state_history_except_last_point.x.size());
}

TEST_F(HistoryParserTest, t_should_correspond_to_last_value_in_yaml)
{
    const SimServerInputs s = parse_SimServerInputs(yaml, 100);
    ASSERT_DOUBLE_EQ(51.123, s.t);
}
