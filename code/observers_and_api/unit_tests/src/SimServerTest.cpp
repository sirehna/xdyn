#include "SimServerTest.hpp"
#include "SimServer.hpp"
#include "yaml_data.hpp"
#include <ssc/macros.hpp>
#define EPS 1E-8

SimServerTest::SimServerTest() : a(ssc::random_data_generator::DataGenerator(123456789))
{
}

SimServerTest::~SimServerTest()
{
}

void SimServerTest::SetUp()
{
}

void SimServerTest::TearDown()
{
}

TEST_F(SimServerTest, test_falling_ball_with_yaml)
{
//! [SimServerTest example]

    const double g = 9.81;
    const double dt = 1.0;
    const double t_start = 0;
    const double Dt = 10;
    const double t_end = t_start+Dt;
    const std::string yaml_model = test_data::falling_ball_example();
    const std::string solver = "euler";
    HistoryParser parser(0);
    SimServer sim_server(yaml_model, solver, dt);
    const std::string input_yaml = test_data::complete_yaml_message_for_falling_ball();
    const std::string output_yaml = sim_server.play_one_step(input_yaml);
    State output_state = parser.parse_state_history_yaml(output_yaml, 0.0);

//! [SimServerTest example]
//! [SimServerTest expected output]
    ASSERT_NEAR(4.0+1.0*10.0,              output_state.x(), EPS);
    ASSERT_NEAR(8.0,                       output_state.y(), EPS);
    ASSERT_NEAR(12.0+g*t_end*(t_end-1.)/2.,output_state.z(), EPS);
    ASSERT_NEAR(1.0,                       output_state.u(), EPS);
    ASSERT_NEAR(0.0,                       output_state.v(), EPS);
    ASSERT_NEAR(0.0+g*t_end,               output_state.w(), EPS);
    ASSERT_NEAR(0,                         output_state.p(), EPS);
    ASSERT_NEAR(0,                         output_state.q(), EPS);
    ASSERT_NEAR(0,                         output_state.r(), EPS);
    ASSERT_NEAR(1,                         output_state.qr(), EPS);
    ASSERT_NEAR(0,                         output_state.qi(), EPS);
    ASSERT_NEAR(0,                         output_state.qj(), EPS);
    ASSERT_NEAR(0,                         output_state.qk(), EPS);
//! [SimServerTest expected output]
}
