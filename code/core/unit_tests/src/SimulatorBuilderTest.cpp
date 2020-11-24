/*
 * SimulatorBuilderTest.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#include "SimulatorBuilderTest.hpp"
#include "SimulatorBuilder.hpp"
#include "InternalErrorException.hpp"
#include "TriMeshTestData.hpp"
#include "generate_body_for_tests.hpp"
#include <ssc/kinematics.hpp>
#include "DefaultSurfaceElevation.hpp"
#include "DefaultWindModel.hpp"
#include "SimulatorYamlParser.hpp"
#include "yaml_data.hpp"

const YamlSimulatorInput SimulatorBuilderTest::input = SimulatorYamlParser(test_data::full_example()).parse();


SimulatorBuilderTest::SimulatorBuilderTest() : a(ssc::random_data_generator::DataGenerator(1212)),
                                               builder(SimulatorBuilder(input, 0))
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
    ASSERT_THROW(builder.get_bodies(MeshMap(), std::vector<bool>(1,false), std::map<std::string,double>()),InternalErrorException);
}

TEST_F(SimulatorBuilderTest, can_get_bodies)
{
    MeshMap m;
    m[input.bodies.front().name] = two_triangles();
    const auto bodies = builder.get_bodies(m, std::vector<bool>(1,false), std::map<std::string,double>());
    ASSERT_EQ(1, bodies.size());
    ASSERT_EQ(input.bodies.front().name, bodies.front()->get_name());
    const auto states = bodies.front()->get_states();
    const auto Id = (*states.inverse_of_the_total_inertia)*(*states.total_inertia);
    for (int i = 0 ; i < 6 ; ++i)
    {
        for (int j = 0 ; j < 6 ; ++j)
        {
            ASSERT_NEAR(Id(i,j), i==j, 1E-10) << "i = " << i << ", j = " << j;
        }
    }
}

TEST_F(SimulatorBuilderTest, can_get_rho_and_g)
{
    builder.can_parse<DefaultSurfaceElevation>();
    builder.can_parse<DefaultWindModel>();
    const auto env = builder.get_environment();
    ASSERT_DOUBLE_EQ(9.81,env.g);
    ASSERT_DOUBLE_EQ(1000,env.rho);
}

std::string SimulatorBuilderTest::customize(const std::string& body_name, const std::string& something) const
{
    return something + "(" + body_name + ")";
}

std::vector<BodyPtr> SimulatorBuilderTest::get_body_vector(const std::string& name) const
{
    return std::vector<BodyPtr>(1, get_body(name));
}

TEST_F(SimulatorBuilderTest, kinematics_contains_body_to_mesh_transform)
{
    builder.can_parse<DefaultSurfaceElevation>();
    builder.can_parse<DefaultWindModel>();
    const auto bodies = get_body_vector(a.random<std::string>());

    ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics());
    builder.add_initial_transforms(bodies,k);
    ASSERT_TRUE(k.get() != NULL);
    for (auto that_body = bodies.begin() ; that_body != bodies.end() ; ++that_body)
    {
        ASSERT_NO_THROW(k->get((*that_body)->get_name(), customize((*that_body)->get_name(), "mesh")));
    }
}

TEST_F(SimulatorBuilderTest, kinematics_contains_ned_to_body_transform)
{
    builder.can_parse<DefaultSurfaceElevation>();
    builder.can_parse<DefaultWindModel>();
    const auto bodies = get_body_vector(a.random<std::string>());
    ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics());
    builder.add_initial_transforms(bodies,k);
    ASSERT_TRUE(k.get() != NULL);
    for (auto that_body = bodies.begin() ; that_body != bodies.end() ; ++that_body)
    {
        ASSERT_NO_THROW(k->get("NED", (*that_body)->get_name()));
    }
}

TEST_F(SimulatorBuilderTest, should_throw_if_no_wave_parser_defined)
{
    ASSERT_THROW(builder.get_environment(), InternalErrorException);
}

TEST_F(SimulatorBuilderTest, should_throw_if_attempting_to_define_wave_model_twice)
{
    YamlModel model;
    model.model = "no waves";
    model.yaml = "constant sea elevation in NED frame:\n   unit: m\n   value: 12";
    auto input2 = input;
    input2.environment.push_back(model);
    SimulatorBuilder builder2(input2, 0);
    builder2.can_parse<DefaultSurfaceElevation>();
    builder2.can_parse<DefaultWindModel>();
    ASSERT_THROW(builder2.get_environment(), InternalErrorException);
}

TEST_F(SimulatorBuilderTest, get_forces_should_throw_if_there_is_anything_it_cannot_parse)
{
    builder.can_parse<DefaultSurfaceElevation>();
    builder.can_parse<DefaultWindModel>();
    MeshMap m;
    const std::string name = input.bodies.front().name;
    m[name] = two_triangles();
    const auto bodies = builder.get_bodies(m, std::vector<bool>(1,false), std::map<std::string,double>());
    const auto env = builder.get_environment();
    ASSERT_THROW(builder.get_forces(env), InvalidInputException);
}
