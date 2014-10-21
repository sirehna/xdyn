/*
 * listenersTest.cpp
 *
 *  Created on: Oct 21, 2014
 *      Author: cady
 */

#include "listenersTest.hpp"
#include "listeners.hpp"
#include "yaml_data.hpp"

listenersTest::listenersTest() : a(ssc::random_data_generator::DataGenerator(54545))
{
}

listenersTest::~listenersTest()
{
}

void listenersTest::SetUp()
{
}

void listenersTest::TearDown()
{
}

TEST_F(listenersTest, example)
{
//! [listenersTest listen_to_file_example]
    auto ds = listen_to_file(test_data::controlled_forces());
    ds.set<double>("t", 0);
    //! [listenersTest listen_to_file_example]
}



