#include "yaml_data.hpp"
#include <ssc/macros.hpp>

#include "TriMeshTestData.hpp"
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
    const std::vector<YamlState> outputs = sim_server.play_one_step(test_data::complete_yaml_message_for_falling_ball());

//! [SimServerTest example]
//! [SimServerTest expected output]
    ASSERT_EQ(11, outputs.size());
    ASSERT_NEAR(t_end, outputs.back().t, EPS);
    ASSERT_NEAR(4.0+1.0*10.0,              outputs.back().x,  EPS);
    ASSERT_NEAR(8.0,                       outputs.back().y,  EPS);
    ASSERT_NEAR(12.0+g*Dt*(Dt-1.)/2.,      outputs.back().z,  EPS);
    ASSERT_NEAR(1.0,                       outputs.back().u,  EPS);
    ASSERT_NEAR(0.0,                       outputs.back().v,  EPS);
    ASSERT_NEAR(0.0+g*Dt,                  outputs.back().w,  EPS);
    ASSERT_NEAR(0,                         outputs.back().p,  EPS);
    ASSERT_NEAR(0,                         outputs.back().q,  EPS);
    ASSERT_NEAR(0,                         outputs.back().r,  EPS);
    ASSERT_NEAR(1,                         outputs.back().qr, EPS);
    ASSERT_NEAR(0,                         outputs.back().qi, EPS);
    ASSERT_NEAR(0,                         outputs.back().qj, EPS);
    ASSERT_NEAR(0,                         outputs.back().qk, EPS);
//! [SimServerTest expected output]
}

TEST_F(XdynForCSTest, should_throw_if_Dt_is_not_strictly_positive)
{
    const double dt = 1.0;
    const std::string yaml_model = test_data::falling_ball_example();
    const std::string solver = "euler";
    SimServer sim_server(yaml_model, solver, dt);
    ASSERT_THROW(sim_server.play_one_step(test_data::invalid_json_for_cs()), InvalidInputException);
}

TEST_F(XdynForCSTest, can_get_extra_observations)
{
    const double dt = 1.0;
    const std::string yaml_model = test_data::GM_cube();
    const std::string solver = "euler";
    SimServer sim_server(yaml_model, unit_cube(), solver, dt);
    const std::vector<YamlState> outputs = sim_server.play_one_step(test_data::complete_yaml_message_for_falling_ball());
    ASSERT_EQ(11, outputs.size());
    ASSERT_FALSE(outputs.front().extra_observations.empty());
}
