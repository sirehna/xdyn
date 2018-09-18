/*
 * ConstantForceModelTest.hpp
 *
 *  Created on: Sept 7, 2018
 *      Author: cady
 */

#ifndef CONSTANTFORCEMODELTEST_HPP_
#define CONSTANTFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>
#include "ConstantForceModel.hpp"

class ConstantForceModelTest : public ::testing::Test
{
    protected:
        ConstantForceModelTest();
        virtual ~ConstantForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        ssc::random_data_generator::DataGenerator a;
        BodyStates get_states(const double phi, const double theta, const double psi, EnvironmentAndFrames& env) const;
        ConstantForceModel get_constant_force(EnvironmentAndFrames& env) const;
        EnvironmentAndFrames get_env() const;
        BodyPtr get_body(const std::string& name) const;
};

#endif  /* CONSTANTFORCEMODELTEST_HPP_ */
