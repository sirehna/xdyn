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
        static YamlSimulatorInput get_yaml();

        static const YamlSimulatorInput yaml;

        DataGenerator a;
        std::vector<std::map<std::string,double> > out;

};

#endif  /* OUTPUTTRANSFORMERRTEST_HPP_ */
