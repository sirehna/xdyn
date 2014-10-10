/*
 * OutputTransformerTest.hpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */

#ifndef OUTPUTTRANSFORMERRTEST_HPP_
#define OUTPUTTRANSFORMERRTEST_HPP_

#include <map>

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>
#include "YamlSimulatorInput.hpp"

class OutputTransformerTest : public ::testing::Test
{
    protected:
        OutputTransformerTest();
        virtual ~OutputTransformerTest();
        virtual void SetUp();
        virtual void TearDown();
        static YamlSimulatorInput get_falling_ball_yaml();
        static YamlSimulatorInput get_full_example_yaml();

        static const YamlSimulatorInput falling_ball_yaml;
        static const YamlSimulatorInput full_example_yaml;

        ssc::random_data_generator::DataGenerator a;
        std::vector<std::map<std::string,double> > falling_ball;
        std::vector<std::map<std::string,double> > full_example;

};

#endif  /* OUTPUTTRANSFORMERRTEST_HPP_ */
