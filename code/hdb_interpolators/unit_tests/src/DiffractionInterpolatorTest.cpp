/*
 * DiffractionInterpolatorTest.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: cady
 */

#include "DiffractionInterpolatorTest.hpp"
#include <algorithm> //std::reverse

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "HDBParser.hpp"
#include "hdb_data.hpp"

#include "DiffractionInterpolator.hpp"

DiffractionInterpolatorTest::DiffractionInterpolatorTest() : a(ssc::random_data_generator::DataGenerator(989899))
{
}

DiffractionInterpolatorTest::~DiffractionInterpolatorTest()
{
}

void DiffractionInterpolatorTest::SetUp()
{
}

void DiffractionInterpolatorTest::TearDown()
{
}

TEST_F(DiffractionInterpolatorTest, example)
{
//! [DiffractionInterpolatorTest example]
    const HDBParser data(test_data::bug_3210());
    std::vector<double> omegas = {4,64,125};
    std::vector<double> psis = {0,30};
    const bool mirror = true;
    for (size_t i = 0 ; i < omegas.size() ; ++i) omegas[i] = 2*PI/omegas[i];
    std::reverse(omegas.begin(),omegas.end());
    for (size_t i = 0 ; i < psis.size() ; ++i) psis[i] *= PI/180.;
    DiffractionInterpolator radiation(data,omegas,psis,mirror);
    const size_t k = 0; // X-axis
    const std::vector<std::vector<double> > modules = radiation.get_modules_cartesian(k);
    const std::vector<std::vector<double> > phases = radiation.get_phases_cartesian(k);
//! [DiffractionInterpolatorTest example]
//! [DiffractionInterpolatorTest expected output]
    ASSERT_EQ(omegas.size(),modules.size());
    ASSERT_EQ(omegas.size(),phases.size());
    for (auto phase:phases)
    {
        ASSERT_EQ(psis.size(),phase.size());
    }
    for (auto module:modules)
    {
        ASSERT_EQ(psis.size(),module.size());
    }
    ASSERT_NEAR(3.378373E+03,modules[1][0], 1E-9); // First column (because X-axis) of the first group (because incidence index is 0), second line (because period index is 1)
    ASSERT_NEAR(9.067188E+02,modules[2][1], 1E-9); // First column (because X-axis) of the second group (because incidence index is 1), third line (because period index is 2)
    ASSERT_NEAR(2.088816E+00,phases[1][0], 1E-9); // First column (because X-axis) of the first group (because incidence index is 0), second line (because period index is 1)
    ASSERT_NEAR(2.123063E+00,phases[2][1], 1E-9); // First column (because X-axis) of the second group (because incidence index is 1), third line (because period index is 2)
//! [DiffractionInterpolatorTest expected output]
}

