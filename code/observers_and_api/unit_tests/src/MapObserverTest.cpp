/*
 * MapObserverTest.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: cady
 */


#include "MapObserver.hpp"
#include "MapObserverTest.hpp"
#include "yaml_data.hpp"
#include "parse_output.hpp"
#include "ListOfObservers.hpp"
#include "simulator_api.hpp"

MapObserverTest::MapObserverTest() : a(ssc::random_data_generator::DataGenerator(8725200))
{
}

MapObserverTest::~MapObserverTest()
{
}

void MapObserverTest::SetUp()
{
}

void MapObserverTest::TearDown()
{
}

TEST_F(MapObserverTest, example)
{
//! [MapObserverTest example]
    const double dt = 1;
    const double tend = 1;
    auto sys = get_system(test_data::falling_ball_example(), 0);

    YamlOutput out;
    out.data = {"x(ball)"};
    out.format = "map";
    ListOfObservers observers({out});
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observers);
    const auto obs = observers.get();
//! [MapObserverTest example]
//! [MapObserverTest expected output]
    ASSERT_EQ(1, obs.size());
    const auto map_observer = static_cast<MapObserver*>(obs.front().get());
    const auto m = map_observer->get();
    ASSERT_EQ(1, m.size());
    ASSERT_TRUE(m.find("x(ball)") != m.end());
//! [MapObserverTest expected output]
}



