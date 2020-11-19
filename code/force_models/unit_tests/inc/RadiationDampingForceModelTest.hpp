/*
 * RadiationDampingForceModelTest.hpp
 *
 *  Created on: Dec 1, 2014
 *      Author: cady
 */


#ifndef RADIATIONDAMPINGFORCEMODELTEST_HPP_
#define RADIATIONDAMPINGFORCEMODELTEST_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "Body.hpp"
#include "HDBParserForTests.hpp"
#include "YamlRadiationDamping.hpp"

class RadiationDampingForceModelTest : public ::testing::Test
{
    protected:
        RadiationDampingForceModelTest();
        virtual ~RadiationDampingForceModelTest();
        virtual void SetUp();
        virtual void TearDown();
        TR1(shared_ptr)<HDBParser> get_hdb_data(const bool only_diagonal_terms=true) const;
        YamlRadiationDamping get_yaml_data(const bool show_debug) const;

        ssc::random_data_generator::DataGenerator a;
};

#endif  /* RADIATIONDAMPINGFORCEMODELTEST_HPP_ */
