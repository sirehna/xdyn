/*
 * update_kinematicsTests.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#include "update_kinematics.hpp"
#include "update_kinematicsTests.hpp"
#include "BodyBuilderTest.hpp"

update_kinematicsTests::update_kinematicsTests() : a(DataGenerator(121))
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

TEST_F(update_kinematicsTests, frame_of_origin_point_is_correct)
{
    const Point O = get_origin(a.random_vector_of<double>().of_size(1000), 46);
    ASSERT_EQ("NED", O.get_frame());
}

TEST_F(update_kinematicsTests, can_build_rotation_matrix_from_states)
{
    const StateType x = {1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,3,5,7,13};
    const RotationMatrix R = get_rot_from_ned_to(x, 1);
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
    const Body body = BodyBuilderTest::build_body();
    const Point P = get_position_of_body_relative_to_mesh(body);
    ASSERT_EQ("mesh(body 1)", P.get_frame());
    ASSERT_DOUBLE_EQ(10,P.x);
    ASSERT_DOUBLE_EQ(0.21,P.y);
    ASSERT_DOUBLE_EQ(33E3,P.z);
}

TEST_F(update_kinematicsTests, can_compute_transform_from_mesh_to_body_from_Body_object)
{
    const Body body = BodyBuilderTest::build_body();
    kinematics::Transform T = get_transform_from_mesh_to(body);
    ASSERT_EQ("mesh(body 1)", T.get_from_frame());
    ASSERT_EQ("body 1", T.get_to_frame());
    ASSERT_EQ("mesh(body 1)", T.get_point().get_frame());
    ASSERT_DOUBLE_EQ(10, T.get_point().x);
    ASSERT_DOUBLE_EQ(0.21, T.get_point().y);
    ASSERT_DOUBLE_EQ(33E3, T.get_point().z);

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
