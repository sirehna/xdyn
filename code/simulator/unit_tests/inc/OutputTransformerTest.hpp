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
#include "DataGenerator.hpp"
#include "YamlSimulatorInput.hpp"

class OutputTransformerTest : public ::testing::Test
{
    protected:
        OutputTransformerTest();
        virtual ~OutputTransformerTest();
        virtual void SetUp();
        virtual void TearDown();
        static YamlSimulatorInput get_yaml1();
        static YamlSimulatorInput get_yaml2();

        static const YamlSimulatorInput yaml1;
        static const YamlSimulatorInput yaml2;

        DataGenerator a;
        std::vector<std::map<std::string,double> > out1;
        std::vector<std::map<std::string,double> > out2;

};

#endif  /* OUTPUTTRANSFORMERRTEST_HPP_ */
