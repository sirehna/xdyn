/*
 * StretchingTest.cpp
 *
 *  Created on: Dec 20, 2016
 *      Author: cady
 */

#include "StretchingTest.hpp"
#include "Stretching.hpp"
#include "YamlWaveModelInput.hpp"

StretchingTest::StretchingTest() : a(ssc::random_data_generator::DataGenerator(88223232))
{
}

StretchingTest::~StretchingTest()
{
}

void StretchingTest::SetUp()
{
}

void StretchingTest::TearDown()
{
}

TEST_F(StretchingTest, single_frequency_single_direction_at_one_point)
{
    //! [StretchingTest example]
    YamlStretching yaml;
    yaml.delta = 0.4;
    yaml.h = 123;
    //! [StretchingTest example]
    //! [StretchingTest expected output]
    //! [StretchingTest expected output]
}
