/*
 * parse_commandsTest.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef PARSE_COMMANDSTEST_HPP_
#define PARSE_COMMANDSTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class parse_commandsTest : public ::testing::Test
{
    protected:
        parse_commandsTest();
        virtual ~parse_commandsTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* PARSE_COMMANDSTEST_HPP_ */
