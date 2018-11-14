#include "HistoryParserTest.hpp"
#include "HistoryParser.hpp"
#include "yaml_data.hpp"
#include <ssc/macros.hpp>

HistoryParserTest::HistoryParserTest() : a(ssc::random_data_generator::DataGenerator(4567))
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

TEST_F(HistoryParserTest, convert_yaml_to_State_object)
{
//! [HistoryParserTest example]
    const double Tmax = 0.4;
    HistoryParser hparser(Tmax);
    const double dt = a.random<double>().between(0.0,5.0);
    State ref_history(Tmax);

    for(double t = 0; t <=Tmax +1 ; t+=dt)
    {
        ref_history.x.record(t, a.random<double>().between(-1e3,1e3));
        ref_history.y.record(t, a.random<double>().between(-1e3,1e3));
        ref_history.z.record(t, a.random<double>().between(-1e3,1e3));
        ref_history.u.record(t, a.random<double>().between(-1e3,1e3));
        ref_history.v.record(t, a.random<double>().between(-1e3,1e3));
        ref_history.w.record(t, a.random<double>().between(-1e3,1e3));
        ref_history.p.record(t, a.random<double>().between(-1e3,1e3));
        ref_history.q.record(t, a.random<double>().between(-1e3,1e3));
        ref_history.r.record(t, a.random<double>().between(-1e3,1e3));
        ref_history.qr.record(t, a.random<double>().between(-1e3,1e3));
        ref_history.qi.record(t, a.random<double>().between(-1e3,1e3));
        ref_history.qj.record(t, a.random<double>().between(-1e3,1e3));
        ref_history.qk.record(t, a.random<double>().between(-1e3,1e3));

    }

    const std::string yaml_generated = hparser.emit_state_history_yaml(ref_history);
    const State history = hparser.parse_state_history_yaml(yaml_generated, Tmax);



//! [HistoryParserTest example]
//! [HistoryParserTest expected output]
    for(double t = 0; t <=Tmax ; t+=dt)
    {
        ASSERT_SMALL_RELATIVE_ERROR(ref_history.x(t), history.x(t),1E-10) << "t = " << t;
        ASSERT_SMALL_RELATIVE_ERROR(ref_history.y(t), history.y(t),1E-10) << "t = " << t;
        ASSERT_SMALL_RELATIVE_ERROR(ref_history.z(t), history.z(t),1E-10) << "t = " << t;
        ASSERT_SMALL_RELATIVE_ERROR(ref_history.u(t), history.u(t),1E-10) << "t = " << t;
        ASSERT_SMALL_RELATIVE_ERROR(ref_history.v(t), history.v(t),1E-10) << "t = " << t;
        ASSERT_SMALL_RELATIVE_ERROR(ref_history.w(t), history.w(t),1E-10) << "t = " << t;
        ASSERT_SMALL_RELATIVE_ERROR(ref_history.p(t), history.p(t),1E-10) << "t = " << t;
        ASSERT_SMALL_RELATIVE_ERROR(ref_history.q(t), history.q(t),1E-10) << "t = " << t;
        ASSERT_SMALL_RELATIVE_ERROR(ref_history.r(t), history.r(t),1E-10) << "t = " << t;
        ASSERT_SMALL_RELATIVE_ERROR(ref_history.qr(t), history.qr(t),1E-10) << "t = " << t;
        ASSERT_SMALL_RELATIVE_ERROR(ref_history.qi(t), history.qi(t),1E-10) << "t = " << t;
        ASSERT_SMALL_RELATIVE_ERROR(ref_history.qj(t), history.qj(t),1E-10) << "t = " << t;
        ASSERT_SMALL_RELATIVE_ERROR(ref_history.qk(t), history.qk(t),1E-10) << "t = " << t;

    }
//! [HistoryParserTest expected output]
}

TEST_F(HistoryParserTest, can_parse_Dt)
{
    const std::string yaml =
            "Dt: 12\n"
            "states:\n"
            "    t: 10.123\n"
            "    x: [[10, 1.123], [1, 23.98], [0.5, 31]]\n"
            "    y: [[10, 1.123], [1, 23.98], [0.5, 32]]\n"
            "    z: [[10, 1.123], [1, 23.98], [0.5, 33]]\n"
            "    u: [[10, 1.123], [1, 23.98], [0.5, 34]]\n"
            "    v: [[10, 1.123], [1, 23.98], [0.5, 35]]\n"
            "    w: [[10, 1.123], [1, 23.98], [0.5, 36]]\n"
            "    p: [[10, 1.123], [1, 23.98], [0.5, 37]]\n"
            "    q: [[10, 1.123], [1, 23.98], [0.5, 38]]\n"
            "    r: [[10, 1.123], [1, 23.98], [0.5, 39]]\n"
            "    qr: [[10, 1.123], [1, 23.98], [0.5, 40]]\n"
            "    qi: [[10, 1.123], [1, 23.98], [0.5, 41]]\n"
            "    qj: [[10, 1.123], [1, 23.98], [0.5, 42]]\n"
            "    qk: [[10, 1.123], [1, 23.98], [0.5, 43]]\n"
            "commands:\n"
            "    a: 3\n"
            "    b: 5\n"
            "    c: -12.3\n";
    const double Tmax = 2;
    const HistoryParser hparser(Tmax);
    const SimStepperInfos s = hparser.get_simstepperinfo(yaml);
    ASSERT_DOUBLE_EQ(12, s.Dt);
}

