/*
 * PolygonTest.hpp
 *
 *  Created on: Aug 27, 2014
 *      Author: cady
 */

#ifndef POLYGONTEST_HPP_
#define POLYGONTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"
#include "GeometricTypes3d.hpp"

class PolygonTest : public ::testing::Test
{
    protected:
        PolygonTest();
        virtual ~PolygonTest();
        virtual void SetUp();
        virtual void TearDown();
        Matrix3x get_rectangle(const double w, const double h) const;

        DataGenerator a;

};

#endif  /* POLYGONTEST_HPP_ */
