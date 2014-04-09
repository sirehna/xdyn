#ifndef TRANSFORMTEST_HPP_
#define TRANSFORMTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"
#include "Point.hpp"

class TransformTest : public ::testing::Test
{
    protected:
        TransformTest();
        virtual ~TransformTest();
        virtual void SetUp();
        virtual void TearDown();
        Point random_point_in_frame(const std::string& frame) const;
        Point random_point() const;
        
        DataGenerator a;
};

#endif  /* TRANSFORMTEST_HPP_ */
