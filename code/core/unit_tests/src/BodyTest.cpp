/*
 * BodyTest.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: cady
 */

#include "BodyWithSurfaceForces.hpp"
#include "BodyTest.hpp"
#include "BodyBuilderTest.hpp"
#include "EnvironmentAndFrames.hpp"
#include "SimulatorYamlParser.hpp"
#include "yaml_data.hpp"
#include "State.hpp"

#define EPS (1E-10)

BodyTest::BodyTest() : a(ssc::random_data_generator::DataGenerator(21212))
{
}

BodyTest::~BodyTest()
{
}

void BodyTest::SetUp()
{
}

void BodyTest::TearDown()
{
}

const BodyPtr BodyTest::body = BodyBuilderTest::build_body(1);

TEST_F(BodyTest, frame_of_origin_point_is_correct)
{
    const ssc::kinematics::Point O = body->get_origin(a.random_vector_of<double>().of_size(1000));
    ASSERT_EQ("NED", O.get_frame());
}

TEST_F(BodyTest, can_build_rotation_matrix_from_states)
{
    const StateType x = {1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,3,5,7,13};
    const ssc::kinematics::RotationMatrix R = body->get_rot_from_ned_to(x);
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

TEST_F(BodyTest, can_return_position_of_body_mesh_from_Body_object)
{
    const ssc::kinematics::Point P = body->get_position_of_body_relative_to_mesh();
    ASSERT_EQ("mesh(body 1)", P.get_frame());
    ASSERT_DOUBLE_EQ(10,P.x());
    ASSERT_DOUBLE_EQ(0.21,P.y());
    ASSERT_DOUBLE_EQ(33E3,P.z());
}

TEST_F(BodyTest, can_compute_transform_from_mesh_to_body_from_Body_object)
{
    ssc::kinematics::Transform T = body->get_transform_from_mesh_to_body();
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

TEST_F(BodyTest, can_get_transform_from_NED_to_body_from_states)
{
    BodyStates states;
    states.name = "body 1";
    BodyWithSurfaceForces b(states,1,BlockedDOF(""));
    const StateType x = {1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,3,5,7,13};
    const ssc::kinematics::Transform T = b.get_transform_from_ned_to_body(x);
    ASSERT_EQ("NED", T.get_from_frame());
    ASSERT_EQ(body->get_name(), T.get_to_frame());
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

TEST_F(BodyTest, can_update_Kinematics_object_from_states)
{
    ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics());
    const StateType x = {1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,3,5,7,13};
    BodyPtr b(body);
    b->update_kinematics(x, k);
    const auto T = k->get("NED", b->get_name());
    ASSERT_EQ("NED", T.get_from_frame());
    ASSERT_EQ(body->get_name(), T.get_to_frame());
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

TEST_F(BodyTest, can_compute_transform_from_ned_to_local_ned)
{
    ssc::kinematics::KinematicsPtr k(new ssc::kinematics::Kinematics());
    const StateType x = {1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,4,5,6,7,8,9,3,5,7,13};
    BodyPtr b(body);
    b->update_kinematics(x, k);
    const std::string local_ned = std::string("NED(") + b->get_name() + ")";
    const auto T = k->get("NED", local_ned);
    ASSERT_EQ("NED", T.get_from_frame());
    ASSERT_EQ(local_ned, T.get_to_frame());
    ASSERT_EQ("NED", T.get_point().get_frame());
    ASSERT_DOUBLE_EQ(1, T.get_point().x());
    ASSERT_DOUBLE_EQ(2, T.get_point().y());
    ASSERT_DOUBLE_EQ(3, T.get_point().z());

    ASSERT_DOUBLE_EQ(1, (double)T.get_rot()(0,0));
    ASSERT_DOUBLE_EQ(0, (double)T.get_rot()(1,0));
    ASSERT_DOUBLE_EQ(0, (double)T.get_rot()(2,0));

    ASSERT_DOUBLE_EQ(0, (double)T.get_rot()(0,1));
    ASSERT_DOUBLE_EQ(1, (double)T.get_rot()(1,1));
    ASSERT_DOUBLE_EQ(0, (double)T.get_rot()(2,1));

    ASSERT_DOUBLE_EQ(0, (double)T.get_rot()(0,2));
    ASSERT_DOUBLE_EQ(0, (double)T.get_rot()(1,2));
    ASSERT_DOUBLE_EQ(1, (double)T.get_rot()(2,2));
}

TEST_F(BodyTest, forced_states_are_taken_into_account)
{
    const StateType x = {1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,44,5,6,7,8,9,3,5,7,13};
    const double t = 2.1;
    body->update_body_states(x, t);
    const auto states = body->get_states();
    ASSERT_DOUBLE_EQ(4.5, states.u());
}

TEST_F(BodyTest, forced_state_derivatives_are_taken_into_account)
{
    const StateType x = {1,2,3,4,5,6,7,8,9,10,11,12,13,1,2,3,44,5,6,7,8,9,3,5,7,13};
    StateType dx_dt = {1,2,3,4,5,6,7,8,9,10,11,12,13,10,11,12,13,14,15,16,17,18,19,20,21,23};
    Eigen::Matrix<double,6,6> total_inertia;
    total_inertia << 1, 2, 3, 4, 5, 6,
                     7, 8, 9, 0, 1, 2,
                     3, 4, 5, 6, 7, 8,
                     9, 0, 1, 2, 3, 4,
                     5, 6, 7, 8, 9, 0,
                     1, 2, 3, 4, 5, 6;
    Eigen::Vector3d F,T;
    F(0) = 50;
    F(1) = 51;
    F(2) = 52;
    T(0) = 60;
    T(1) = 61;
    T(2) = 62;
    ssc::kinematics::Wrench sum_of_other_forces(ssc::kinematics::Point("d",1,2,3),F,T);
    EnvironmentAndFrames env;
    ssc::kinematics::Point P("NED", 0,0,0);
    ssc::kinematics::Transform tr(P, body->get_name());
    env.k->add(tr);
    const double t = 2.1;
    body->calculate_state_derivatives(sum_of_other_forces, x, dx_dt, t, env);

    ASSERT_DOUBLE_EQ(1./4.2, dx_dt[UIDX(1)]);
}

TEST_F(BodyTest, can_overwrite_history_with_single_value)
{
    AbstractStates<History> new_states;
    new_states.x.record(a.random<double>(),a.random<double>());
    new_states.y.record(a.random<double>(),a.random<double>());
    new_states.z.record(a.random<double>(),a.random<double>());
    new_states.u.record(a.random<double>(),a.random<double>());
    new_states.v.record(a.random<double>(),a.random<double>());
    new_states.w.record(a.random<double>(),a.random<double>());
    new_states.p.record(a.random<double>(),a.random<double>());
    new_states.q.record(a.random<double>(),a.random<double>());
    new_states.r.record(a.random<double>(),a.random<double>());
    new_states.qr.record(a.random<double>(),a.random<double>());
    new_states.qi.record(a.random<double>(),a.random<double>());
    new_states.qj.record(a.random<double>(),a.random<double>());
    new_states.qk.record(a.random<double>(),a.random<double>());
    body->set_states_history(new_states);

    ASSERT_DOUBLE_EQ(body->get_states().x(),new_states.x());
    ASSERT_EQ(body->get_states().x.size(),1);
    ASSERT_DOUBLE_EQ(body->get_states().y(),new_states.y());
    ASSERT_EQ(body->get_states().y.size(),1);
    ASSERT_DOUBLE_EQ(body->get_states().z(),new_states.z());
    ASSERT_EQ(body->get_states().z.size(),1);
    ASSERT_DOUBLE_EQ(body->get_states().u(),new_states.u());
    ASSERT_EQ(body->get_states().u.size(),1);
    ASSERT_DOUBLE_EQ(body->get_states().v(),new_states.v());
    ASSERT_EQ(body->get_states().v.size(),1);
    ASSERT_DOUBLE_EQ(body->get_states().w(),new_states.w());
    ASSERT_EQ(body->get_states().w.size(),1);
    ASSERT_DOUBLE_EQ(body->get_states().p(),new_states.p());
    ASSERT_EQ(body->get_states().p.size(),1);
    ASSERT_DOUBLE_EQ(body->get_states().q(),new_states.q());
    ASSERT_EQ(body->get_states().q.size(),1);
    ASSERT_DOUBLE_EQ(body->get_states().r(),new_states.r());
    ASSERT_EQ(body->get_states().r.size(),1);
    ASSERT_DOUBLE_EQ(body->get_states().qr(),new_states.qr());
    ASSERT_EQ(body->get_states().qr.size(),1);
    ASSERT_DOUBLE_EQ(body->get_states().qi(),new_states.qi());
    ASSERT_EQ(body->get_states().qi.size(),1);
    ASSERT_DOUBLE_EQ(body->get_states().qj(),new_states.qj());
    ASSERT_EQ(body->get_states().qj.size(),1);
    ASSERT_DOUBLE_EQ(body->get_states().qk(),new_states.qk());
    ASSERT_EQ(body->get_states().qk.size(),1);
}

TEST_F(BodyTest, can_overwrite_history_with_single_value_several_times)
{

    for(size_t i=0; i<5; i++)
    {
        double new_x=a.random<double>();
        double new_y=a.random<double>();
        double new_z=a.random<double>();
        double new_u=a.random<double>();
        double new_v=a.random<double>();
        double new_w=a.random<double>();
        double new_p=a.random<double>();
        double new_q=a.random<double>();
        double new_r=a.random<double>();
        double new_qr=a.random<double>();
        double new_qi=a.random<double>();
        double new_qj=a.random<double>();
        double new_qk=a.random<double>();
        double t=0.0;

        const State new_state(AbstractStates<double>(new_x, new_y, new_z, new_u, new_v, new_w, new_p, new_q, new_r, new_qr, new_qi, new_qj, new_qk),t);

        body->set_states_history(new_state);

        ASSERT_DOUBLE_EQ(body->get_states().x(),new_x);
        ASSERT_EQ(body->get_states().x.size(),1);
        ASSERT_DOUBLE_EQ(body->get_states().y(),new_y);
        ASSERT_EQ(body->get_states().y.size(),1);
        ASSERT_DOUBLE_EQ(body->get_states().z(),new_z);
        ASSERT_EQ(body->get_states().z.size(),1);
        ASSERT_DOUBLE_EQ(body->get_states().u(),new_u);
        ASSERT_EQ(body->get_states().u.size(),1);
        ASSERT_DOUBLE_EQ(body->get_states().v(),new_v);
        ASSERT_EQ(body->get_states().v.size(),1);
        ASSERT_DOUBLE_EQ(body->get_states().w(),new_w);
        ASSERT_EQ(body->get_states().w.size(),1);
        ASSERT_DOUBLE_EQ(body->get_states().p(),new_p);
        ASSERT_EQ(body->get_states().p.size(),1);
        ASSERT_DOUBLE_EQ(body->get_states().q(),new_q);
        ASSERT_EQ(body->get_states().q.size(),1);
        ASSERT_DOUBLE_EQ(body->get_states().r(),new_r);
        ASSERT_EQ(body->get_states().r.size(),1);
        ASSERT_DOUBLE_EQ(body->get_states().qr(),new_qr);
        ASSERT_EQ(body->get_states().qr.size(),1);
        ASSERT_DOUBLE_EQ(body->get_states().qi(),new_qi);
        ASSERT_EQ(body->get_states().qi.size(),1);
        ASSERT_DOUBLE_EQ(body->get_states().qj(),new_qj);
        ASSERT_EQ(body->get_states().qj.size(),1);
        ASSERT_DOUBLE_EQ(body->get_states().qk(),new_qk);
        ASSERT_EQ(body->get_states().qk.size(),1);
    }
}
