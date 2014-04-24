/*
 * HydrostaticForceTest.cpp
 *
 *  Created on: 31 mars 2014
 *      Author: maroff
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
    std::tr1::shared_ptr<Kinematics> k(new Kinematics());
    const DefaultWaveModel w(10, k);
    const Point P("NED", 0, 0, -20);
//! [DefaultWaveModelTest example]
//! [DefaultWaveModelTest expected output]
    ASSERT_DOUBLE_EQ(30, w.get_relative_wave_height(P));
//! [DefaultWaveModelTest expected output]
}

TEST_F(DefaultWaveModelTest, more_interesting_example)
{
    std::tr1::shared_ptr<Kinematics> k(new Kinematics());
    const Point P("body", 0, 0, 0);
    const Point Q("NED", 7, -100, 1795);
    const DefaultWaveModel w(7, k);

    const kinematics::Transform bodyTned(Point("NED", 4, 5, 9), "body");
    k->add(bodyTned);
    ASSERT_DOUBLE_EQ(-2, w.get_relative_wave_height(P));
    ASSERT_DOUBLE_EQ(-1788, w.get_relative_wave_height(Q));
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

    std::tr1::shared_ptr<Kinematics> k(new Kinematics());
    const DefaultWaveModel w(888, k);
    const kinematics::Transform T(Point("NED", 1, 20, 300), "body");
    k->add(T);

    const std::vector<double> delta = w.get_relative_wave_height(M);
    //! [DefaultWaveModelTest example]
    //! [DefaultWaveModelTest expected output]
    ASSERT_EQ(5, delta.size());
    ASSERT_DOUBLE_EQ(888-300-10, delta.at(0));
    ASSERT_DOUBLE_EQ(888-300-71, delta.at(1));
    ASSERT_DOUBLE_EQ(888-300-14, delta.at(2));
    ASSERT_DOUBLE_EQ(888-300+11, delta.at(3));
    ASSERT_DOUBLE_EQ(888-300+91, delta.at(4));
    //! [DefaultWaveModelTest expected output]
}
