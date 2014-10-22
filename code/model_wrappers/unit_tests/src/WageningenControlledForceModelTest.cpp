/*
 * WageningenControlledForceModelTest.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#include "force_parsers.hpp"
#include "yaml_data.hpp"
#include "WageningenControlledForceModel.hpp"
#include "WageningenControlledForceModelTest.hpp"

WageningenControlledForceModelTest::WageningenControlledForceModelTest() : a(ssc::random_data_generator::DataGenerator(7))
{
}

WageningenControlledForceModelTest::~WageningenControlledForceModelTest()
{
}

void WageningenControlledForceModelTest::SetUp()
{
}

void WageningenControlledForceModelTest::TearDown()
{
}

TEST_F(WageningenControlledForceModelTest, example)
{
//! [WageningenControlledForceModelTest example]
    const WageningenControlledForceModel w(parse_wageningen(test_data::wageningen()));
//! [WageningenControlledForceModelTest example]
//! [WageningenControlledForceModelTest expected output]
//! [WageningenControlledForceModelTest expected output]
}



