/*
 * PolygonTest.cpp
 *
 *  Created on: Aug 27, 2014
 *      Author: cady
 */

#include "Mesh.hpp"
#include "MeshBuilder.hpp"
#include "PolygonTest.hpp"
#include "Polygon.hpp"
#include "TriMeshTestData.hpp"
#include "test_macros.hpp"
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
    const double side = 1;//a.random<double>();
    const double x0 = 0;//a.random<double>();
    const double y0 = 0;//a.random<double>();
    const double z0 = 0;//a.random<double>();
    const MeshPtr mesh(new Mesh(MeshBuilder(cube(side, x0, y0, z0)).build()));
    const size_t facet_idx = a.random<size_t>().between(0,5);

    const Polygon polygon_from_mesh(mesh, facet_idx);
    const Matrix3x M = mesh->nodes.block(0,0,3,4);
    const Polygon polygon_from_matrix(M);

//! [PolygonTest example]
//! [PolygonTest expected output]
    ASSERT_EQ(side*side, polygon_from_mesh.get_area());

    ASSERT_EQ(side*side, polygon_from_matrix.get_area());
//! [PolygonTest expected output]
}



