/*
 * EverythingObserverTest.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: cady
 */



#include "EverythingObserver.hpp"
#include "EverythingObserverTest.hpp"
#include "yaml_data.hpp"
#include "parse_output.hpp"
#include "ListOfObservers.hpp"
#include "simulator_api.hpp"
#include "stl_data.hpp"

#define EPS 1E-8
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

EverythingObserverTest::EverythingObserverTest() : a(ssc::random_data_generator::DataGenerator(8725200))
{
}

EverythingObserverTest::~EverythingObserverTest()
{
}

void EverythingObserverTest::SetUp()
{
}

void EverythingObserverTest::TearDown()
{
}

ListOfObservers observers();
ListOfObservers observers()
{
    return ListOfObservers({ObserverPtr(new EverythingObserver())});
}

std::vector<Res> get_results(const ListOfObservers& observers)
{
    if (observers.empty())
        return std::vector<Res>();
    const auto observer = static_cast<EverythingObserver*>(observers.get().front().get());
    return observer->get();
}

TEST_F(EverythingObserverTest, example)
{
//! [MapObserverTest example]
    const double dt = 1;
    const double tend = 1;
    auto sys = get_system(test_data::GM_cube(), test_data::cube(), 0);
    auto list_of_observers = observers();

    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, list_of_observers);
    const auto obs = list_of_observers.get();
//! [MapObserverTest example]
//! [MapObserverTest expected output]
    ASSERT_EQ(1, obs.size());
    const auto results = get_results(list_of_observers);
    ASSERT_EQ(2, results.size());
    ASSERT_TRUE(results.front().extra_observations.find("GM(cube)") != results.front().extra_observations.end());
//! [MapObserverTest expected output]
}

TEST_F(EverythingObserverTest, GM)
{
    const double dt = 1;
    const double tend = 1;
    auto sys = get_system(test_data::GM_cube(), test_data::cube(), 0);
    auto list_of_observers = observers();
    ssc::solver::quicksolve<ssc::solver::EulerStepper>(sys, 0, tend, dt, list_of_observers);
    const auto obs = list_of_observers.get();
//! [MapObserverTest example]
//! [MapObserverTest expected output]
    ASSERT_EQ(1, obs.size());
    const auto results = get_results(list_of_observers);
    ASSERT_EQ(2, results.size());
    ASSERT_TRUE(results.back().extra_observations.find("Fz(GM,cube,NED)") != results.back().extra_observations.end());
    ASSERT_TRUE(results.back().extra_observations.find("GM(cube)") != results.back().extra_observations.end());
    ASSERT_NEAR(-1000*9.81*0.5, results.back().extra_observations.at("Fz(GM,cube,NED)"), EPS);
    ASSERT_NEAR(1/(12*PI), results.back().extra_observations.at("GM(cube)"), EPS);
}
