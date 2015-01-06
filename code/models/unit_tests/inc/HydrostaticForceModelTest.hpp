/*
 * HydrostaticForceModuleTest.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef HYDROSTATICFORCEMODELTEST_HPP_
#define HYDROSTATICFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>
#include "EnvironmentAndFrames.hpp"
#include "GeometricTypes3d.hpp"

class HydrostaticForceModelTest : public ::testing::Test
{
    protected:
        HydrostaticForceModelTest();
        virtual ~HydrostaticForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        EnvironmentAndFrames get_environment_and_frames() const;
        VectorOfVectorOfPoints get_points() const;
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* HYDROSTATICFORCEMODELTEST_HPP_ */
