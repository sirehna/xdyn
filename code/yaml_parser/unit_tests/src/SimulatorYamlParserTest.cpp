/*
 * SimulatorYamlParserTest.cpp
 *
 *  Created on: 15 avr. 2014
 *      Author: cady
 */

#include "SimulatorYamlParserTest.hpp"
#include "yaml_data.hpp"
#include "SimulatorYamlParser.hpp"

const YamlSimulatorInput SimulatorYamlParserTest::yaml = SimulatorYamlParser(test_data::full_example_with_propulsion()).parse();

SimulatorYamlParserTest::SimulatorYamlParserTest() : a(ssc::random_data_generator::DataGenerator(1212))
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
    const YamlRotation rot = yaml.rotations;
    ASSERT_EQ("angle", rot.order_by);
    ASSERT_EQ(3, rot.convention.size());
    ASSERT_EQ("z", rot.convention.at(0));
    ASSERT_EQ("y'", rot.convention.at(1));
    ASSERT_EQ("x''", rot.convention.at(2));
}

TEST_F(SimulatorYamlParserTest, can_parse_environment)
{
    ASSERT_EQ(1, yaml.environment.size());
    const std::vector<YamlModel> env = yaml.environment;
    ASSERT_EQ("no waves", env.at(0).model);
    ASSERT_EQ("constant sea elevation in NED frame:\n  unit: m\n  value: 12\nmodel: no waves", env.at(0).yaml);
}

TEST_F(SimulatorYamlParserTest, can_parse_environmental_constants)
{
    ASSERT_DOUBLE_EQ(9.81, yaml.environmental_constants.g);
    ASSERT_DOUBLE_EQ(1000, yaml.environmental_constants.rho);
    ASSERT_DOUBLE_EQ(1.18e-6, yaml.environmental_constants.nu);
}

TEST_F(SimulatorYamlParserTest, can_parse_external_forces)
{
    ASSERT_EQ(3, yaml.bodies.at(0).external_forces.size());
    ASSERT_EQ("gravity", yaml.bodies.at(0).external_forces.at(0).model);
    ASSERT_EQ("non-linear hydrostatic (fast)", yaml.bodies.at(0).external_forces.at(1).model);
    ASSERT_EQ("quadratic damping", yaml.bodies.at(0).external_forces.at(2).model);
}

TEST_F(SimulatorYamlParserTest, can_parse_controlled_forces)
{
    ASSERT_EQ(2, yaml.bodies.at(0).controlled_forces.size());
    ASSERT_EQ("wageningen B-series", yaml.bodies.at(0).controlled_forces.at(0).model);
    ASSERT_EQ("wageningen B-series", yaml.bodies.at(0).controlled_forces.at(1).model);
}

TEST_F(SimulatorYamlParserTest, can_parse_bodies)
{
    ASSERT_EQ(1, yaml.bodies.size());
    ASSERT_EQ("body 1", yaml.bodies.at(0).name);
    ASSERT_EQ("anthineas.stl", yaml.bodies.at(0).mesh);
}

TEST_F(SimulatorYamlParserTest, can_parse_position_of_body_frame_relative_to_mesh)
{
    ASSERT_DOUBLE_EQ(1,yaml.bodies.front().position_of_body_frame_relative_to_mesh.angle.phi);
    ASSERT_DOUBLE_EQ(3,yaml.bodies.front().position_of_body_frame_relative_to_mesh.angle.theta);
    ASSERT_DOUBLE_EQ(2,yaml.bodies.front().position_of_body_frame_relative_to_mesh.angle.psi);
    ASSERT_EQ("mesh",yaml.bodies.front().position_of_body_frame_relative_to_mesh.frame);
    ASSERT_DOUBLE_EQ(10,yaml.bodies.front().position_of_body_frame_relative_to_mesh.coordinates.x);
    ASSERT_DOUBLE_EQ(0.21,yaml.bodies.front().position_of_body_frame_relative_to_mesh.coordinates.y);
    ASSERT_DOUBLE_EQ(33e3,yaml.bodies.front().position_of_body_frame_relative_to_mesh.coordinates.z);
}

TEST_F(SimulatorYamlParserTest, can_parse_initial_position_of_body_frame_relative_to_NED)
{
    ASSERT_EQ("NED",yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.frame);
    ASSERT_DOUBLE_EQ(4,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.x);
    ASSERT_DOUBLE_EQ(8,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.y);
    ASSERT_DOUBLE_EQ(12,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.coordinates.z);
    ASSERT_DOUBLE_EQ(1.3,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.phi);
    ASSERT_DOUBLE_EQ(1.4,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.theta);
    ASSERT_DOUBLE_EQ(1.5,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.psi);
}

