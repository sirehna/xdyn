/*
 * ResultantForceComputerTest.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#include <set>

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

TEST_F(ResultantForceComputerTest, sim_only_contains_gravity_and_hydrostatic_forces)
{
    const auto s = GZ::make_sim(test_data::stable_cube_example(), test_data::cube());

    std::map<std::string,std::vector<ForcePtr> > forces = s.get_forces();
    std::set<std::string> force_models;
    for (auto force:forces["cube"])
    {
        force_models.insert(force->get_name());
    }
    ASSERT_EQ(2, force_models.size());
    const bool has_gravity = force_models.find("gravity") != force_models.end();
    const bool has_hydrostatic = force_models.find("non-linear hydrostatic (exact)") != force_models.end();
    ASSERT_TRUE(has_gravity);
    ASSERT_TRUE(has_hydrostatic);
}

TEST_F(ResultantForceComputerTest, can_compute_resultant_force_for_a_cube)
{
    const GZ::ResultantForceComputer sum_of_forces(sim);
    const auto F = sum_of_forces(GZ::State(0,0,0));
    ASSERT_DOUBLE_EQ(0, F.z);
    ASSERT_DOUBLE_EQ(0, F.theta);
    ASSERT_DOUBLE_EQ(0, F.phi);
}



