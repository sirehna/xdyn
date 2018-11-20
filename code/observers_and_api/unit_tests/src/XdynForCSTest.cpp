#include "yaml_data.hpp"
#include <ssc/macros.hpp>

#include "XdynForCS.hpp"
#include "XdynForCSTest.hpp"
#define EPS 1E-8

XdynForCSTest::XdynForCSTest() : a(ssc::random_data_generator::DataGenerator(123456789))
{
}

XdynForCSTest::~XdynForCSTest()
{
}

void XdynForCSTest::SetUp()
{
}

void XdynForCSTest::TearDown()
{
}

TEST_F(XdynForCSTest, test_falling_ball_with_yaml)
{
//! [SimServerTest example]

    const double g = 9.81;
    const double dt = 1.0;
    const double t_start = 1.87;
    const double Dt = 10;
    const double t_end = t_start+Dt;
    const std::string yaml_model = test_data::falling_ball_example();
    const std::string solver = "euler";
    SimServer sim_server(yaml_model, solver, dt);
    const std::string input_yaml = test_data::complete_yaml_message_for_falling_ball();
    const std::vector<Res> outputs = sim_server.play_one_step(input_yaml);

//! [SimServerTest example]
//! [SimServerTest expected output]
    ASSERT_EQ(11, outputs.size());
    ASSERT_NEAR(t_end, outputs.back().t, EPS);
    ASSERT_NEAR(4.0+1.0*10.0,              outputs.back().x[0], EPS);
    ASSERT_NEAR(8.0,                       outputs.back().x[1], EPS);
    ASSERT_NEAR(12.0+g*Dt*(Dt-1.)/2.,outputs.back().x[2], EPS);
    ASSERT_NEAR(1.0,                       outputs.back().x[3], EPS);
    ASSERT_NEAR(0.0,                       outputs.back().x[4], EPS);
    ASSERT_NEAR(0.0+g*Dt,               outputs.back().x[5], EPS);
    ASSERT_NEAR(0,                         outputs.back().x[6], EPS);
    ASSERT_NEAR(0,                         outputs.back().x[7], EPS);
    ASSERT_NEAR(0,                         outputs.back().x[8], EPS);
    ASSERT_NEAR(1,                         outputs.back().x[9], EPS);
    ASSERT_NEAR(0,                         outputs.back().x[10], EPS);
    ASSERT_NEAR(0,                         outputs.back().x[11], EPS);
    ASSERT_NEAR(0,                         outputs.back().x[12], EPS);
//! [SimServerTest expected output]
}
