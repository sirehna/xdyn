/*
 * SimulatorBuilderTest.hpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#ifndef SIMULATORBUILDERTEST_HPP_
#define SIMULATORBUILDERTEST_HPP_

#include "gtest/gtest.h"
#include "DataGenerator.hpp"
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

        DataGenerator a;
        YamlSimulatorInput input;
        SimulatorBuilder builder;

};

#endif  /* SIMULATORBUILDERTEST_HPP_ */
