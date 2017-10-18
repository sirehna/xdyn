#ifndef SURFACEELEVATIONFROMWAVESTEST_HPP_
#define SURFACEELEVATIONFROMWAVESTEST_HPP_

#include "gtest/gtest.h"
#include "WaveModel.hpp"
#include <ssc/random_data_generator.hpp>

class SurfaceElevationFromWavesTest : public ::testing::Test
{
    protected:
        SurfaceElevationFromWavesTest();
        virtual ~SurfaceElevationFromWavesTest();
        virtual void SetUp();
        virtual void TearDown();
        WaveModelPtr get_model() const;
        WaveModelPtr get_model(const size_t nfreq) const;
        WaveModelPtr get_model(const double Hs, const double Tp, const double phase, const double water_depth) const;
        WaveModelPtr get_model(const double psi0, const double Hs, const double Tp, const double phase, const double water_depth, const double omega_min, const double omega_max, const size_t nfreq) const;
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* SURFACEELEVATIONFROMWAVESTEST_HPP_ */
