/*
 * GRPCForceModelTest.hpp
 *
 *  Created on: Jun 17, 2019
 *      Author: cady
 */

#ifndef GRPC_UNIT_TESTS_INC_GRPCFORCEMODELTEST_HPP_
#define GRPC_UNIT_TESTS_INC_GRPCFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class GRPCForceModelTest : public ::testing::Test
{
    protected:
        GRPCForceModelTest();
        virtual ~GRPCForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
};


#endif /* GRPC_UNIT_TESTS_INC_GRPCFORCEMODELTEST_HPP_ */
