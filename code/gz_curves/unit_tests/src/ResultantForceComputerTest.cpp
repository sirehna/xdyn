/*
 * ResultantForceComputerTest.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#include "make_sim_for_GZ.hpp"
#include "ResultantForceComputerTest.hpp"
#include "ResultantForceComputer.hpp"
#include "GZTypes.hpp"
#include "STL_data.hpp"
#include "yaml_data.hpp"


Sim ResultantForceComputerTest::sim = GZ::make_sim(test_data::stable_cube_example(), test_data::cube());

ResultantForceComputerTest::ResultantForceComputerTest() : a(ssc::random_data_generator::DataGenerator(87878899))
{
}

ResultantForceComputerTest::~ResultantForceComputerTest()
{
}

void ResultantForceComputerTest::SetUp()
{
}

void ResultantForceComputerTest::TearDown()
{
}

TEST_F(ResultantForceComputerTest, can_compute_resultant_force_for_a_cube)
{
    const GZ::ResultantForceComputer sum_of_forces(sim);
    const auto F = sum_of_forces(GZ::State(0,0,0));
    ASSERT_DOUBLE_EQ(0, F.z);
    ASSERT_DOUBLE_EQ(0, F.theta);
    ASSERT_DOUBLE_EQ(0, F.phi);
}



