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

