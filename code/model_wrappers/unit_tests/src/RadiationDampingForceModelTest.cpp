/*
 * RadiationDampingForceModelTest.cpp
 *
 *  Created on: Dec 1, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "generate_body_for_tests.hpp"
#include "RadiationDampingForceModel.hpp"
#include "RadiationDampingForceModelTest.hpp"

RadiationDampingForceModelTest::RadiationDampingForceModelTest() : a(ssc::random_data_generator::DataGenerator(2121))
{
}

RadiationDampingForceModelTest::~RadiationDampingForceModelTest()
{
}

void RadiationDampingForceModelTest::SetUp()
{
}

void RadiationDampingForceModelTest::TearDown()
{
}

HDBParserForTests RadiationDampingForceModelTest::get_test_data() const
{
    std::vector<double> Tp, Br;
    const double Tpmin = 0;
    const double Tpmax = 10;
    const size_t N = 100;
    for (size_t i = 0 ; i < N ; ++i)
    {
        const double t = Tpmin + (double)i/((double)(N-1)*(Tpmax-Tpmin));
        const double omega = 2*PI/t;
        Tp.push_back(t);
        Br.push_back(0.5*(0.1/(0.01+(0.5-omega)*(0.5-omega))+0.1/(0.01+(0.5+omega)*(0.5+omega))));
    }
    return HDBParserForTests(Tp, Br);
}

TEST_F(RadiationDampingForceModelTest, example)
{
//! [RadiationDampingForceModelTest example]
//! [RadiationDampingForceModelTest example]
//! [RadiationDampingForceModelTest expected output]
//! [RadiationDampingForceModelTest expected output]
}



