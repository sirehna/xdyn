/*
 * PolygonTest.cpp
 *
 *  Created on: Aug 27, 2014
 *      Author: cady
 */

#include "Mesh.hpp"
#include "TriMeshTestData.hpp"
#include "PolygonTest.hpp"
#include "Polygon.hpp"

PolygonTest::PolygonTest() : a(DataGenerator(846531231))
{
}

PolygonTest::~PolygonTest()
{
}

void PolygonTest::SetUp()
{
}

void PolygonTest::TearDown()
{
}

TEST_F(PolygonTest, example)
{
//! [PolygonTest example]
    //const double side = a.random<double>();
    //const MeshPtr mesh(new Mesh(MeshBuilder(cube(side, a.random<double>(), a.random<double>(), a.random<double>())).build()));
    /*const size_t facet_idx = a.random<size_t>().between(0,7);
    const Polygon polygon_from_mesh(mesh, facet_idx);
//! [PolygonTest example]
//! [PolygonTest expected output]
    ASSERT_EQ(side*side/2, polygon_from_mesh.area());
    const Polygon polygon_from_matrix(mesh->nodes.block(0,0,3,3));
    ASSERT_EQ(side*side/2, polygon_from_matrix.area());*/
//! [PolygonTest expected output]
}



