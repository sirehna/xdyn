#include "yaml_data.hpp"
#include "parse_output.hpp"
#include "JsonObserver.hpp"
#include "JsonObserverTest.hpp"
#include "ListOfObservers.hpp"
#include "simulator_api.hpp"

JsonObserverTest::JsonObserverTest() : a(ssc::random_data_generator::DataGenerator(666))
{
}

TEST_F(JsonObserverTest, should_be_able_to_create_an_observer)
{
    const double dt = 1E-1;
    const double tend = 10;
    auto sys = get_system(test_data::falling_ball_example(), 0);
    const auto yaml = parse_output(test_data::falling_ball_example());
    {
        ListOfObservers observers(yaml);
        ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observers);
    }
    for (auto output:yaml)
    {
        if (output.format == "json")
        {
            if (not output.filename.empty())
            {
                //EXPECT_EQ(0,remove(output.filename.c_str()));
            }
        }
    }
}

TEST_F(JsonObserverTest, should_be_able_to_create_an_observer_with_waves)
{
    const double dt = 1E-1;
    const double tend = 10;
    auto sys = get_system(test_data::falling_ball_example(), 0);
    const auto yaml = parse_output(test_data::falling_ball_example());
    {
        ListOfObservers observers(yaml);
        ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observers);
    }
    for (auto output:yaml)
    {
        if (output.format == "json")
        {
            if (not output.filename.empty())
            {
                //EXPECT_EQ(0,remove(output.filename.c_str()));
            }
        }
    }
}
