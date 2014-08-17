#ifndef DEFAULTSURFACELEVATIONTEST_HPP_
#define DEFAULTSURFACELEVATIONTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class DefaultSurfaceElevationTest : public ::testing::Test
{
    protected:
        DefaultSurfaceElevationTest();
        virtual ~DefaultSurfaceElevationTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;

};

#endif  /* DEFAULTSURFACELEVATIONTEST_HPP_ */
