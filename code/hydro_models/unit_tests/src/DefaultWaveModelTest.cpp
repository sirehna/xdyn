/*
 * HydrostaticForceTest.cpp
 *
 *  Created on: 31 mars 2014
 *      Author: maroff
 */

#include "DefaultWaveModelTest.hpp"
#include "DefaultWaveModel.hpp"
#include "Point.hpp"
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
