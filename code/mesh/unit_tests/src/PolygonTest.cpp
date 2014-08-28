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
    const double side = a.random<double>();
    const double x0 = a.random<double>();
    const double y0 = a.random<double>();
    const double z0 = a.random<double>();
    const MeshPtr mesh(new Mesh(MeshBuilder(cube(side, x0, y0, z0)).build()));
    const size_t facet_idx = a.random<size_t>().between(0,5);

    const Polygon polygon_from_mesh(mesh, facet_idx);
    const Matrix3x M = mesh->nodes.block(0,0,3,4);
    const Polygon polygon_from_matrix(M);

//! [PolygonTest example]
//! [PolygonTest expected output]
    ASSERT_DOUBLE_EQ(side*side, polygon_from_mesh.get_area());

    ASSERT_DOUBLE_EQ(side*side, polygon_from_matrix.get_area());
//! [PolygonTest expected output]
}

TEST_F(PolygonTest, unit_normal)
{
    //! [PolygonTest unit_normal_example]
    const double side = a.random<double>();
    const double x0 = a.random<double>();
    const double y0 = a.random<double>();
    const double z0 = a.random<double>();
    const MeshPtr mesh(new Mesh(MeshBuilder(cube(side, x0, y0, z0)).build()));
    const Polygon polygon_from_mesh(mesh, 0);
    const Matrix3x M = mesh->nodes.block(0,0,3,4);
    const Polygon polygon_from_matrix(M);

    const EPoint normal_to_polygon_from_mesh = polygon_from_mesh.get_unit_normal();
    const EPoint normal_to_polygon_from_matrix = polygon_from_matrix.get_unit_normal();

    ASSERT_DOUBLE_EQ((double)normal_to_polygon_from_mesh(0), (double)normal_to_polygon_from_matrix(0));
    ASSERT_DOUBLE_EQ((double)normal_to_polygon_from_mesh(1), (double)normal_to_polygon_from_matrix(1));
    ASSERT_DOUBLE_EQ((double)normal_to_polygon_from_mesh(2), (double)normal_to_polygon_from_matrix(2));

    ASSERT_DOUBLE_EQ(0, (double)normal_to_polygon_from_mesh(0));
    ASSERT_DOUBLE_EQ(0, (double)normal_to_polygon_from_mesh(1));
    ASSERT_DOUBLE_EQ(-1, (double)normal_to_polygon_from_mesh(2));
    //! [PolygonTest unit_normal_example]
}

TEST_F(PolygonTest, barycenter)
{
    //! [PolygonTest barycenter_example]
    const double side = a.random<double>();
    const double x0 = a.random<double>();
    const double y0 = a.random<double>();
    const double z0 = a.random<double>();
    const MeshPtr mesh(new Mesh(MeshBuilder(cube(side, x0, y0, z0)).build()));
    const Polygon polygon_from_mesh(mesh, 0);
    const Matrix3x M = mesh->nodes.block(0,0,3,4);
    const Polygon polygon_from_matrix(M);

    const EPoint barycenter_of_polygon_from_mesh = polygon_from_mesh.get_barycenter();
    const EPoint barycenter_of_polygon_from_matrix = polygon_from_matrix.get_barycenter();

    ASSERT_DOUBLE_EQ((double)barycenter_of_polygon_from_mesh(0), (double)barycenter_of_polygon_from_matrix(0));
    ASSERT_DOUBLE_EQ((double)barycenter_of_polygon_from_mesh(1), (double)barycenter_of_polygon_from_matrix(1));
    ASSERT_DOUBLE_EQ((double)barycenter_of_polygon_from_mesh(2), (double)barycenter_of_polygon_from_matrix(2));

    ASSERT_DOUBLE_EQ(x0, (double)barycenter_of_polygon_from_mesh(0));
    ASSERT_DOUBLE_EQ(y0, (double)barycenter_of_polygon_from_mesh(1));
    ASSERT_DOUBLE_EQ(z0-side/2., (double)barycenter_of_polygon_from_mesh(2));
    //! [PolygonTest unit_normal_example]
}


TEST_F(PolygonTest, inertia)
{
	Matrix3x square_vertices(3,4);
	square_vertices <<
			-2 , +2 , +2 , -2 ,
			-2 , -2 , +2 , +2 ,
			 0 ,  0 ,  0 ,  0 ;
	const Polygon square(square_vertices);
	const Eigen::Matrix3d J=square.get_inertia_wrt(Eigen::MatrixXd::Identity(3,3));
	Eigen::Matrix3d expected;
	double side = 4;
	double Jx = std::pow(side,4) / 12.0;
	double area = std::pow(side,2);
	expected <<
			 Jx ,  0 ,  0  ,
			 0  , Jx ,  0  ,
			 0  ,  0 ,  0  ;
	ASSERT_LT((expected/area - J).array().abs().maxCoeff(),1.0e-10);
}
