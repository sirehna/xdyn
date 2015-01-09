/*
 * SimulatorBuilderTest.hpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#ifndef SIMULATORBUILDERTEST_HPP_
#define SIMULATORBUILDERTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator.hpp>
#include "YamlSimulatorInput.hpp"
#include "SimulatorBuilder.hpp"

class SimulatorBuilderTest : public ::testing::Test
{
    protected:
        SimulatorBuilderTest();
        virtual ~SimulatorBuilderTest();
        virtual void SetUp();
        virtual void TearDown();
        std::string customize(const std::string& body_name, const std::string& something) const;
        std::vector<BodyPtr> get_body_vector(const std::string& name) const;

        ssc::random_data_generator::DataGenerator a;
        static const YamlSimulatorInput input;
        SimulatorBuilder builder;
};

#endif  /* SIMULATORBUILDERTEST_HPP_ */
