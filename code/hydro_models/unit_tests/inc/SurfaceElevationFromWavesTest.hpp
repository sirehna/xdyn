#ifndef SURFACEELEVATIONFROMWAVESTEST_HPP_
#define SURFACEELEVATIONFROMWAVESTEST_HPP_

#include "gtest/gtest.h"
#include "Airy.hpp"
#include "DataGenerator.hpp"

#include "tr1_macros.hpp"
#include TR1INC(memory)

class SurfaceElevationFromWavesTest : public ::testing::Test
{
    protected:
        SurfaceElevationFromWavesTest();
        virtual ~SurfaceElevationFromWavesTest();
        virtual void SetUp();
        virtual void TearDown();
        TR1(shared_ptr)<WaveModel> get_model() const;
        DataGenerator a;
};

#endif  /* SURFACEELEVATIONFROMWAVESTEST_HPP_ */
