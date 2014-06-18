/*
 * update_kinematicsTests.hpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#ifndef UPDATE_KINEMATICSTESTS_HPP_
#define UPDATE_KINEMATICSTESTS_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class update_kinematicsTests : public ::testing::Test
{
    protected:
        update_kinematicsTests();
	    virtual ~update_kinematicsTests();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* UPDATE_KINEMATICSTESTS_HPP_ */
