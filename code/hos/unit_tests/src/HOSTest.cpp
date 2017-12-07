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
#include <boost/algorithm/string.hpp> // replace in string

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
//! [HOSTest relative_wave_height expected output]
    ASSERT_FLOAT_EQ(-0.39214659, hos.wave_height(0,0,0));
//! [HOSTest relative_wave_height expected output]
}

TEST_F(HOSTest, relative_wave_height_taking_direction_into_account)
{
    const double t = 0;//a.random<double>().between(0, 5);
    auto yaml = test_data::hos_for_parser_validation_only();

    double hos_at_0_deg_1_0 = 1;
    double hos_at_0_deg_0_1 = 2;
    double hos_at_0_deg_minus_1_0 = 3;
    // Scoped calls so destructors are called: cannot use several clients with same HOS server...
    {
        boost::replace_all(yaml,"waves propagating to: {value: 90, unit: deg}","waves propagating to: {value: 0, unit: deg}");
        HOS hos_at_0_deg(parse_hos(yaml));
        hos_at_0_deg_1_0 = hos_at_0_deg.wave_height(1,0,t);
        hos_at_0_deg_0_1 = hos_at_0_deg.wave_height(0,1,t);
        hos_at_0_deg_minus_1_0 = hos_at_0_deg.wave_height(-1,0,t);
    }
    double hos_at_90_deg_minus_1_0 = 4;
    double hos_at_90_deg_0_1 = 5;
    {
        boost::replace_all(yaml,"waves propagating to: {value: 0, unit: deg}","waves propagating to: {value: 90, unit: deg}");
        HOS hos_at_90_deg(parse_hos(yaml));
        hos_at_90_deg_minus_1_0 = hos_at_90_deg.wave_height(1,0,t);
        hos_at_90_deg_0_1 = hos_at_90_deg.wave_height(0,1,t);
    }
    double hos_at_180_deg_1_0 = 6;
    {
        boost::replace_all(yaml,"waves propagating to: {value: 90, unit: deg}","waves propagating to: {value: 180, unit: deg}");
        HOS hos_at_180_deg(parse_hos(yaml));
        hos_at_180_deg_1_0 = hos_at_180_deg.wave_height(1,0,t);
    }
    double hos_at_135_deg_sqrt = 7;
    {
        boost::replace_all(yaml,"waves propagating to: {value: 180, unit: deg}","waves propagating to: {value: 135, unit: deg}");
        HOS hos_at_135_deg(parse_hos(yaml));
        hos_at_135_deg_sqrt = hos_at_135_deg.wave_height(-sqrt(2)/2,sqrt(2)/2,t);
    }

    const double EPS=1E-2;
    ASSERT_NEAR(hos_at_0_deg_1_0, hos_at_90_deg_0_1, EPS);
    ASSERT_NEAR(hos_at_0_deg_0_1, hos_at_90_deg_minus_1_0, EPS);
    ASSERT_NEAR(hos_at_180_deg_1_0, hos_at_0_deg_minus_1_0, EPS);
    ASSERT_NEAR(hos_at_135_deg_sqrt, hos_at_0_deg_1_0, EPS);

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
