/*
 * GravityModuleTest.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#include "GravityModuleTest.hpp"
#include "GravityModule.hpp"
#include "DataSource.hpp"
#include "Wrench.hpp"

GravityModuleTest::GravityModuleTest() : a(DataGenerator(822))
{
}

GravityModuleTest::~GravityModuleTest()
{
}

void GravityModuleTest::SetUp()
{
}

void GravityModuleTest::TearDown()
{
}

TEST_F(GravityModuleTest, example)
{
//! [GravityModuleTest example]
    DataSource ds;
    GravityModule g(&ds, "gravity", "body 1");
    ds.add(g);
    ds.set<double>("m(body 1)", 1234);
    ds.set<double>("g", 10);
    ds.set<Point>("G(body 1)", Point("f", 1,78,95));
//! [GravityModuleTest example]
//! [GravityModuleTest expected output]
    const Wrench Fg = ds.get<Wrench>("gravity(body 1)");
    ASSERT_EQ("f", Fg.get_point().get_frame());
    ASSERT_DOUBLE_EQ(1, Fg.get_point().x);
    ASSERT_DOUBLE_EQ(78, Fg.get_point().y);
    ASSERT_DOUBLE_EQ(95, Fg.get_point().z);
    ASSERT_DOUBLE_EQ(0,Fg.X);
    ASSERT_DOUBLE_EQ(0,Fg.Y);
    ASSERT_DOUBLE_EQ(12340,Fg.Z);
    ASSERT_DOUBLE_EQ(0,Fg.K);
    ASSERT_DOUBLE_EQ(0,Fg.M);
    ASSERT_DOUBLE_EQ(0,Fg.N);
//! [GravityModuleTest expected output]
}
