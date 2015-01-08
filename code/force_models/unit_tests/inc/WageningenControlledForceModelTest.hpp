/*
 * WageningenControlledForceModelTest.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef WAGENINGENCONTROLLEDFORCEMODELTEST_HPP_
#define WAGENINGENCONTROLLEDFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class WageningenControlledForceModelTest : public ::testing::Test
{
    protected:
        WageningenControlledForceModelTest();
        virtual ~WageningenControlledForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* WAGENINGENCONTROLLEDFORCEMODELTEST_HPP_ */
