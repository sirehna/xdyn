/*
 * RadiationInterpolatorTest.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: cady
 */

#include <algorithm> //std::reverse

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "HDBParser.hpp"
#include "HDBData.hpp"
#include "DiffractionInterpolatorTest.hpp"
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
    const HDBData data((HDBParser(test_data::anthineas_hdb())));
    std::vector<double> omegas = {1,2,3,3.5,3.8,4};
    std::vector<double> psis = {0,15,30,45,60,75,90,105,120,135,150,165,180,195};
    const bool mirror = true;
    for (size_t i = 0 ; i < omegas.size() ; ++i) omegas[i] = 2*PI/omegas[i];
    std::reverse(omegas.begin(),omegas.end());
    for (size_t i = 0 ; i < psis.size() ; ++i) psis[i] *= PI/180.;
    DiffractionInterpolator radiation(data,omegas,psis,mirror);
    const size_t k = 0;
    const std::vector<std::vector<double> > modules = radiation.get_modules(k);
    const std::vector<std::vector<double> > phases = radiation.get_phases(k);
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
    ASSERT_DOUBLE_EQ(2.231911E+04,modules[1][0]);
    ASSERT_DOUBLE_EQ(1.347913E+05,modules[3][2]);
    ASSERT_DOUBLE_EQ(3.997016E-01,phases[1][0]);
    ASSERT_DOUBLE_EQ(3.042103E+00,phases[3][2]);
//! [DiffractionInterpolatorTest expected output]
}

