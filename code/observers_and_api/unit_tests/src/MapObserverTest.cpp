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
#include "STL_data.hpp"

#define EPS 1E-8
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

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

ListOfObservers observe(const std::vector<std::string>& stuff_to_watch);
ListOfObservers observe(const std::vector<std::string>& stuff_to_watch)
{
    YamlOutput out;
    out.data = stuff_to_watch;
    out.format = "map";
    return ListOfObservers({out});
}

std::map<std::string,std::vector<double> > get_map(const ListOfObservers& observers)
{
    const auto map_observer = static_cast<MapObserver*>(observers.get().front().get());
    return map_observer->get();
}

TEST_F(MapObserverTest, example)
{
//! [MapObserverTest example]
    const double dt = 1;
    const double tend = 1;
    auto sys = get_system(test_data::falling_ball_example(), 0);

    auto observers = observe({"x(ball)"});
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observers);
    const auto obs = observers.get();
//! [MapObserverTest example]
//! [MapObserverTest expected output]
    ASSERT_EQ(1, obs.size());
    const auto m = get_map(observers);
    ASSERT_EQ(1, m.size());
    ASSERT_TRUE(m.find("x(ball)") != m.end());
//! [MapObserverTest expected output]
}

TEST_F(MapObserverTest, GM)
{
    const double dt = 1;
    const double tend = 1;
    auto sys = get_system(test_data::GM_cube(), test_data::cube(), 0);
    auto observers = observe({"Fz(GM,cube,NED)","GM(cube)"});
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, observers);
    const auto m = get_map(observers);
    ASSERT_TRUE(m.find("Fz(GM,cube,NED)") != m.end());
    ASSERT_TRUE(m.find("GM(cube)") != m.end());
    ASSERT_NEAR(-1000*9.81*0.5, m.find("Fz(GM,cube,NED)")->second.back(), EPS);
    ASSERT_NEAR(-1/(12*PI), m.find("GM(cube)")->second.back(), EPS);
}

