/*
 * hydrostaticTest.hpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#ifndef HYDROSTATICTEST_HPP_
#define HYDROSTATICTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"
#include "GeometricTypes3d.hpp"

class hydrostaticTest : public ::testing::Test
{
    protected:
        hydrostaticTest();
	    virtual ~hydrostaticTest();
        virtual void SetUp();
        virtual void TearDown();
        Matrix3x get_rectangle(const double w, const double h) const;
		DataGenerator a;

};

#endif  /* HYDROSTATICTEST_HPP_ */
