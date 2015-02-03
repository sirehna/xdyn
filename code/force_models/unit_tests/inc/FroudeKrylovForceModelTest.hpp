/*
 * FroudeKrylovForceModelTest.hpp
 *
 *  Created on: Oct 3, 2014
 *      Author: cady
 */

#ifndef FROUDEKRYLOVFORCEMODELTEST_HPP_
#define FROUDEKRYLOVFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>
#include "EnvironmentAndFrames.hpp"
#include "GeometricTypes3d.hpp"
#include <ssc/macros.hpp>
#include "WaveModel.hpp"
#include TR1INC(memory)

class FroudeKrylovForceModelTest : public ::testing::Test
{
    protected:
        FroudeKrylovForceModelTest();
        virtual ~FroudeKrylovForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        VectorOfVectorOfPoints get_points() const;
        TR1(shared_ptr)<WaveModel> get_wave_model() const;
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* FROUDEKRYLOVFORCEMODELTEST_HPP_ */
