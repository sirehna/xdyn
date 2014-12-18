/*
 * h5_interface_tests.hpp
 *
 *  Created on: Dec 15, 2014
 *      Author: jacquenot
 */

#ifndef H5_INTERFACE_TESTS_HPP
#define H5_INTERFACE_TESTS_HPP

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>

class H5InterfaceTest : public ::testing::Test
{
    protected:
        H5InterfaceTest();
        virtual ~H5InterfaceTest(){}
        virtual void SetUp(){}
        virtual void TearDown(){}
        ssc::random_data_generator::DataGenerator a;
};

#endif

