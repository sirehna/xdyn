#ifndef DEFAULTSURFACELEVATIONTEST_HPP_
#define DEFAULTSURFACELEVATIONTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class DefaultSurfaceElevationTest : public ::testing::Test
{
    protected:
        DefaultSurfaceElevationTest();
        virtual ~DefaultSurfaceElevationTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;

};

#endif  /* DEFAULTSURFACELEVATIONTEST_HPP_ */
