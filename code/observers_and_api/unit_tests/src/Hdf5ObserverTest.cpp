#include "yaml_data.hpp"
#include "parse_output.hpp"
#include "Hdf5Observer.hpp"
#include "Hdf5ObserverTest.hpp"
#include "ListOfObservers.hpp"
#include "simulator_api.hpp"

Hdf5ObserverTest::Hdf5ObserverTest() : a(ssc::random_data_generator::DataGenerator(546545))
{
}

TEST_F(Hdf5ObserverTest, should_be_able_to_create_an_observer)
{
    const double dt = 1E-1;
    const double tend = 10;
    auto sys = get_system(test_data::falling_ball_example());
    const auto yaml = parse_output(test_data::falling_ball_example());
    {
        ListOfObservers observers(yaml);
        ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observers);
    }
    for (auto output:yaml)
    {
        if (output.format == "hdf5")
        {
            EXPECT_EQ(0,remove(output.filename.c_str()));
        }
    }
}

