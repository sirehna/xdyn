/*
 * HistoryTest.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: cady
 */


#include "History.hpp"
#include "HistoryTest.hpp"

HistoryTest::HistoryTest() : a(ssc::random_data_generator::DataGenerator(5422))
{
}

HistoryTest::~HistoryTest()
{
}

void HistoryTest::SetUp()
{
}

void HistoryTest::TearDown()
{
}

TEST_F(HistoryTest, example)
{
//! [HistoryTest example]
    History h;
//! [HistoryTest example]
//! [HistoryTest expected output]
//! [HistoryTest expected output]
}



