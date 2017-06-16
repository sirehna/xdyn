#ifndef SURFACEELEVATIONFROMWAVESTEST_HPP_
#define SURFACEELEVATIONFROMWAVESTEST_HPP_

#include "gtest/gtest.h"
#include "Airy.hpp"
#include <ssc/random_data_generator.hpp>
#include <ssc/macros.hpp>

#include TR1INC(memory)

class SurfaceElevationFromWavesTest : public ::testing::Test
{
    protected:
        SurfaceElevationFromWavesTest();
        virtual ~SurfaceElevationFromWavesTest();
        virtual void SetUp();
        virtual void TearDown();
        TR1(shared_ptr)<WaveModel> get_model() const;
        TR1(shared_ptr)<WaveModel> get_model(const size_t nfreq) const;
        TR1(shared_ptr)<WaveModel> get_model(const double Hs, const double Tp, const double phase, const double water_depth) const;
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* SURFACEELEVATIONFROMWAVESTEST_HPP_ */
