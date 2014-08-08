#ifndef SIMULATORYAMLPARSERTEST_HPP_
#define SIMULATORYAMLPARSERTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"
#include "YamlSimulatorInput.hpp"
#include "YamlWaveModelInput.hpp"

class SimulatorYamlParserTest : public ::testing::Test
{
    protected:
        SimulatorYamlParserTest();
        virtual ~SimulatorYamlParserTest();
        virtual void SetUp();
        virtual void TearDown();
        DataGenerator a;
        static const YamlSimulatorInput yaml;
};

#endif  /* SIMULATORYAMLPARSERTEST_HPP_ */
