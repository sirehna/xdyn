/*
 * StateDerivativesModuleTest.hpp
 *
 *  Created on: May 29, 2014
 *      Author: cady
 */

#ifndef STATEDERIVATIVESMODULETEST_HPP_
#define STATEDERIVATIVESMODULETEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class StateDerivativesModuleTest : public ::testing::Test
{
    protected:
        StateDerivativesModuleTest();
	    virtual ~StateDerivativesModuleTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* STATEDERIVATIVESMODULETEST_HPP_ */
