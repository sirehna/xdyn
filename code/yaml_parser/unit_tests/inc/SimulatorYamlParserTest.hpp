#ifndef SIMULATORYAMLPARSERTEST_HPP_
#define SIMULATORYAMLPARSERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>
#include "YamlSimulatorInput.hpp"
#include "YamlWaveModelInput.hpp"

class SimulatorYamlParserTest : public ::testing::Test
{
    protected:
        SimulatorYamlParserTest();
        virtual ~SimulatorYamlParserTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
        static const YamlSimulatorInput yaml;
        static const YamlSimulatorInput old_yaml;
};

#endif  /* SIMULATORYAMLPARSERTEST_HPP_ */