TEST_F(HistoryParserTest, can_parse_commands)
{
    const std::string yaml =
            "Dt: 12\n"
            "states:\n"
            "    t: 10.123\n"
            "    x: [[10, 1.123], [1, 23.98], [0.5, 31]]\n"
            "    y: [[10, 1.123], [1, 23.98], [0.5, 32]]\n"
            "    z: [[10, 1.123], [1, 23.98], [0.5, 33]]\n"
            "    u: [[10, 1.123], [1, 23.98], [0.5, 34]]\n"
            "    v: [[10, 1.123], [1, 23.98], [0.5, 35]]\n"
            "    w: [[10, 1.123], [1, 23.98], [0.5, 36]]\n"
            "    p: [[10, 1.123], [1, 23.98], [0.5, 37]]\n"
            "    q: [[10, 1.123], [1, 23.98], [0.5, 38]]\n"
            "    r: [[10, 1.123], [1, 23.98], [0.5, 39]]\n"
            "    qr: [[10, 1.123], [1, 23.98], [0.5, 40]]\n"
            "    qi: [[10, 1.123], [1, 23.98], [0.5, 41]]\n"
            "    qj: [[10, 1.123], [1, 23.98], [0.5, 42]]\n"
            "    qk: [[10, 1.123], [1, 23.98], [0.5, 43]]\n"
            "commands:\n"
            "    a: 3\n"
            "    b: 5\n"
            "    c: -12.3\n";
    const double Tmax = 2;
    const HistoryParser hparser(Tmax);
    const SimStepperInfos s = hparser.get_simstepperinfo(yaml);
    ASSERT_DOUBLE_EQ(3, s.commands.at("a"));
    ASSERT_DOUBLE_EQ(5, s.commands.at("b"));
    ASSERT_DOUBLE_EQ(-12.3, s.commands.at("c"));
}

TEST_F(HistoryParserTest, can_parse_state_at_t)
{
    const std::string yaml =
            "Dt: 12\n"
            "states:\n"
            "    t: 10.123\n"
            "    x: [[10, 1.123], [1, 23.98], [0.5, 31]]\n"
            "    y: [[10, 1.123], [1, 23.98], [0.5, 32]]\n"
            "    z: [[10, 1.123], [1, 23.98], [0.5, 33]]\n"
            "    u: [[10, 1.123], [1, 23.98], [0.5, 34]]\n"
            "    v: [[10, 1.123], [1, 23.98], [0.5, 35]]\n"
            "    w: [[10, 1.123], [1, 23.98], [0.5, 36]]\n"
            "    p: [[10, 1.123], [1, 23.98], [0.5, 37]]\n"
            "    q: [[10, 1.123], [1, 23.98], [0.5, 38]]\n"
            "    r: [[10, 1.123], [1, 23.98], [0.5, 39]]\n"
            "    qr: [[10, 1.123], [1, 23.98], [0.5, 40]]\n"
            "    qi: [[10, 1.123], [1, 23.98], [0.5, 41]]\n"
            "    qj: [[10, 1.123], [1, 23.98], [0.5, 42]]\n"
            "    qk: [[10, 1.123], [1, 23.98], [0.5, 43]]\n"
            "commands:\n"
            "    a: 3\n"
            "    b: 5\n"
            "    c: -12.3\n";
    const double Tmax = 2;
    const HistoryParser hparser(Tmax);
    const SimStepperInfos s = hparser.get_simstepperinfo(yaml);
    ASSERT_EQ(13, s.state_at_t.size());
    ASSERT_EQ(31, s.state_at_t.at(0));
    ASSERT_EQ(32, s.state_at_t.at(1));
    ASSERT_EQ(33, s.state_at_t.at(2));
    ASSERT_EQ(34, s.state_at_t.at(3));
    ASSERT_EQ(35, s.state_at_t.at(4));
    ASSERT_EQ(36, s.state_at_t.at(5));
    ASSERT_EQ(37, s.state_at_t.at(6));
    ASSERT_EQ(38, s.state_at_t.at(7));
    ASSERT_EQ(39, s.state_at_t.at(8));
    ASSERT_EQ(40, s.state_at_t.at(9));
    ASSERT_EQ(41, s.state_at_t.at(10));
    ASSERT_EQ(42, s.state_at_t.at(11));
    ASSERT_EQ(43, s.state_at_t.at(12));
}
