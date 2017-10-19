/*
 * ForceTests.hpp
 *
 *  Created on: Feb 16, 2015
 *      Author: cady
 */


#ifndef FORCETESTS_HPP_
#define FORCETESTS_HPP_

#include "gtest/gtest.h"
#include <ssc/random_data_generator/DataGenerator.hpp>
#include "DiffractionForceModel.hpp"
struct YamlModel;

class ForceTests : public ::testing::Test
{
    protected:
        ForceTests();
        virtual ~ForceTests();
        virtual void SetUp();
        virtual void TearDown();
        DiffractionForceModel get_diffraction_force_model(const YamlModel& waves, const std::string& diffraction_yaml, const std::string& hdb_file_contents);
        ssc::random_data_generator::DataGenerator a;
};

#endif  /* FORCETESTS_HPP_ */
