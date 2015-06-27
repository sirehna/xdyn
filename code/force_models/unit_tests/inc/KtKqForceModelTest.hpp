/*
 * KtKqForceModelTest.hpp
 *
 *  Created on: Jun 27, 2015
 *      Author: cady
 */

#ifndef KTKQFORCEMODELTEST_HPP_
#define KTKQFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class KtKqForceModelTest : public ::testing::Test
{
    protected:
        KtKqForceModelTest();
        virtual ~KtKqForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* KTKQFORCEMODELTEST_HPP_ */
