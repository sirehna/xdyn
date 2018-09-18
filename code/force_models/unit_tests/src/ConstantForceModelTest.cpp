/*
 * GravityForceModelTest.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "BodyBuilder.hpp"
#include "EnvironmentAndFrames.hpp"
#include "ConstantForceModel.hpp"
#include "ConstantForceModelTest.hpp"
#include "generate_body_for_tests.hpp"
#include "yaml_data.hpp"
#include "TriMeshTestData.hpp"
#include <ssc/kinematics.hpp>

#define BODY "body 1"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define DEG (PI/180.)

ConstantForceModelTest::ConstantForceModelTest() : a(ssc::random_data_generator::DataGenerator(45454))
{
}

ConstantForceModelTest::~ConstantForceModelTest()
{
}

void ConstantForceModelTest::SetUp()
{
}

void ConstantForceModelTest::TearDown()
{
}

TEST_F(ConstantForceModelTest, can_parse_reference_frame)
{
    const auto input = ConstantForceModel::parse(test_data::constant_force());
    ASSERT_EQ("NED", input.frame);
}

TEST_F(ConstantForceModelTest, can_parse_x)
{
    const auto input = ConstantForceModel::parse(test_data::constant_force());
    ASSERT_DOUBLE_EQ(0.5, input.x);
}

TEST_F(ConstantForceModelTest, can_parse_y)
{
    const auto input = ConstantForceModel::parse(test_data::constant_force());
    ASSERT_DOUBLE_EQ(-0.2, input.y);
}

TEST_F(ConstantForceModelTest, can_parse_z)
{
    const auto input = ConstantForceModel::parse(test_data::constant_force());
    ASSERT_DOUBLE_EQ(-440, input.z);
}

TEST_F(ConstantForceModelTest, can_parse_X)
{
    const auto input = ConstantForceModel::parse(test_data::constant_force());
    ASSERT_DOUBLE_EQ(10e3, input.X);
}

TEST_F(ConstantForceModelTest, can_parse_Y)
{
    const auto input = ConstantForceModel::parse(test_data::constant_force());
    ASSERT_DOUBLE_EQ(20e3, input.Y);
}

TEST_F(ConstantForceModelTest, can_parse_Z)
{
    const auto input = ConstantForceModel::parse(test_data::constant_force());
    ASSERT_DOUBLE_EQ(30e3, input.Z);
}

TEST_F(ConstantForceModelTest, can_parse_K)
{
    const auto input = ConstantForceModel::parse(test_data::constant_force());
    ASSERT_DOUBLE_EQ(100e3, input.K);
}

TEST_F(ConstantForceModelTest, can_parse_M)
{
    const auto input = ConstantForceModel::parse(test_data::constant_force());
    ASSERT_DOUBLE_EQ(200e3, input.M);
}

TEST_F(ConstantForceModelTest, can_parse_N)
{
    const auto input = ConstantForceModel::parse(test_data::constant_force());
    ASSERT_DOUBLE_EQ(300e3, input.N);
}

BodyPtr ConstantForceModelTest::get_body(const std::string& name) const

{
    YamlRotation rot;
    rot.convention.push_back("z");
    rot.convention.push_back("y'");
    rot.convention.push_back("x''");
    rot.order_by = "angle";
    return BodyBuilder(rot).build(name, two_triangles(), 0, 0, rot, true);
}


BodyStates ConstantForceModelTest::get_states(const double phi, const double theta, const double psi, EnvironmentAndFrames& env) const
{
    auto body = get_body("Anthineas");
    auto states = body->get_states();
    YamlRotation rotations;
    rotations.order_by = "angle";
    rotations.convention.push_back("z");
    rotations.convention.push_back("y'");
    rotations.convention.push_back("x''");
    const auto angle = states.convert(
            ssc::kinematics::EulerAngles(phi, theta, psi), rotations);
    states.x.record(0, 0.1);
    states.y.record(0, 2.04);
    states.z.record(0, 6.28);
    states.u.record(0, 0.45);
    states.v.record(0, 0.01);
    states.w.record(0, 5.869);
    states.p.record(0, 0.23);
    states.q.record(0, 0);
    states.r.record(0, 0.38);
    states.qr.record(0, std::get<0>(angle));
    states.qi.record(0, std::get<1>(angle));
    states.qj.record(0, std::get<2>(angle));
    states.qk.record(0, std::get<3>(angle));
    body->update_kinematics(states.get_current_state_values(0), env.k);
    return states;
}

EnvironmentAndFrames ConstantForceModelTest::get_env() const
{
    EnvironmentAndFrames env;
    env.rot.convention = {"z", "y'", "x''"};
    env.rot.order_by = "angle";
    env.rho = 1024;
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    env.k->add(ssc::kinematics::Transform(ssc::kinematics::Point("NED"), BODY));
    return env;
}

ConstantForceModel ConstantForceModelTest::get_constant_force(EnvironmentAndFrames& env) const
{
    const auto input = ConstantForceModel::parse(test_data::constant_force());
    return ConstantForceModel(input, "Anthineas", env);
}

TEST_F(ConstantForceModelTest, ship_at_45_deg)
{
    EnvironmentAndFrames env = get_env();
    const double phi = 0;
    const double theta = 0;
    const double psi = 45 * DEG;
    auto states = get_states(phi, theta, psi, env);
    const auto W = get_constant_force(env)(states, a.random<double>());
    ASSERT_DOUBLE_EQ(10e3*std::sqrt(2)/2 + 20e3*sqrt(2)/2, (double)W.X());
    ASSERT_DOUBLE_EQ(-10e3*sqrt(2)/2+20e3*std::sqrt(2)/2, (double)W.Y());
    ASSERT_DOUBLE_EQ(30e3, (double)W.Z());
}

TEST_F(ConstantForceModelTest, ship_at_30_deg)
{
    EnvironmentAndFrames env = get_env();
    const double phi = 0;
    const double theta = 0;
    const double psi = 30 * DEG;
    auto states = get_states(phi, theta, psi, env);
    const auto W = get_constant_force(env)(states, a.random<double>());
    ASSERT_DOUBLE_EQ(10e3*std::sqrt(3)/2 + 20e3*sqrt(1)/2, (double)W.X());
    ASSERT_DOUBLE_EQ(-10e3*sqrt(1)/2+20e3*std::sqrt(3)/2, (double)W.Y());
    ASSERT_DOUBLE_EQ(30e3, (double)W.Z());
}
