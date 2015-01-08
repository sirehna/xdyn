/*
 * update_kinematicsTests.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#include "update_kinematics.hpp"
#include "update_kinematicsTests.hpp"
#include "BodyBuilderTest.hpp"
#include <ssc/kinematics.hpp>
#include "SimulatorYamlParser.hpp"
#include "yaml_data.hpp"

#define EPS (1E-10)

update_kinematicsTests::update_kinematicsTests() : a(ssc::random_data_generator::DataGenerator(121))
{
}

update_kinematicsTests::~update_kinematicsTests()
{
}

void update_kinematicsTests::SetUp()
{
}

void update_kinematicsTests::TearDown()
{
}

const Body update_kinematicsTests::body = BodyBuilderTest::build_body();

TEST_F(update_kinematicsTests, frame_of_origin_point_is_correct)
{
    const ssc::kinematics::Point O = get_origin(a.random_vector_of<double>().of_size(1000), 46);
    ASSERT_EQ("NED", O.get_frame());
}

TEST_F(update_kinematicsTests, can_build_rotation_matrix_from_states)
{
    const StateType x = {1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,3,5,7,13};
    const ssc::kinematics::RotationMatrix R = get_rot_from_ned_to(x, 1);
    ASSERT_DOUBLE_EQ(1-2*7*7-2*13*13, (double)R(0,0));
    ASSERT_DOUBLE_EQ(2*5*7+2*13*3,    (double)R(1,0));
    ASSERT_DOUBLE_EQ(2*5*13-2*7*3,    (double)R(2,0));

    ASSERT_DOUBLE_EQ(2*5*7-2*13*3,    (double)R(0,1));
    ASSERT_DOUBLE_EQ(1-2*5*5-2*13*13, (double)R(1,1));
    ASSERT_DOUBLE_EQ(2*7*13+2*5*3,    (double)R(2,1));

    ASSERT_DOUBLE_EQ(2*5*13+2*7*3,    (double)R(0,2));
    ASSERT_DOUBLE_EQ(2*7*13-2*5*3,    (double)R(1,2));
    ASSERT_DOUBLE_EQ(1-2*5*5-2*7*7,   (double)R(2,2));
}

TEST_F(update_kinematicsTests, can_return_position_of_body_mesh_from_Body_object)
{
    const ssc::kinematics::Point P = get_position_of_body_relative_to_mesh(body);
    ASSERT_EQ("mesh(body 1)", P.get_frame());
    ASSERT_DOUBLE_EQ(10,P.x());
    ASSERT_DOUBLE_EQ(0.21,P.y());
    ASSERT_DOUBLE_EQ(33E3,P.z());
}

TEST_F(update_kinematicsTests, can_compute_transform_from_mesh_to_body_from_Body_object)
{
    ssc::kinematics::Transform T = get_transform_from_mesh_to(body);
    ASSERT_EQ("mesh(body 1)", T.get_from_frame());
    ASSERT_EQ("body 1", T.get_to_frame());
    ASSERT_EQ("mesh(body 1)", T.get_point().get_frame());
    ASSERT_DOUBLE_EQ(10, T.get_point().x());
    ASSERT_DOUBLE_EQ(0.21, T.get_point().y());
    ASSERT_DOUBLE_EQ(33E3, T.get_point().z());

    ASSERT_DOUBLE_EQ(cos(2)*cos(3),(double)T.get_rot()(0,0));
    ASSERT_DOUBLE_EQ(sin(2)*cos(3),(double)T.get_rot()(1,0));
    ASSERT_DOUBLE_EQ(-sin(3),      (double)T.get_rot()(2,0));

    ASSERT_DOUBLE_EQ(-sin(2)*cos(1)+cos(2)*sin(3)*sin(1),(double)T.get_rot()(0,1));
    ASSERT_DOUBLE_EQ(cos(2)*cos(1)+sin(2)*sin(3)*sin(1), (double)T.get_rot()(1,1));
    ASSERT_DOUBLE_EQ(cos(3)*sin(1),                      (double)T.get_rot()(2,1));

    ASSERT_DOUBLE_EQ(sin(2)*sin(1)+cos(2)*cos(1)*sin(3), (double)T.get_rot()(0,2));
    ASSERT_DOUBLE_EQ(-cos(2)*sin(1)+sin(3)*sin(2)*cos(1),(double)T.get_rot()(1,2));
    ASSERT_DOUBLE_EQ(cos(3)*cos(1),                      (double)T.get_rot()(2,2));
}

TEST_F(update_kinematicsTests, can_get_transform_from_NED_to_body_from_states)
{
    Body body(1);
    body.states.name = a.random<std::string>();
    const StateType x = {1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,3,5,7,13};
    const ssc::kinematics::Transform T = get_transform_from_ned_to(x, body, 1);
    ASSERT_EQ("NED", T.get_from_frame());
    ASSERT_EQ(body.states.name, T.get_to_frame());
    ASSERT_EQ("NED", T.get_point().get_frame());
    ASSERT_DOUBLE_EQ(1, T.get_point().x());
    ASSERT_DOUBLE_EQ(2, T.get_point().y());
    ASSERT_DOUBLE_EQ(3, T.get_point().z());

    ASSERT_DOUBLE_EQ(1-2*7*7-2*13*13, (double)T.get_rot()(0,0));
    ASSERT_DOUBLE_EQ(2*5*7+2*13*3,    (double)T.get_rot()(1,0));
    ASSERT_DOUBLE_EQ(2*5*13-2*7*3,    (double)T.get_rot()(2,0));

    ASSERT_DOUBLE_EQ(2*5*7-2*13*3,    (double)T.get_rot()(0,1));
    ASSERT_DOUBLE_EQ(1-2*5*5-2*13*13, (double)T.get_rot()(1,1));
    ASSERT_DOUBLE_EQ(2*7*13+2*5*3,    (double)T.get_rot()(2,1));

    ASSERT_DOUBLE_EQ(2*5*13+2*7*3,    (double)T.get_rot()(0,2));
    ASSERT_DOUBLE_EQ(2*7*13-2*5*3,    (double)T.get_rot()(1,2));
    ASSERT_DOUBLE_EQ(1-2*5*5-2*7*7,   (double)T.get_rot()(2,2));
}

TEST_F(update_kinematicsTests, can_update_Kinematics_object_from_states)
{
    KinematicsPtr k(new ssc::kinematics::Kinematics());
    const StateType x = {1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,3,5,7,13};
    update_kinematics(x, body, 1, k);
    const auto T = k->get("NED", body.states.name);
    ASSERT_EQ("NED", T.get_from_frame());
    ASSERT_EQ(body.states.name, T.get_to_frame());
    ASSERT_EQ("NED", T.get_point().get_frame());
    ASSERT_DOUBLE_EQ(1, T.get_point().x());
    ASSERT_DOUBLE_EQ(2, T.get_point().y());
    ASSERT_DOUBLE_EQ(3, T.get_point().z());

    ASSERT_DOUBLE_EQ(1-2*7*7-2*13*13, (double)T.get_rot()(0,0));
    ASSERT_DOUBLE_EQ(2*5*7+2*13*3,    (double)T.get_rot()(1,0));
    ASSERT_DOUBLE_EQ(2*5*13-2*7*3,    (double)T.get_rot()(2,0));

    ASSERT_DOUBLE_EQ(2*5*7-2*13*3,    (double)T.get_rot()(0,1));
    ASSERT_DOUBLE_EQ(1-2*5*5-2*13*13, (double)T.get_rot()(1,1));
    ASSERT_DOUBLE_EQ(2*7*13+2*5*3,    (double)T.get_rot()(2,1));

    ASSERT_DOUBLE_EQ(2*5*13+2*7*3,    (double)T.get_rot()(0,2));
    ASSERT_DOUBLE_EQ(2*7*13-2*5*3,    (double)T.get_rot()(1,2));
    ASSERT_DOUBLE_EQ(1-2*5*5-2*7*7,   (double)T.get_rot()(2,2));
}

TEST_F(update_kinematicsTests, can_get_initial_states)
{
    const auto yaml = SimulatorYamlParser(test_data::full_example()).parse();
    const StateType x = get_initial_states(yaml.rotations, yaml.bodies);
    ASSERT_EQ(13, x.size());
    ASSERT_DOUBLE_EQ(4, x.at(0));
    ASSERT_DOUBLE_EQ(8, x.at(1));
    ASSERT_DOUBLE_EQ(12, x.at(2));
    ASSERT_DOUBLE_EQ(-8, x.at(3));
    ASSERT_DOUBLE_EQ(-9, x.at(4));
    ASSERT_DOUBLE_EQ(14, x.at(5));
    ASSERT_DOUBLE_EQ(56, x.at(6));
    ASSERT_DOUBLE_EQ(7, x.at(7));
    ASSERT_DOUBLE_EQ(6, x.at(8));

    ASSERT_NEAR(cos(1.3/2)*cos(1.4/2)*cos(1.5/2)+sin(1.3/2)*sin(1.4/2)*sin(1.5/2), x.at(9), EPS);
    ASSERT_NEAR(sin(1.3/2)*cos(1.4/2)*cos(1.5/2)-cos(1.3/2)*sin(1.4/2)*sin(1.5/2), x.at(10), EPS);
    ASSERT_NEAR(cos(1.3/2)*sin(1.4/2)*cos(1.5/2)+sin(1.3/2)*cos(1.4/2)*sin(1.5/2), x.at(11), EPS);
    ASSERT_NEAR(cos(1.3/2)*cos(1.4/2)*sin(1.5/2)-sin(1.3/2)*sin(1.4/2)*cos(1.5/2), x.at(12), EPS);
}
