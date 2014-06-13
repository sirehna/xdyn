/*
 * GravityModuleTest.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#include "WaveModuleTest.hpp"
#include "WaveModule.hpp"
#include "DataSource.hpp"
#include "TriMeshTestData.hpp"
#include "Kinematics.hpp"
#include "DefaultWaveModel.hpp"
#include "MeshBuilder.hpp"
#include "PointMatrix.hpp"
#include "Transform.hpp"

WaveModuleTest::WaveModuleTest() : a(DataGenerator(822))
{
}

WaveModuleTest::~WaveModuleTest()
{
}

void WaveModuleTest::SetUp()
{
}

void WaveModuleTest::TearDown()
{
}

TEST_F(WaveModuleTest, example)
{
//! [WaveModuleTest example]
    DataSource ds;
    WaveModule g(&ds, "waves", "body 1");
    ds.add(g);
    TR1(shared_ptr)<Kinematics> k(new Kinematics());
    TR1(shared_ptr)<WaveModelInterface> wave(new DefaultWaveModel(10));
    ds.set<TR1(shared_ptr)<WaveModelInterface> >("wave model", wave);
    ds.set<TR1(shared_ptr)<Kinematics> >("kinematics", k);
    k->add(kinematics::Transform(Point("NED",0,0,0),"mesh(body 1)"));
    const TR1(shared_ptr)<Mesh> mesh(new Mesh(MeshBuilder(one_triangle()).build()));
    ds.set<TR1(shared_ptr)<Mesh> >("body 1", mesh);
    ds.set<TR1(shared_ptr)<PointMatrix> >("body 1", TR1(shared_ptr)<PointMatrix>(new PointMatrix(mesh->nodes,"mesh(body 1)")));

//! [WaveModuleTest example]
//! [WaveModuleTest expected output]
    const std::vector<double> dz = ds.get<std::vector<double> >("wave heights(body 1)");
    ASSERT_EQ(mesh->nodes.cols(), dz.size());
    ASSERT_DOUBLE_EQ(-6.8, dz.at(0));
    ASSERT_DOUBLE_EQ(-5.5, dz.at(1));
    ASSERT_DOUBLE_EQ(-3.3, dz.at(2));
//! [WaveModuleTest expected output]
}
