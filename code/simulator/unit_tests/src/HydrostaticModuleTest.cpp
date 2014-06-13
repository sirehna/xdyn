/*
 * GravityModuleTest.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#include "HydrostaticModuleTest.hpp"
#include "HydrostaticModule.hpp"
#include "DataSource.hpp"
#include "Wrench.hpp"
#include "TriMeshTestData.hpp"
#include "MeshBuilder.hpp"

HydrostaticModuleTest::HydrostaticModuleTest() : a(DataGenerator(822))
{
}

HydrostaticModuleTest::~HydrostaticModuleTest()
{
}

void HydrostaticModuleTest::SetUp()
{
}

void HydrostaticModuleTest::TearDown()
{
}

TEST_F(HydrostaticModuleTest, example)
{
//! [HydrostaticModuleTest example]
    DataSource ds;
    ds.set<double>("g", 10);
    ds.set<Point>("G(body 1)", Point("body 1", 1,2,4));
    ds.set<double>("rho", 1024);
    ds.set<TR1(shared_ptr)<Mesh> >("body 1", TR1(shared_ptr)<Mesh>(new Mesh(MeshBuilder(two_triangles()).build())));
    ds.set<std::vector<double> >("wave heights(body 1)", std::vector<double>({-0.5,-0.5,-2.5,0.5}));
    const HydrostaticModule hs(&ds, "hydrostatic module", "body 1");
    ds.add(hs);
    const Wrench Fhs = ds.get<Wrench>("non-linear hydrostatic(body 1)");
//! [HydrostaticModuleTest example]
//! [HydrostaticModuleTest expected output]
    ASSERT_DOUBLE_EQ(0, Fhs.X);
    ASSERT_DOUBLE_EQ(0, Fhs.Y);
    const double dz = 0.5/3;
    const double dS = 0.5;
    ASSERT_DOUBLE_EQ(0, Fhs.X);
    ASSERT_DOUBLE_EQ(0, Fhs.Y);
    ASSERT_DOUBLE_EQ(-1024*10*dz*dS, Fhs.Z);
    ASSERT_DOUBLE_EQ(-8/3.*Fhs.Z, Fhs.K);
    ASSERT_DOUBLE_EQ(-Fhs.Z, Fhs.M);
    ASSERT_DOUBLE_EQ(0, Fhs.N);
//! [HydrostaticModuleTest expected output]
}
