/*
 * KinematicsModuleTest.hpp
 *
 *  Created on: Apr 25, 2014
 *      Author: cady
 */

#ifndef KINEMATICSMODULETEST_HPP_
#define KINEMATICSMODULETEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class KinematicsModuleTest : public ::testing::Test
{
    protected:
        KinematicsModuleTest();
	    virtual ~KinematicsModuleTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* KINEMATICSMODULETEST_HPP_ */
