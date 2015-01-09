/*
 * BodyBuilderTest.hpp
 *
 *  Created on: Jun 18, 2014
 *      Author: cady
 */

#ifndef BODYBUILDERTEST_HPP_
#define BODYBUILDERTEST_HPP_

#include "gtest/gtest.h"
#include "Body.hpp"
#include <ssc/random_data_generator.hpp>

class BodyBuilderTest : public ::testing::Test
{
    public:
        static BodyPtr build_body(const size_t idx=0);

    protected:
        BodyBuilderTest();
        virtual ~BodyBuilderTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
        static const BodyPtr body;
};

#endif  /* BODYBUILDERTEST_HPP_ */
