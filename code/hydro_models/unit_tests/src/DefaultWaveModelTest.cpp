/*
 * HydrostaticForceTest.cpp
 *
 *  Created on: 31 mars 2014
 *      Author: cec
 */

#include "DefaultWaveModelTest.hpp"
#include "DefaultWaveModel.hpp"
#include "Kinematics.hpp"
#include "Transform.hpp"

DefaultWaveModelTest::DefaultWaveModelTest() : a(DataGenerator(5466123))
{
}

DefaultWaveModelTest::~DefaultWaveModelTest()
{
}

void DefaultWaveModelTest::SetUp()
{
}

void DefaultWaveModelTest::TearDown()
{
}

TEST_F(DefaultWaveModelTest, example)
{
//! [DefaultWaveModelTest example]
    TR1(shared_ptr)<Kinematics> k(new Kinematics());
    const DefaultWaveModel w(10);
    const Point P("NED", 0, 0, -20);
//! [DefaultWaveModelTest example]
//! [DefaultWaveModelTest expected output]
    ASSERT_DOUBLE_EQ(-30, w.get_relative_wave_height(P, k, a.random<double>()));
//! [DefaultWaveModelTest expected output]
}

TEST_F(DefaultWaveModelTest, more_interesting_example)
{
    TR1(shared_ptr)<Kinematics> k(new Kinematics());
    const Point P("body", 0, 0, 0);
    const Point Q("NED", 7, -100, 1795);
    const DefaultWaveModel w(7);

    const kinematics::Transform bodyTned(Point("NED", 4, 5, 9), "body");
    k->add(bodyTned);
    ASSERT_DOUBLE_EQ(2, w.get_relative_wave_height(P,k, a.random<double>()));
    ASSERT_DOUBLE_EQ(1788, w.get_relative_wave_height(Q,k, a.random<double>()));
}

TEST_F(DefaultWaveModelTest, more_interesting_example_with_rotation_180p_around_Y)
{
    TR1(shared_ptr)<Kinematics> k(new Kinematics());
    const Point P("body", 0, 0, 1);
    const Point Q("NED", 7, -100, 1795);
    const DefaultWaveModel w(7);
    RotationMatrix rot;
    rot(0,0)=-1.0;
    rot(0,1)=0.0;
    rot(0,2)=0.0;
    rot(1,0)=0.0;
    rot(1,1)=1.0;
    rot(1,2)=0.0;
    rot(2,0)=0.0;
    rot(2,1)=0.0;
    rot(2,2)=-1.0;
    const kinematics::Transform bodyTned(Point("NED", 4, 5, 9), rot, "body");
    k->add(bodyTned);
    ASSERT_DOUBLE_EQ(1, w.get_relative_wave_height(P,k, a.random<double>()));
    ASSERT_DOUBLE_EQ(1788, w.get_relative_wave_height(Q,k, a.random<double>()));
}

TEST_F(DefaultWaveModelTest, more_interesting_example_with_rotation_with_rotation_090p_around_Y)
{
    TR1(shared_ptr)<Kinematics> k(new Kinematics());
    const Point P("body", 0, 0, 1);
    const Point Q("NED", 7, -100, 1795);
    const DefaultWaveModel w(7);
    RotationMatrix rot;
    rot(0,0)= 0.0;
    rot(0,1)= 0.0;
    rot(0,2)= 1.0;
    rot(1,0)= 0.0;
    rot(1,1)= 1.0;
    rot(1,2)= 0.0;
    rot(2,0)=-1.0;
    rot(2,1)= 0.0;
    rot(2,2)= 0.0;
    const kinematics::Transform bodyTned(Point("NED", 4, 5, 9), rot, "body");
    k->add(bodyTned);
    ASSERT_DOUBLE_EQ(2, w.get_relative_wave_height(P,k, a.random<double>()));
    ASSERT_DOUBLE_EQ(1788, w.get_relative_wave_height(Q,k, a.random<double>()));
}

TEST_F(DefaultWaveModelTest, more_interesting_example_with_rotation)
{
    TR1(shared_ptr)<Kinematics> k(new Kinematics());
    const Point P("body", 0, 0, 1);
    const Point Q("NED", 7, -100, 1795);
    const DefaultWaveModel w(7);
    RotationMatrix rot;
    rot(0,0)=-1.0;
    rot(0,1)=0.0;
    rot(0,2)=0.0;
    rot(1,0)=0.0;
    rot(1,1)=1.0;
    rot(1,2)=0.0;
    rot(2,0)=0.0;
    rot(2,1)=0.0;
    rot(2,2)=-1.0;
    const kinematics::Transform bodyTned(Point("NED", 4, 5, 9), rot, "body");
    k->add(bodyTned);
    ASSERT_DOUBLE_EQ(1, w.get_relative_wave_height(P,k, a.random<double>()));
    ASSERT_DOUBLE_EQ(1788, w.get_relative_wave_height(Q,k, a.random<double>()));
}

TEST_F(DefaultWaveModelTest, can_compute_relative_wave_height_for_PointMatrix)
{
    //! [DefaultWaveModelTest example]
    PointMatrix M("body", 5);
    M.m(0,0) = 12;
    M.m(1,0) = -11;
    M.m(2,0) = 10;
    M.m(0,1) = -19;
    M.m(1,1) = 81;
    M.m(2,1) = 71;
    M.m(0,2) = 16;
    M.m(1,2) = -51;
    M.m(2,2) = 14;
    M.m(0,3) = 31;
    M.m(1,3) = -12;
    M.m(2,3) = -11;
    M.m(0,4) = 10;
    M.m(1,4) = -81;
    M.m(2,4) = -91;

    TR1(shared_ptr)<Kinematics> k(new Kinematics());
    const DefaultWaveModel w(888);
    const kinematics::Transform T(Point("NED", 1, 20, 300), "body");
    k->add(T);

    const std::vector<double> delta = w.get_relative_wave_height(M,k, a.random<double>());
    //! [DefaultWaveModelTest example]
    //! [DefaultWaveModelTest expected output]
    ASSERT_EQ(5, delta.size());
    ASSERT_DOUBLE_EQ(-888+300+10, delta.at(0));
    ASSERT_DOUBLE_EQ(-888+300+71, delta.at(1));
    ASSERT_DOUBLE_EQ(-888+300+14, delta.at(2));
    ASSERT_DOUBLE_EQ(-888+300-11, delta.at(3));
    ASSERT_DOUBLE_EQ(-888+300-91, delta.at(4));
    //! [DefaultWaveModelTest expected output]
}
