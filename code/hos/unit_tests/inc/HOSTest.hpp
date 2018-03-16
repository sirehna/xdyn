/*
 * HOSTest.hpp
 *
 *  Created on: Nov 22, 2017
 *      Author: cady
 */

#ifndef HOS_UNIT_TESTS_INC_HOSTEST_HPP_
#define HOS_UNIT_TESTS_INC_HOSTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class HOSTest : public ::testing::Test
{
    protected:
        HOSTest();
        virtual ~HOSTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};


#endif /* HOS_UNIT_TESTS_INC_HOSTEST_HPP_ */