TEST_F(SimulatorYamlParserTest, can_parse_initial_velocity_of_body_frame_relative_to_NED)
{
    ASSERT_EQ("body 1",yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.frame);
    ASSERT_DOUBLE_EQ(-8,yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.u);
    ASSERT_DOUBLE_EQ(-9,yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.v);
    ASSERT_DOUBLE_EQ(14,yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.w);
    ASSERT_DOUBLE_EQ(56,yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.p);
    ASSERT_DOUBLE_EQ(7,yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.q);
    ASSERT_DOUBLE_EQ(6,yaml.bodies.front().initial_velocity_of_body_frame_relative_to_NED_projected_in_body.r);
}

TEST_F(SimulatorYamlParserTest, can_parse_centre_of_inertia)
{
    ASSERT_DOUBLE_EQ(4,yaml.bodies.front().dynamics.centre_of_inertia.x);
    ASSERT_DOUBLE_EQ(7,yaml.bodies.front().dynamics.centre_of_inertia.y);
    ASSERT_DOUBLE_EQ(-10,yaml.bodies.front().dynamics.centre_of_inertia.z);
    ASSERT_EQ("body 1",yaml.bodies.front().dynamics.centre_of_inertia.frame);
    ASSERT_EQ("centre of inertia",yaml.bodies.front().dynamics.centre_of_inertia.name);
}

TEST_F(SimulatorYamlParserTest, can_parse_mass)
{
    ASSERT_DOUBLE_EQ(1E6, yaml.bodies.front().dynamics.mass);
}

TEST_F(SimulatorYamlParserTest, can_parse_inertia_matrix)
{
    const YamlDynamics6x6Matrix M = yaml.bodies.front().dynamics.rigid_body_inertia;
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

TEST_F(SimulatorYamlParserTest, can_parse_added_mass_matrix)
{
    const YamlDynamics6x6Matrix M =  yaml.bodies.front().dynamics.added_mass;
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
    ASSERT_DOUBLE_EQ(-7, M.row_2.at(0));
    ASSERT_DOUBLE_EQ(2, M.row_2.at(1));
    ASSERT_DOUBLE_EQ(1, M.row_2.at(2));
    ASSERT_DOUBLE_EQ(2, M.row_2.at(3));
    ASSERT_DOUBLE_EQ(11, M.row_2.at(4));
    ASSERT_DOUBLE_EQ(13, M.row_2.at(5));
    ASSERT_DOUBLE_EQ(-20, M.row_3.at(0));
    ASSERT_DOUBLE_EQ(30, M.row_3.at(1));
    ASSERT_DOUBLE_EQ(3, M.row_3.at(2));
    ASSERT_DOUBLE_EQ(80, M.row_3.at(3));
    ASSERT_DOUBLE_EQ(0.9, M.row_3.at(4));
    ASSERT_DOUBLE_EQ(0.1, M.row_3.at(5));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(0));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(1));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(2));
    ASSERT_DOUBLE_EQ(4.1, M.row_4.at(3));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(4));
    ASSERT_DOUBLE_EQ(0, M.row_4.at(5));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(0));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(1));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(2));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(3));
    ASSERT_DOUBLE_EQ(5.2, M.row_5.at(4));
    ASSERT_DOUBLE_EQ(0, M.row_5.at(5));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(0));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(1));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(2));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(3));
    ASSERT_DOUBLE_EQ(0, M.row_6.at(4));
    ASSERT_DOUBLE_EQ(6.3, M.row_6.at(5));
}

TEST_F(SimulatorYamlParserTest, can_parse_added_mass_matrix_from_hdb_file)
{
    const YamlDynamics6x6Matrix M =  yaml.bodies.front().dynamics.added_mass;
    const YamlSimulatorInput input = SimulatorYamlParser(test_data::added_mass_from_file()).parse();
    ASSERT_TRUE(input.bodies.front().dynamics.added_mass.read_from_file);
    ASSERT_EQ("anthineas.hdb", input.bodies.front().dynamics.added_mass.hdb_filename);
}

TEST_F(SimulatorYamlParserTest, should_not_throw_even_if_no_mesh_is_defined)
{
    ASSERT_NO_THROW(SimulatorYamlParser(test_data::falling_ball_example()).parse());
}

TEST_F(SimulatorYamlParserTest, can_have_test_data_with_just_waves)
{
    const YamlSimulatorInput input = SimulatorYamlParser(test_data::waves()).parse();
    ASSERT_EQ(1,input.environment.size());
    ASSERT_EQ("waves",input.environment[0].model);
}

TEST_F(SimulatorYamlParserTest, can_parse_hydrodynamic_calculation_point)
{
    const YamlCoordinates P = yaml.bodies.front().dynamics.hydrodynamic_forces_calculation_point_in_body_frame;
    ASSERT_DOUBLE_EQ(0.696, P.x);
    ASSERT_DOUBLE_EQ(0, P.y);
    ASSERT_DOUBLE_EQ(1.418, P.z);
}
