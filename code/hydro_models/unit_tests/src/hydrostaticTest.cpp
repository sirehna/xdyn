/*
 * hydrostaticTest.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#include "hydrostaticTest.hpp"
#include "hydrostatic.hpp"
#include "TriMeshTestData.hpp"
#include "MeshBuilder.hpp"
#include "MeshIntersector.hpp"
#include "mesh_manipulations.hpp"
#include <ssc/macros.hpp>
#include "STL_data.hpp"
#include "StlReader.hpp"

#define EPS 1E-6

hydrostaticTest::hydrostaticTest() : a(ssc::random_data_generator::DataGenerator(2))
{
}

hydrostaticTest::~hydrostaticTest()
{
}

void hydrostaticTest::SetUp()
{
}

void hydrostaticTest::TearDown()
{
}

Matrix3x hydrostaticTest::get_rectangle(const double w, const double h) const
{
    Matrix3x rectangle_vertices(3,4);
    rectangle_vertices <<
            -w/2 , +w/2 , +w/2 , -w/2 ,
            -h/2 , -h/2 , +h/2 , +h/2 ,
             0   ,  0   ,  0   ,  0   ;
    return rectangle_vertices;
}

using namespace hydrostatic;

TEST_F(hydrostaticTest, can_compute_average_immersion)
{
//! [hydrostaticTest average_immersion_example]
    const Mesh mesh = MeshBuilder(trapezium()).build();
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double h = a.random<double>();
        ASSERT_DOUBLE_EQ(h/2., average_immersion(mesh.facets.front().vertex_index, {0,0,h,h}));
    }
//! [hydrostaticTest average_immersion_example]
}

TEST_F(hydrostaticTest, inertia_of_rectangle)
{
    const double w = a.random<double>().between(-2,2);
    const double h = a.random<double>().between(-2,2);
    const auto all_nodes = get_rectangle(w,h);

    const std::vector<double> v = {1,2,3,4};

    const MeshPtr mesh(new Mesh(MeshBuilder(all_nodes).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);
    const auto Id=Eigen::MatrixXd::Identity(3,3);
    const auto J0=hydrostatic::get_inertia_of_polygon_wrt(intersector.begin_immersed() , Id , mesh->all_nodes );

    double Jx = h*std::pow(w,3) / 12.0;
    double Jy = w*std::pow(h,3) / 12.0;
    double area = w*h;
    Eigen::Matrix3d expected;
    expected <<
             Jx ,  0 ,  0  ,
             0  , Jy ,  0  ,
             0  ,  0 ,  0  ;
    ASSERT_LT((expected/area - J0).array().abs().maxCoeff(),1.0e-10);
}

TEST_F(hydrostaticTest, inertia_of_rotated_rectangle)
{
    const double w = a.random<double>().between(-2,2);
    const double h = a.random<double>().between(-2,2);
    const auto all_nodes = get_rectangle(w,h);

    const std::vector<double> v = {1,2,3,4};

    const MeshPtr mesh(new Mesh(MeshBuilder(all_nodes).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);

    const double theta = a.random<double>().between(0,2*std::acos(1.0));
    const double c = std::cos(theta);
    const double s = std::sin(theta);
    Eigen::Matrix3d R10;
    R10 <<
             c , s , 0 ,
            -s , c , 0 ,
             0 , 0 , 1 ;
    const auto R01 = R10.transpose();

    const auto Id=Eigen::MatrixXd::Identity(3,3);
    const auto J0=hydrostatic::get_inertia_of_polygon_wrt(intersector.begin_immersed() , Id  , mesh->all_nodes );
    const auto J1=hydrostatic::get_inertia_of_polygon_wrt(intersector.begin_immersed() , R10 , mesh->all_nodes );

    const auto expected = R10*J0*R01;
    ASSERT_LT((expected - J1).array().abs().maxCoeff(),1.0e-10);
}
