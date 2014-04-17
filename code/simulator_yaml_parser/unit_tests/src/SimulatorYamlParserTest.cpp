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

TEST_F(SimulatorYamlParserTest, can_parse_environment)
{
    ASSERT_EQ(2, input.environment.size());
    const std::vector<YamlModel> env = input.environment;
    ASSERT_EQ("no waves", env.at(0).model);
    ASSERT_EQ("no wind", env.at(1).model);
    ASSERT_EQ("frame: NED\nmodel: no waves\nz:\n  unit: m\n  value: 12", env.at(0).yaml);
}

TEST_F(SimulatorYamlParserTest, can_parse_external_forces)
{
    ASSERT_EQ(2, input.bodies.at(0).external_forces.size());
    const YamlModel external_forces = input.bodies.at(0).external_forces.at(0);
    ASSERT_EQ("gravity", external_forces.model);
    ASSERT_EQ("g:\n  unit: m/s^2\n  value: 9.81\nmodel: gravity", external_forces.yaml);
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
    ASSERT_DOUBLE_EQ(4,input.bodies.front().dynamics.centre_of_inertia.x);
    ASSERT_DOUBLE_EQ(7,input.bodies.front().dynamics.centre_of_inertia.y);
    ASSERT_DOUBLE_EQ(-10,input.bodies.front().dynamics.centre_of_inertia.z);
    ASSERT_EQ("body 1",input.bodies.front().dynamics.centre_of_inertia.frame);
    ASSERT_EQ("centre of inertia",input.bodies.front().dynamics.centre_of_inertia.name);
}

TEST_F(SimulatorYamlParserTest, can_parse_mass)
{
    ASSERT_DOUBLE_EQ(1E6, input.bodies.front().dynamics.mass);
}

TEST_F(SimulatorYamlParserTest, can_parse_inertia_matrix)
{
    const YamlInertiaMatrix M =  input.bodies.front().dynamics.inertia_matrix_divided_by_mass;
    ASSERT_EQ("body 1", M.frame);
    ASSERT_EQ(6, M.row_1.size());
    ASSERT_EQ(6, M.row_2.size());
    ASSERT_EQ(6, M.row_3.size());
    ASSERT_EQ(6, M.row_4.size());
    ASSERT_EQ(6, M.row_5.size());
    ASSERT_EQ(6, M.row_6.size());
    ASSERT_DOUBLE_EQ(1, M.row_1.at(0));
    ASSERT_DOUBLE_EQ(2, M.row_1.at(1));
    ASSERT_DOUBLE_EQ(3, M.row_1.at(2));
    ASSERT_DOUBLE_EQ(4, M.row_1.at(3));
    ASSERT_DOUBLE_EQ(5, M.row_1.at(4));
    ASSERT_DOUBLE_EQ(6, M.row_1.at(5));
    ASSERT_DOUBLE_EQ(7, M.row_2.at(0));
    ASSERT_DOUBLE_EQ(2, M.row_2.at(1));
    ASSERT_DOUBLE_EQ(1, M.row_2.at(2));
    ASSERT_DOUBLE_EQ(2, M.row_2.at(3));
    ASSERT_DOUBLE_EQ(11, M.row_2.at(4));
    ASSERT_DOUBLE_EQ(13, M.row_2.at(5));
    ASSERT_DOUBLE_EQ(20, M.row_3.at(0));
    ASSERT_DOUBLE_EQ(30, M.row_3.at(1));
    ASSERT_DOUBLE_EQ(3, M.row_3.at(2));
    ASSERT_DOUBLE_EQ(80, M.row_3.at(3));
    ASSERT_DOUBLE_EQ(0.9, M.row_3.at(4));
    ASSERT_DOUBLE_EQ(0.1, M.row_3.at(5));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(0));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(1));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(2));
    ASSERT_DOUBLE_EQ(4, M.row_4.at(3));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(4));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(5));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(0));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(1));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(2));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(3));
    ASSERT_DOUBLE_EQ(5, M.row_5.at(4));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(5));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(0));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(1));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(2));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(3));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(4));
    ASSERT_DOUBLE_EQ(6, M.row_6.at(5));
}

TEST_F(SimulatorYamlParserTest, can_parse_points)
{
    const std::vector<YamlPoint> points = input.points;
    ASSERT_EQ(2, points.size());
    ASSERT_EQ("P", points.at(0).name);
    ASSERT_EQ("body 1", points.at(0).frame);
    ASSERT_DOUBLE_EQ(10, points.at(0).x);
    ASSERT_DOUBLE_EQ(10, points.at(0).y);
    ASSERT_DOUBLE_EQ(20, points.at(0).z);

    ASSERT_EQ("O", points.at(1).name);
    ASSERT_EQ("NED", points.at(1).frame);
    ASSERT_DOUBLE_EQ(0.03, points.at(1).x);
    ASSERT_DOUBLE_EQ(-0.7, points.at(1).y);
    ASSERT_DOUBLE_EQ(300, points.at(1).z);
}

TEST_F(SimulatorYamlParserTest, can_parse_blocked_degrees_of_freedom)
{
    const std::vector<YamlBlockedDegreesOfFreedom> b = input.blocked_degrees_of_freedom;
    ASSERT_EQ(1,b.size());
    ASSERT_EQ("body 1", b.at(0).body);
    ASSERT_EQ(2, b.at(0).blocked.size());
    ASSERT_EQ("x", b.at(0).blocked.at(0));
    ASSERT_EQ("theta", b.at(0).blocked.at(1));
}
