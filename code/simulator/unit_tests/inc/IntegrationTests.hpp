/*
 * IntegrationTests.hpp
 *
 *  Created on: Jun 5, 2014
 *      Author: cady
 */

#ifndef INTEGRATIONTESTS_HPP_
#define INTEGRATIONTESTS_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class IntegrationTests : public ::testing::Test
{
    protected:
        IntegrationTests();
	    virtual ~IntegrationTests();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* INTEGRATIONTESTS_HPP_ */
