/*
 * SimpleTrackKeepingControllerTest.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: cady
 */


#include "SimpleTrackKeepingControllerTest.hpp"
#include "SimpleTrackKeepingController.hpp"
#include "yaml_data.hpp"

SimpleTrackKeepingControllerTest::SimpleTrackKeepingControllerTest() : a(ssc::random_data_generator::DataGenerator(545454))
{
}

SimpleTrackKeepingControllerTest::~SimpleTrackKeepingControllerTest()
{
}

void SimpleTrackKeepingControllerTest::SetUp()
{
}

void SimpleTrackKeepingControllerTest::TearDown()
{
}

TEST_F(SimpleTrackKeepingControllerTest, parser)
{
    const auto k = SimpleTrackKeepingController::parse(test_data::simple_track_keeping());
    ASSERT_DOUBLE_EQ(4, k.Tp);
    ASSERT_DOUBLE_EQ(0.9, k.ksi);
    ASSERT_EQ("controller", k.name);
}



