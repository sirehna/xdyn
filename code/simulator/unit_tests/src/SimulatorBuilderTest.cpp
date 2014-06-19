/*
 * SimulatorBuilderTest.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#include "SimulatorBuilderTest.hpp"
#include "SimulatorBuilder.hpp"
#include "SimulatorYamlParser.hpp"
#include "SimulatorBuilderException.hpp"
#include "TriMeshTestData.hpp"
#include "yaml_data.hpp"
#include "generate_body_for_tests.hpp"
#include "Kinematics.hpp"
#include "Transform.hpp"

SimulatorBuilderTest::SimulatorBuilderTest() : a(DataGenerator(1212))
{
}

SimulatorBuilderTest::~SimulatorBuilderTest()
{
}

void SimulatorBuilderTest::SetUp()
{
}

void SimulatorBuilderTest::TearDown()
{
}

TEST_F(SimulatorBuilderTest, throws_if_cannot_find_mesh)
{
    const auto input = SimulatorYamlParser(test_data::falling_ball_example()).parse();
    SimulatorBuilder builder(input);
    ASSERT_THROW(builder.get_bodies(MeshMap()),SimulatorBuilderException);
}

TEST_F(SimulatorBuilderTest, can_get_bodies)
{
    const auto input = SimulatorYamlParser(test_data::falling_ball_example()).parse();
    SimulatorBuilder builder(input);
    MeshMap m;
    m["ball"] = two_triangles();
    const auto bodies = builder.get_bodies(m);
    ASSERT_EQ(1, bodies.size());
    ASSERT_EQ("ball", bodies.front().name);
    const auto Id = (*bodies.front().inverse_of_the_total_inertia)*(*bodies.front().total_inertia);
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        for (size_t j = 0 ; j < 6 ; ++j)
        {
            ASSERT_DOUBLE_EQ(Id(i,j), i==j) << "i = " << i << ", j = " << j;
        }
    }
}

TEST_F(SimulatorBuilderTest, can_get_rho_and_g)
{
    const auto input = SimulatorYamlParser(test_data::full_example()).parse();
    SimulatorBuilder builder(input);
    const auto env = builder.get_environment_and_frames(std::vector<Body>());
    ASSERT_DOUBLE_EQ(9.81,env.g);
    ASSERT_DOUBLE_EQ(1000,env.rho);
}

std::string SimulatorBuilderTest::customize(const std::string& body_name, const std::string& something) const
{
    return something + "(" + body_name + ")";
}

TEST_F(SimulatorBuilderTest, kinematics_contains_body_to_mesh_transform)
{
    const auto input = SimulatorYamlParser(test_data::full_example()).parse();
    SimulatorBuilder builder(input);
    std::vector<Body> bodies;
    for (size_t i = 0 ; i < 10 ; ++i) bodies.push_back(get_body(a.random<std::string>()));
    const auto env = builder.get_environment_and_frames(bodies);
    ASSERT_TRUE(env.k.get() != NULL);
    for (const auto body:bodies)
    {
        ASSERT_NO_THROW(env.k->get(body.name, customize(body.name, "mesh")));
    }
}

TEST_F(SimulatorBuilderTest, kinematics_contains_ned_to_body_transform)
{
    const auto input = SimulatorYamlParser(test_data::full_example()).parse();
    SimulatorBuilder builder(input);
    std::vector<Body> bodies;
    for (size_t i = 0 ; i < 10 ; ++i) bodies.push_back(get_body(a.random<std::string>()));
    const auto env = builder.get_environment_and_frames(bodies);
    ASSERT_TRUE(env.k.get() != NULL);
    for (const auto body:bodies)
    {
        ASSERT_NO_THROW(env.k->get("NED", body.name));
    }
}
