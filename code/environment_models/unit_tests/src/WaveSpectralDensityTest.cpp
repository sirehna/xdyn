/*
 * WaveSpectralDensityTest.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: cady
 */


#include "WaveSpectralDensityTest.hpp"
#include "WaveSpectralDensity.hpp"

WaveSpectralDensityTest::WaveSpectralDensityTest() : a(ssc::random_data_generator::DataGenerator(3215))
{
}

WaveSpectralDensityTest::~WaveSpectralDensityTest()
{
}

void WaveSpectralDensityTest::SetUp()
{
}

void WaveSpectralDensityTest::TearDown()
{
}

class DummyWaveSpectralDensity : public WaveSpectralDensity
{
    public:
        double operator()(const double omega) const
        {
            return omega;
        }
        WaveSpectralDensity* clone() const
        {
            return new DummyWaveSpectralDensity(*this);
        }
};

TEST_F(WaveSpectralDensityTest, bug_detected_by_FS)
{
    DummyWaveSpectralDensity wsd;
    const auto omega = wsd.get_angular_frequencies(1,1,1);
    ASSERT_EQ(1, omega.size());
    ASSERT_DOUBLE_EQ(1, omega.front());
}


