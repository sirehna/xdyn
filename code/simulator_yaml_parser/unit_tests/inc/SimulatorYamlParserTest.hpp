#ifndef SIMULATORYAMLPARSERTEST_HPP_
#define SIMULATORYAMLPARSERTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"

class SimulatorYamlParserTest : public ::testing::Test
{
    protected:
        SimulatorYamlParserTest();
	    virtual ~SimulatorYamlParserTest();
        virtual void SetUp();
        virtual void TearDown();
		DataGenerator a;

};

#endif  /* SIMULATORYAMLPARSERTEST_HPP_ */
