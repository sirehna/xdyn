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
                                                     yaml(SimulatorYamlParser(test_data::full_example()).parse())
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
    ASSERT_EQ(2, yaml.environment.size());
    const std::vector<YamlModel> env = yaml.environment;
    ASSERT_EQ("no waves", env.at(0).model);
    ASSERT_EQ("no wind", env.at(1).model);
    ASSERT_EQ("constant wave height in NED frame:\n  unit: m\n  value: 12\nmodel: no waves", env.at(0).yaml);
}

TEST_F(SimulatorYamlParserTest, can_parse_external_forces)
{
    ASSERT_EQ(2, yaml.bodies.at(0).external_forces.size());
    const YamlModel external_forces = yaml.bodies.at(0).external_forces.at(0);
    ASSERT_EQ("gravity", external_forces.model);
    ASSERT_EQ("g:\n  unit: m/s^2\n  value: 9.81\nmodel: gravity", external_forces.yaml);
}

TEST_F(SimulatorYamlParserTest, can_parse_bodies)
{
    ASSERT_EQ(1, yaml.bodies.size());
    ASSERT_EQ("body 1", yaml.bodies.at(0).name);
    ASSERT_EQ("anthineas.stl", yaml.bodies.at(0).mesh);
    ASSERT_EQ(2, yaml.bodies.at(0).external_forces.size());
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
    ASSERT_DOUBLE_EQ(13,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.phi);
    ASSERT_DOUBLE_EQ(14,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.theta);
    ASSERT_DOUBLE_EQ(15,yaml.bodies.front().initial_position_of_body_frame_relative_to_NED_projected_in_NED.angle.psi);
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
    const YamlInertiaMatrix M =  yaml.bodies.front().dynamics.rigid_body_inertia;
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
    const YamlInertiaMatrix M =  yaml.bodies.front().dynamics.added_mass;
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

TEST_F(SimulatorYamlParserTest, can_parse_points)
{
    const std::vector<YamlPoint> points = yaml.points;
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
    const std::vector<YamlBlockedDegreesOfFreedom> b = yaml.blocked_degrees_of_freedom;
    ASSERT_EQ(1,b.size());
    ASSERT_EQ("body 1", b.at(0).body);
    ASSERT_EQ(2, b.at(0).blocked.size());
    ASSERT_EQ("x", b.at(0).blocked.at(0));
    ASSERT_EQ("theta", b.at(0).blocked.at(1));
}

TEST_F(SimulatorYamlParserTest, can_parse_output_positions)
{
    const std::vector<YamlPositionOutput> p = yaml.position_output;
    ASSERT_EQ(1, p.size());
    ASSERT_EQ(2, p.at(0).axes.size());
    ASSERT_EQ("x",p.at(0).axes.at(0));
    ASSERT_EQ("z",p.at(0).axes.at(1));
    ASSERT_EQ("O",p.at(0).point);
    ASSERT_EQ("body 1",p.at(0).projected_in_frame);
    ASSERT_EQ("NED",p.at(0).relative_to_frame);
}

TEST_F(SimulatorYamlParserTest, can_parse_output_angles)
{
    const std::vector<YamlAnglesOutput> p = yaml.angles_output;
    ASSERT_EQ(1, p.size());
    ASSERT_EQ(3, p.at(0).axes.size());
    ASSERT_EQ("x",p.at(0).axes.at(0));
    ASSERT_EQ("y",p.at(0).axes.at(1));
    ASSERT_EQ("z",p.at(0).axes.at(2));
    ASSERT_EQ("body 1",p.at(0).projected_in_frame);
    ASSERT_EQ("NED",p.at(0).relative_to_frame);
    ASSERT_EQ("body 1",p.at(0).frame);
}

TEST_F(SimulatorYamlParserTest, can_parse_output_linear_velocities)
{
    const std::vector<YamlPositionOutput> p = yaml.linear_velocities_output;
    ASSERT_EQ(2, p.size());

    ASSERT_EQ(2, p.at(0).axes.size());
    ASSERT_EQ("x",p.at(0).axes.at(0));
    ASSERT_EQ("y",p.at(0).axes.at(1));
    ASSERT_EQ("P",p.at(0).point);
    ASSERT_EQ("body 1",p.at(0).projected_in_frame);
    ASSERT_EQ("NED",p.at(0).relative_to_frame);

    ASSERT_EQ(1, p.at(1).axes.size());
    ASSERT_EQ("z",p.at(1).axes.at(0));
    ASSERT_EQ("P",p.at(1).point);
    ASSERT_EQ("NED",p.at(1).projected_in_frame);
    ASSERT_EQ("NED",p.at(1).relative_to_frame);
}

TEST_F(SimulatorYamlParserTest, can_parse_output_angular_velocities)
{
    const std::vector<YamlAnglesOutput> p = yaml.angular_velocities_output;
    ASSERT_EQ(1, p.size());
    ASSERT_EQ(2, p.at(0).axes.size());
    ASSERT_EQ("x",p.at(0).axes.at(0));
    ASSERT_EQ("y",p.at(0).axes.at(1));
    ASSERT_EQ("body 1",p.at(0).projected_in_frame);
    ASSERT_EQ("NED",p.at(0).relative_to_frame);
    ASSERT_EQ("body 1",p.at(0).frame);
}

TEST_F(SimulatorYamlParserTest, can_parse_output_linear_accelerations)
{
    const std::vector<YamlPositionOutput> p = yaml.linear_accelerations_output;
    ASSERT_EQ(2, p.size());

    ASSERT_EQ(2, p.at(0).axes.size());
    ASSERT_EQ("x",p.at(0).axes.at(0));
    ASSERT_EQ("y",p.at(0).axes.at(1));
    ASSERT_EQ("P",p.at(0).point);
    ASSERT_EQ("body 1",p.at(0).projected_in_frame);
    ASSERT_EQ("NED",p.at(0).relative_to_frame);

    ASSERT_EQ(1, p.at(1).axes.size());
    ASSERT_EQ("z",p.at(1).axes.at(0));
    ASSERT_EQ("P",p.at(1).point);
    ASSERT_EQ("NED",p.at(1).projected_in_frame);
    ASSERT_EQ("NED",p.at(1).relative_to_frame);
}

TEST_F(SimulatorYamlParserTest, can_parse_output_angular_accelerations)
{
    const std::vector<YamlAnglesOutput> p = yaml.angular_accelerations_output;
    ASSERT_EQ(1, p.size());
    ASSERT_EQ(2, p.at(0).axes.size());
    ASSERT_EQ("x",p.at(0).axes.at(0));
    ASSERT_EQ("y",p.at(0).axes.at(1));
    ASSERT_EQ("body 1",p.at(0).projected_in_frame);
    ASSERT_EQ("NED",p.at(0).relative_to_frame);
    ASSERT_EQ("body 1",p.at(0).frame);
}

TEST_F(SimulatorYamlParserTest, can_parse_output_forces_and_torques)
{
    const std::vector<YamlForcesAndTorquesOutput> f = yaml.forces_and_torques_output;
    ASSERT_EQ(1, f.size());
    ASSERT_EQ(4, f.at(0).axes.size());
    ASSERT_EQ("X", f.at(0).axes.at(0));
    ASSERT_EQ("Z", f.at(0).axes.at(1));
    ASSERT_EQ("K", f.at(0).axes.at(2));
    ASSERT_EQ("N", f.at(0).axes.at(3));
    ASSERT_EQ("body 1", f.at(0).point_frame);
    ASSERT_EQ("centre of inertia", f.at(0).point_name);
    ASSERT_EQ("NED", f.at(0).projected_in_frame);
    ASSERT_EQ("non-linear hydrostatic", f.at(0).type);
}
