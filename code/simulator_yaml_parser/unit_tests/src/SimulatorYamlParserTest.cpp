/*
 * SimulatorYamlParserTest.cpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */


#include "SimulatorYamlParserTest.hpp"
#include "yaml_data.hpp"
#include "SimulatorYamlParser.hpp"

SimulatorYamlParserTest::SimulatorYamlParserTest() : a(DataGenerator(1212)),
                                                     input(SimulatorYamlParser(test_data::full_example()).parse())
{
}

SimulatorYamlParserTest::~SimulatorYamlParserTest()
{
}

void SimulatorYamlParserTest::SetUp()
{
}

void SimulatorYamlParserTest::TearDown()
{
}

TEST_F(SimulatorYamlParserTest, can_parse_rotations)
{
    const YamlRotation rot = input.rotations;
    ASSERT_EQ("angle", rot.order_by);
    ASSERT_EQ(3, rot.convention.size());
    ASSERT_EQ("x", rot.convention.at(0));
    ASSERT_EQ("y'", rot.convention.at(1));
    ASSERT_EQ("z''", rot.convention.at(2));
}

TEST_F(SimulatorYamlParserTest, can_parse_models)
{
    const YamlModel mod1 = input.environment.at(0);
    ASSERT_EQ("no waves", mod1.model);
    ASSERT_EQ("frame: NED\nmodel: no waves\nz:\n  unit: m\n  value: 12", mod1.yaml);
    const YamlModel mod2 = input.bodies.at(0).external_forces.at(0);
    ASSERT_EQ("gravity", mod2.model);
    ASSERT_EQ("g:\n  unit: m/s^2\n  value: 9.81\nmodel: gravity", mod2.yaml);
}

TEST_F(SimulatorYamlParserTest, can_parse_environment)
{
    const std::vector<YamlModel> env = input.environment;
    ASSERT_EQ(2, env.size());
    ASSERT_EQ("no waves", env.at(0).model);
    ASSERT_EQ("no wind", env.at(1).model);
}

TEST_F(SimulatorYamlParserTest, can_parse_bodies)
{
    ASSERT_EQ(1, input.bodies.size());
    ASSERT_EQ("body 1", input.bodies.at(0).name);
    ASSERT_EQ("anthineas.stl", input.bodies.at(0).mesh);
    ASSERT_EQ(2, input.bodies.at(0).external_forces.size());
}

TEST_F(SimulatorYamlParserTest, can_parse_position_of_body_frame_relative_to_mesh)
{
    ASSERT_DOUBLE_EQ(1,input.bodies.front().position_of_body_frame_relative_to_mesh.angle.phi);
    ASSERT_DOUBLE_EQ(3,input.bodies.front().position_of_body_frame_relative_to_mesh.angle.theta);
    ASSERT_DOUBLE_EQ(2,input.bodies.front().position_of_body_frame_relative_to_mesh.angle.psi);
    ASSERT_EQ("mesh",input.bodies.front().position_of_body_frame_relative_to_mesh.frame);
    ASSERT_DOUBLE_EQ(10,input.bodies.front().position_of_body_frame_relative_to_mesh.coordinates.x);
    ASSERT_DOUBLE_EQ(0.21,input.bodies.front().position_of_body_frame_relative_to_mesh.coordinates.y);
    ASSERT_DOUBLE_EQ(33e3,input.bodies.front().position_of_body_frame_relative_to_mesh.coordinates.z);
}

TEST_F(SimulatorYamlParserTest, can_parse_initial_position_of_body_frame_relative_to_NED)
{
    ASSERT_EQ("NED",input.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.frame);
    ASSERT_DOUBLE_EQ(4,input.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.x);
    ASSERT_DOUBLE_EQ(8,input.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.y);
    ASSERT_DOUBLE_EQ(12,input.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.z);
    ASSERT_DOUBLE_EQ(13,input.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.phi);
    ASSERT_DOUBLE_EQ(14,input.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.theta);
    ASSERT_DOUBLE_EQ(15,input.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.psi);
}


TEST_F(SimulatorYamlParserTest, can_parse_initial_velocity_of_body_frame_relative_to_NED)
{
    ASSERT_EQ("body 1",input.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.frame);
    ASSERT_DOUBLE_EQ(-8,input.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.u);
    ASSERT_DOUBLE_EQ(-9,input.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.v);
    ASSERT_DOUBLE_EQ(14,input.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.w);
    ASSERT_DOUBLE_EQ(56,input.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.p);
    ASSERT_DOUBLE_EQ(7,input.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.q);
    ASSERT_DOUBLE_EQ(6,input.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.r);
}

TEST_F(SimulatorYamlParserTest, can_parse_centre_of_inertia)
{
    ASSERT_DOUBLE_EQ(4,input.bodies.front().dynamics.position_of_centre_of_inertia.x);
    ASSERT_DOUBLE_EQ(7,input.bodies.front().dynamics.position_of_centre_of_inertia.y);
    ASSERT_DOUBLE_EQ(-10,input.bodies.front().dynamics.position_of_centre_of_inertia.z);
    ASSERT_EQ("body 1",input.bodies.front().dynamics.position_of_centre_of_inertia.frame);
    ASSERT_EQ("centre of inertia",input.bodies.front().dynamics.position_of_centre_of_inertia.name);
}
