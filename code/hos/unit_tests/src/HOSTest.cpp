/*
 * HOSTest.cpp
 *
 *  Created on: Nov 22, 2017
 *      Author: cady
 */

#include "environment_parsers.hpp"
#include "HOSTest.hpp"
#include "HOS.hpp"
#include "yaml_data.hpp"

HOSTest::HOSTest() : a(ssc::random_data_generator::DataGenerator(892251200))
{
}

HOSTest::~HOSTest()
{
}

void HOSTest::SetUp()
{
}

void HOSTest::TearDown()
{
}

TEST_F(HOSTest, can_set_hos_parameters)
{
//! [HOSTest relative_wave_height example]
    HOS hos(parse_hos(test_data::hos_for_parser_validation_only()));
//! [HOSTest relative_wave_height example]
//! [HOSTest relative_wave_height expected output]

//! [HOSTest relative_wave_height expected output]
}

TEST_F(HOSTest, relative_wave_height)
{
//! [HOSTest relative_wave_height example]
    HOS hos(parse_hos(test_data::hos_for_parser_validation_only()));
//! [HOSTest relative_wave_height example]
    ASSERT_FLOAT_EQ(0.392147, hos.wave_height(0,0,0));
//! [HOSTest relative_wave_height expected output]

//! [HOSTest relative_wave_height expected output]
}

TEST_F(HOSTest, dynamic_pressure)
{
//! [HOSTest dynamic_pressure example]

//! [HOSTest dynamic_pressure example]
//! [HOSTest dynamic_pressure expected output]

//! [HOSTest dynamic_pressure expected output]
}

TEST_F(HOSTest, orbital_velocity)
{
    //! [HOSTest orbital_velocity example]
    //! [HOSTest orbital_velocity example]
    //! [HOSTest orbital_velocity expected output]
    //! [HOSTest orbital_velocity expected output]
}
