/*
 * parse_historyTest.hpp
 *
 *  Created on: 23 oct. 2015
 *      Author: sirehna
 */


#ifndef YAML_PARSER_UNIT_TESTS_INC_PARSE_HISTORYTEST_HPP_
#define YAML_PARSER_UNIT_TESTS_INC_PARSE_HISTORYTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

class parse_historyTest : public ::testing::Test
{
    protected:
        parse_historyTest();
        virtual ~parse_historyTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* YAML_PARSER_UNIT_TESTS_INC_PARSE_HISTORYTEST_HPP_ */
