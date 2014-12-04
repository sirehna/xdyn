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

TR1(shared_ptr)<HDBParser> RadiationDampingForceModelTest::get_hdb_data() const
{
    std::vector<double> omegas, Br;
    const double Tpmin = 1;
    const double Tpmax = 10;
    const size_t N = 100;
    for (int i = N-1 ; i>-1 ; --i)
    {
        const double t = Tpmin + (double)i/((double)(N-1))*(Tpmax-Tpmin);
        const double omega = 2*PI/t;
        omegas.push_back(omega);
        Br.push_back(0.5*(0.1/(0.01+(0.5-omega)*(0.5-omega))+0.1/(0.01+(0.5+omega)*(0.5+omega))));
    }
    return TR1(shared_ptr)<HDBParser>(new HDBParserForTests(omegas, Br));
}

YamlRadiationDamping RadiationDampingForceModelTest::get_yaml_data() const
{
    YamlRadiationDamping ret;
    ret.interpolation = TypeOfInterpolation::SPLINES;
    ret.nb_of_points_in_retardation_function = 50;
    ret.quadrature = TypeOfQuadrature::GAUSS_KRONROD;
    return ret;
}

TEST_F(RadiationDampingForceModelTest, example)
{
//! [RadiationDampingForceModelTest example]
//! [RadiationDampingForceModelTest example]
//! [RadiationDampingForceModelTest expected output]
//! [RadiationDampingForceModelTest expected output]
}



