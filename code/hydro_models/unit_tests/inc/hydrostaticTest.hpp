/*
 * hydrostaticTest.hpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#ifndef HYDROSTATICTEST_HPP_
#define HYDROSTATICTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>
#include "GeometricTypes3d.hpp"

class hydrostaticTest : public ::testing::Test
{
    protected:
        hydrostaticTest();
        virtual ~hydrostaticTest();
        virtual void SetUp();
        virtual void TearDown();
        Matrix3x get_rectangle(const double w, const double h) const;
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* HYDROSTATICTEST_HPP_ */
