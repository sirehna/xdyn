/*
 * hydrostaticTest.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#include "hydrostaticTest.hpp"
#include "hydrostatic.hpp"
#include "HydrostaticException.hpp"
#include "TriMeshTestData.hpp"
#include "MeshBuilder.hpp"
#include "mesh_manipulations.hpp"
#include "extra_test_assertions.hpp"
#include "STL_data.hpp"
#include "StlReader.hpp"

#define EPS 1E-6

hydrostaticTest::hydrostaticTest() : a(DataGenerator(2))
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

using namespace hydrostatic;

TEST_F(hydrostaticTest, can_compute_average_immersion)
{
//! [hydrostaticTest average_immersion_example]
    const Mesh mesh = MeshBuilder(trapezium()).build();
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double h = a.random<double>();
        ASSERT_DOUBLE_EQ(5*h/12., average_immersion(mesh.nodes, mesh.facets.front().vertex_index, {0,0,h,h}));
    }
//! [hydrostaticTest average_immersion_example]
}

TEST_F(hydrostaticTest, can_compute_average_immersion_even_when_all_nodes_are_used)
{
    const Mesh mesh = MeshBuilder(one_triangle()).build();
    ASSERT_DOUBLE_EQ(5, average_immersion(std::make_pair(mesh.nodes, std::vector<double>({4,5,6}))));
}

TEST_F(hydrostaticTest, average_immersion_should_throw_if_index_does_not_have_the_right_size)
{
    const Mesh mesh = MeshBuilder(one_triangle()).build();
    ASSERT_THROW(average_immersion(std::make_pair(mesh.nodes, std::vector<double>({4,5}))), HydrostaticException);
    ASSERT_THROW(average_immersion(std::make_pair(mesh.nodes, std::vector<double>({4,5,6,7}))), HydrostaticException);
}

TEST_F(hydrostaticTest, can_compute_average_immersion_even_when_not_all_nodes_are_used)
{
    const Mesh mesh = MeshBuilder(two_triangles()).build();
    ASSERT_DOUBLE_EQ(7/3., average_immersion(mesh.nodes, mesh.facets.back().vertex_index, {1,2,3,4}));
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_for_one_immerged_node)
{
//! [hydrostaticTest immerged_polygon_example_1]
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1; M(0,1) = 4; M(0,2) = 7;
    M(1,0) = 1; M(1,1) = 6; M(1,2) = 8;
    M(2,0) = 3; M(2,1) = 6; M(2,2) = 9;

    const std::vector<double> v({-1,-1,1});

    Mesh mesh(MeshBuilder(M).build());
    mesh.update_intersection_with_free_surface(v);
    const auto polygon    = mesh.coordinates_of_facet(mesh.list_of_facets_immersed.at(0));
    const auto immersions = mesh.immersions_of_facet(mesh.list_of_facets_immersed.at(0));

    ASSERT_EQ(3, polygon.cols());
    ASSERT_DOUBLE_EQ(4  ,polygon(0,2));
    ASSERT_DOUBLE_EQ(4.5,polygon(1,2));
    ASSERT_DOUBLE_EQ(6  ,polygon(2,2));
    ASSERT_DOUBLE_EQ(5.5,polygon(0,0));
    ASSERT_DOUBLE_EQ(7  ,polygon(1,0));
    ASSERT_DOUBLE_EQ(7.5,polygon(2,0));
    ASSERT_DOUBLE_EQ(7  ,polygon(0,1));
    ASSERT_DOUBLE_EQ(8  ,polygon(1,1));
    ASSERT_DOUBLE_EQ(9  ,polygon(2,1));
    ASSERT_EQ(3, immersions.size());
    ASSERT_DOUBLE_EQ(0,immersions.at(2));
    ASSERT_DOUBLE_EQ(0,immersions.at(0));
    ASSERT_DOUBLE_EQ(1,immersions.at(1));
}


TEST_F(hydrostaticTest, can_compute_immerged_polygon_for_two_immerged_node)
{
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1; M(0,1) = 4; M(0,2) = 7;
    M(1,0) = 1; M(1,1) = 6; M(1,2) = 8;
    M(2,0) = 3; M(2,1) = 6; M(2,2) = 9;

    const std::vector<double> v({-1,1,1});

    Mesh mesh(MeshBuilder(M).build());
    mesh.update_intersection_with_free_surface(v);
    const auto polygon    = mesh.coordinates_of_facet(mesh.list_of_facets_immersed.at(0));
    const auto immersions = mesh.immersions_of_facet(mesh.list_of_facets_immersed.at(0));

    ASSERT_EQ(4, polygon.cols());
    ASSERT_DOUBLE_EQ(4  ,polygon(0,3));
    ASSERT_DOUBLE_EQ(4.5,polygon(1,3));
    ASSERT_DOUBLE_EQ(6  ,polygon(2,3));
    ASSERT_DOUBLE_EQ(2.5,polygon(0,0));
    ASSERT_DOUBLE_EQ(3.5,polygon(1,0));
    ASSERT_DOUBLE_EQ(4.5,polygon(2,0));
    ASSERT_DOUBLE_EQ(4  ,polygon(0,1));
    ASSERT_DOUBLE_EQ(6  ,polygon(1,1));
    ASSERT_DOUBLE_EQ(6  ,polygon(2,1));
    ASSERT_DOUBLE_EQ(7  ,polygon(0,2));
    ASSERT_DOUBLE_EQ(8  ,polygon(1,2));
    ASSERT_DOUBLE_EQ(9  ,polygon(2,2));
    ASSERT_EQ(4, immersions.size());
    ASSERT_DOUBLE_EQ(0,immersions.at(3));
    ASSERT_DOUBLE_EQ(0,immersions.at(0));
    ASSERT_DOUBLE_EQ(1,immersions.at(1));
    ASSERT_DOUBLE_EQ(1,immersions.at(2));
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_when_emerged_nodes_are_the_first_nodes)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    const std::vector<double> v({-1,-1,-1,1,1});

    Mesh mesh(MeshBuilder(M).build());
    mesh.update_intersection_with_free_surface(v);
    const auto polygon    = mesh.coordinates_of_facet(mesh.list_of_facets_immersed.at(0));
    const auto immersions = mesh.immersions_of_facet(mesh.list_of_facets_immersed.at(0));

    ASSERT_EQ(4, polygon.cols());
    ASSERT_DOUBLE_EQ(5  ,polygon(0,3));
    ASSERT_DOUBLE_EQ(5  ,polygon(1,3));
    ASSERT_DOUBLE_EQ(6.5,polygon(2,3));
    ASSERT_DOUBLE_EQ(4.5,polygon(0,0));
    ASSERT_DOUBLE_EQ(5  ,polygon(1,0));
    ASSERT_DOUBLE_EQ(5.5,polygon(2,0));
    ASSERT_DOUBLE_EQ(5  ,polygon(0,1));
    ASSERT_DOUBLE_EQ(6  ,polygon(1,1));
    ASSERT_DOUBLE_EQ(6  ,polygon(2,1));
    ASSERT_DOUBLE_EQ(9  ,polygon(0,2));
    ASSERT_DOUBLE_EQ(9  ,polygon(1,2));
    ASSERT_DOUBLE_EQ(11 ,polygon(2,2));
    ASSERT_EQ(4, immersions.size());
    ASSERT_DOUBLE_EQ(0,immersions.at(3));
    ASSERT_DOUBLE_EQ(0,immersions.at(0));
    ASSERT_DOUBLE_EQ(1,immersions.at(1));
    ASSERT_DOUBLE_EQ(1,immersions.at(2));
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_when_emerged_nodes_are_the_last_nodes)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    const std::vector<double> v({1,1,-1,-1,-1});

    Mesh mesh(MeshBuilder(M).build());
    mesh.update_intersection_with_free_surface(v);
    const auto polygon    = mesh.coordinates_of_facet(mesh.list_of_facets_immersed.at(0));
    const auto immersions = mesh.immersions_of_facet(mesh.list_of_facets_immersed.at(0));

    ASSERT_EQ(4, polygon.cols());
    ASSERT_DOUBLE_EQ(1  ,polygon(0,3));
    ASSERT_DOUBLE_EQ(1  ,polygon(1,3));
    ASSERT_DOUBLE_EQ(2  ,polygon(2,3));
    ASSERT_DOUBLE_EQ(2  ,polygon(0,0));
    ASSERT_DOUBLE_EQ(3  ,polygon(1,0));
    ASSERT_DOUBLE_EQ(3  ,polygon(2,0));
    ASSERT_DOUBLE_EQ(3  ,polygon(0,1));
    ASSERT_DOUBLE_EQ(3.5,polygon(1,1));
    ASSERT_DOUBLE_EQ(4  ,polygon(2,1));
    ASSERT_DOUBLE_EQ(5  ,polygon(0,2));
    ASSERT_DOUBLE_EQ(5  ,polygon(1,2));
    ASSERT_DOUBLE_EQ(6.5,polygon(2,2));
    ASSERT_EQ(4, immersions.size());
    ASSERT_DOUBLE_EQ(1,immersions.at(3));
    ASSERT_DOUBLE_EQ(1,immersions.at(0));
    ASSERT_DOUBLE_EQ(0,immersions.at(1));
    ASSERT_DOUBLE_EQ(0,immersions.at(2));
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_when_emerged_nodes_are_in_the_middle)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    const std::vector<double> v({1,-1,-1,-1,1});

    Mesh mesh(MeshBuilder(M).build());
    mesh.update_intersection_with_free_surface(v);
    const auto polygon    = mesh.coordinates_of_facet(mesh.list_of_facets_immersed.at(0));
    const auto immersions = mesh.immersions_of_facet(mesh.list_of_facets_immersed.at(0));

    ASSERT_EQ(4, polygon.cols());
    ASSERT_DOUBLE_EQ(1  ,polygon(0,3));
    ASSERT_DOUBLE_EQ(1  ,polygon(1,3));
    ASSERT_DOUBLE_EQ(2  ,polygon(2,3));
    ASSERT_DOUBLE_EQ(1.5,polygon(0,0));
    ASSERT_DOUBLE_EQ(2  ,polygon(1,0));
    ASSERT_DOUBLE_EQ(2.5,polygon(2,0));
    ASSERT_DOUBLE_EQ(7  ,polygon(0,1));
    ASSERT_DOUBLE_EQ(7.5,polygon(1,1));
    ASSERT_DOUBLE_EQ(8.5,polygon(2,1));
    ASSERT_DOUBLE_EQ(9  ,polygon(0,2));
    ASSERT_DOUBLE_EQ(9  ,polygon(1,2));
    ASSERT_DOUBLE_EQ(11 ,polygon(2,2));
    ASSERT_EQ(4, immersions.size());
    ASSERT_DOUBLE_EQ(1,immersions.at(3));
    ASSERT_DOUBLE_EQ(0,immersions.at(0));
    ASSERT_DOUBLE_EQ(0,immersions.at(1));
    ASSERT_DOUBLE_EQ(1,immersions.at(2));
}

TEST_F(hydrostaticTest, immerged_polygon_should_throw_if_all_points_are_immerged)
{
    Eigen::Matrix<double,3,3> M;
    M <<
        1 , 4 , 7 ,
        2 , 5 , 8 ,
        3 , 6 , 9 ;
    std::vector<size_t> index(3);
    const std::vector<double> v({1,2,5});

    Mesh mesh(MeshBuilder(M).build());
    mesh.update_intersection_with_free_surface(v);
    ASSERT_EQ(0,mesh.list_of_facets_emerged.size());
    ASSERT_EQ(0,mesh.list_of_facets_immersed.at(0));
}

TEST_F(hydrostaticTest, can_compute_intersection_with_the_surface)
{
    const EPoint A(1,5,9);
    const EPoint B(98,2,0);
    const double dzA = 77;
    const double dzB = -1;
    const EPoint I = Mesh::edge_intersection(A, dzA, B, dzB);
    ASSERT_NEAR(77./78.*97+1, (double)I(0),EPS*EPS);
    ASSERT_NEAR(5-77./78.*3, (double)I(1),EPS*EPS);
    ASSERT_NEAR(9-77./78.*9, (double)I(2),EPS*EPS);
}

TEST_F(hydrostaticTest, can_compute_intersection_with_the_surface_second_test)
{
    const EPoint A(-98,-2,9);
    const EPoint B(98,2,-9);
    const double dzA = 9;
    const double dzB = -9;
    const EPoint I = Mesh::edge_intersection(A, dzA, B, dzB);
    ASSERT_DOUBLE_EQ(0, I(0));
    ASSERT_DOUBLE_EQ(0, I(1));
    ASSERT_DOUBLE_EQ(0, I(2));
}

TEST_F(hydrostaticTest, can_compute_intersection_with_the_surface_third_test)
{
    const EPoint A(-1,-2,10);
    const EPoint B(99,2,-10);
    const double dzA = 1;
    const double dzB = -3;
    const EPoint I = Mesh::edge_intersection(A, dzA, B, dzB);
    ASSERT_DOUBLE_EQ(24, I(0));
    ASSERT_DOUBLE_EQ(-1, I(1));
    ASSERT_DOUBLE_EQ(5, I(2));
}

TEST_F(hydrostaticTest, can_compute_intersection_with_the_surface_bug_detected_in_integration_test)
{
    const EPoint A(-1,-2,10);
    const EPoint B(99,2,-10);
    const double dzA = 1;
    const double dzB = -3;
    const EPoint I = Mesh::edge_intersection(A, dzA, B, dzB);
    ASSERT_DOUBLE_EQ(24, I(0));
    ASSERT_DOUBLE_EQ(-1, I(1));
    ASSERT_DOUBLE_EQ(5, I(2));
}

TEST_F(hydrostaticTest, immerged_polygon_should_throw_if_no_points_are_immerged)
{
    Eigen::Matrix<double,3,3> M;
    M <<
        1 , 4 , 7 ,
        2 , 5 , 8 ,
        3 , 6 , 9 ;
    const std::vector<double> v({-1,-2,-5});

    Mesh mesh(MeshBuilder(M).build());
    mesh.update_intersection_with_free_surface(v);
    ASSERT_EQ(0,mesh.list_of_facets_immersed.size());
    ASSERT_EQ(0,mesh.list_of_facets_emerged.at(0));
}

TEST_F(hydrostaticTest, can_compute_the_elementary_hydrostatic_force)
{
    const std::string frame = a.random<std::string>();
    const Point O(frame, 1,2,4);
    const Eigen::Vector3d C(78,-4,6);
    const Eigen::Vector3d dS(0,3,4);
    const double rho = 1024;
    const double g = 10;
    const double dz = 3;
    const UnsafeWrench Fhs = dF(O, C, rho, g, dz, dS);

    ASSERT_EQ(frame, Fhs.get_frame());

    ASSERT_DOUBLE_EQ(0, Fhs.X());
    ASSERT_DOUBLE_EQ(-1024*10*3*3, Fhs.Y());
    ASSERT_DOUBLE_EQ(-1024*10*3*4, Fhs.Z());
    ASSERT_DOUBLE_EQ(-6*Fhs.Z()-2*Fhs.Y(), Fhs.K());
    ASSERT_DOUBLE_EQ(-77*Fhs.Z(), Fhs.M());
    ASSERT_DOUBLE_EQ(77*Fhs.Y(), Fhs.N());
}

TEST_F(hydrostaticTest, another_bug_in_immerged_polygon)
{
    Mesh mesh = MeshBuilder(two_triangles()).build();
    const std::vector<double> z = {-1,-1,-2,1};

    mesh.update_intersection_with_free_surface(z);
    const auto polygon = mesh.coordinates_of_facet(mesh.list_of_facets_immersed.at(0));
    const auto immersions = mesh.immersions_of_facet(mesh.list_of_facets_immersed.at(0));

    ASSERT_DOUBLE_EQ( 1  , polygon(0,0));
    ASSERT_DOUBLE_EQ(-0.5, polygon(1,0));
    ASSERT_DOUBLE_EQ( 0  , polygon(2,0));

    ASSERT_DOUBLE_EQ( 2, polygon(0,1));
    ASSERT_DOUBLE_EQ(-1, polygon(1,1));
    ASSERT_DOUBLE_EQ( 0, polygon(2,1));

    ASSERT_DOUBLE_EQ( 3  , polygon(0,2));
    ASSERT_DOUBLE_EQ(-0.5, polygon(1,2));
    ASSERT_DOUBLE_EQ( 0  , polygon(2,2));

    ASSERT_EQ(3, immersions.size());
    ASSERT_DOUBLE_EQ(0,immersions.at(0));
    ASSERT_DOUBLE_EQ(1,immersions.at(1));
    ASSERT_DOUBLE_EQ(0,immersions.at(2));
}

TEST_F(hydrostaticTest, can_compute_the_hydrostatic_force_on_two_triangles)
{
    const MeshPtr mesh(new Mesh(MeshBuilder(two_triangles()).build()));
    const double rho = 1024;
    const EPoint g(0, 0, 10);
    const Point G(a.random<std::string>(), 1,2,4);
    const std::vector<double> z = {-0.5,-0.5,-2.5,0.5};
    const Wrench Fhs = force(mesh, G, rho, g, z);

    ASSERT_EQ(1,mesh->list_of_facets_immersed.size());
    ASSERT_DOUBLE_EQ(0.5,::area(mesh->coordinates_of_facet(mesh->list_of_facets_immersed.at(0))));
    ASSERT_DOUBLE_EQ(0.5,mesh->facets[mesh->list_of_facets_immersed.at(0)].area);
    ASSERT_DOUBLE_EQ(0.5/3,hydrostatic::average_immersion(mesh->all_nodes, mesh->facets[mesh->list_of_facets_immersed.at(0)].vertex_index, mesh->all_immersions));

    ASSERT_DOUBLE_EQ(0, Fhs.X());
    ASSERT_DOUBLE_EQ(0, Fhs.Y());
    const double dz = 0.5/3;
    const double dS = 0.5;
    ASSERT_DOUBLE_EQ(0, Fhs.X());
    ASSERT_DOUBLE_EQ(0, Fhs.Y());
    ASSERT_DOUBLE_EQ(rho*10*dz*dS, Fhs.Z());
    ASSERT_DOUBLE_EQ(-8/3.*Fhs.Z(), Fhs.K());
    ASSERT_DOUBLE_EQ(-Fhs.Z(), Fhs.M());
    ASSERT_DOUBLE_EQ(0, Fhs.N());
}

TEST_F(hydrostaticTest, can_compute_the_hydrostatic_force_on_a_cube)
{
    for (size_t i = 0 ; i < 10 ; ++i)
    {
        const double L = a.random<double>().between(0,10);
        const double x0 = a.random<double>().between(-1000,1000);
        const double y0 = a.random<double>().between(-1000,1000);
        const double z0 = L/2;
        const Point G(a.random<std::string>(), 0, 0, 0);
        const MeshPtr mesh(new Mesh(MeshBuilder(cube(L,x0,y0,z0)).build()));
        const std::vector<double> dz = {z0-L/2,z0-L/2,z0-L/2,z0-L/2,z0+L/2,z0+L/2,z0+L/2,z0+L/2};
        const double rho = 1000;
        const EPoint g(0, 0, 9.81);
        const Wrench Fhs = force(mesh, G, rho, g, dz);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.X(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.Y(), EPS);
        const double V = L*L*L;
        ASSERT_SMALL_RELATIVE_ERROR(-rho*9.81*V, Fhs.Z(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(-rho*9.81*V*y0, Fhs.K(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(+rho*9.81*V*x0, Fhs.M(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.N(), EPS);
    }
}

TEST_F(hydrostaticTest, should_not_crash)
{
    for (size_t i = 0 ; i < 10 ; ++i)
    {
        const double L = a.random<double>().between(0,10);
        const double x0 = a.random<double>().between(-1000,1000);
        const double y0 = a.random<double>().between(-1000,1000);
        const double z0 = 0.0;
        const double h = 0.5*sqrt(2.0)*L;
        const Point G(a.random<std::string>(), 0, 0, 0);
        const MeshPtr mesh(new Mesh(MeshBuilder(cube(L,x0,y0,z0)).build()));
        const std::vector<double> dz = {-h,0,0,-h,0,h,h,0};
        const double rho = 1000;
        const EPoint g(0, 0, 9.81);
        force(mesh, G, rho, g, dz);
    }
}

TEST_F(hydrostaticTest, can_compute_the_hydrostatic_force_on_a_stl_cube)
{
    const VectorOfVectorOfPoints mesh_cube(read_stl(test_data::cube()));
    const MeshPtr mesh(new Mesh(MeshBuilder(mesh_cube).build()));
    const std::vector<double> dz = {-1,-1,-1,-1,0,0,0,0};
    const Point G(a.random<std::string>(), 0, 0, 0);
    const double rho = 1000;
    const EPoint g(0, 0, 9.81);
    const Wrench Fhs = force(mesh, G, rho, g, dz);
}

TEST_F(hydrostaticTest, can_compute_the_hydrostatic_force_on_a_tetrahedron)
{
    for (size_t i = 0 ; i < 10 ; ++i)
    {
        const double L = a.random<double>().between(0,10);
        const double x0 = a.random<double>().between(-1000,1000);
        const double y0 = a.random<double>().between(-1000,1000);
        const double z0 = a.random<double>().between(0,1000);
        const Point G(a.random<std::string>(), 0, 0, 0);
        const MeshPtr mesh(new Mesh(MeshBuilder(tetrahedron(L,x0,y0,z0)).build()));
        const std::vector<double> dz = {z0, sqrt(6)*L/3 + z0, sqrt(6)*L/3 + z0, sqrt(6)*L/3 + z0};
        const double rho = 1000;
        const EPoint g(0, 0, 9.81);
        const Wrench Fhs = force(mesh, G, rho, g, dz);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.X(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.Y(), EPS);
        const double V = L*L*L/(6.*sqrt(2));
        ASSERT_SMALL_RELATIVE_ERROR(-rho*9.81*V, Fhs.Z(), EPS);
    }
}

//TEST_F(hydrostaticTest, bug_discovered_when_implementing_sum_of_forces)
//{
//    Eigen::Matrix<double,3,4> M;
//    M(0,0) = 1; M(0,1) = -1.879706489;M(0,2) = -1.879706489;M(0,3) = 6.759412979;
//    M(1,0) = 2; M(1,1) = -2.98779795; M(1,2) = 6.98779795;  M(1,3) = 2;
//    M(2,0) = 3; M(2,1) = 11.14503995; M(2,2) = 11.14503995; M(2,3) = 11.14503995;
//
//    const std::vector<double> v = {-204294e4,953171e4,38521e5,-990113e4};
//    const std::vector<size_t> idx = {2,1,3};
//
//    immerged_polygon(M, idx, v);
//    ASSERT_NO_THROW(immerged_polygon(M, idx, v));
//
//    const double L = a.random<double>().between(0,10);
//    const double x0 = 1;
//    const double y0 = 2;
//    const double z0 = 3;
//    const Point G(a.random<std::string>(), -3.63075e+09, -7.71511e+09,  9.07162e+09);
//    const MeshPtr mesh(new Mesh(MeshBuilder(tetrahedron(L,x0,y0,z0)).build()));
//    const std::vector<double> dz = {-2.04294e+09, 9.53171e+09, 3.8521e+09, -9.90113e+09};
//    force(mesh, G, a.random<double>(), a.random<EPoint>(), dz);
//    ASSERT_NO_THROW(force(mesh, G, a.random<double>(), a.random<EPoint>(), dz));
//}

TEST_F(hydrostaticTest, hydrostatic_force_should_be_computed_at_the_right_point)
{
    const double L = a.random<double>().between(0,10);
    const double x0 = 1;
    const double y0 = 2;
    const double z0 = 3;
    const Point G(a.random<std::string>(), a.random<double>(), a.random<double>(), a.random<double>());
    const MeshPtr mesh(new Mesh(MeshBuilder(tetrahedron(L,x0,y0,z0)).build()));
    const std::vector<double> dz = a.random_vector_of<double>().of_size(4);
    const Wrench Fhs = force(mesh, G, a.random<double>(), a.random<EPoint>(), dz);

    ASSERT_EQ(G.get_frame(), Fhs.get_point().get_frame());
    ASSERT_EQ(G.get_frame(), Fhs.get_frame());
    ASSERT_DOUBLE_EQ(G.x(), Fhs.get_point().x());
    ASSERT_DOUBLE_EQ(G.y(), Fhs.get_point().y());
    ASSERT_DOUBLE_EQ(G.z(), Fhs.get_point().z());
}

TEST_F(hydrostaticTest, should_be_able_to_compute_intersection_even_if_one_of_the_points_isexactly_on_the_surface)
{
    const EPoint A(1,2,3);
    const EPoint B(4,5,6);
    const EPoint C(7,8,9);
    const EPoint D(11,12,13);
    const EPoint P = Mesh::edge_intersection(A,0,B,-1);
    const EPoint Q = Mesh::edge_intersection(C,0,D,-1);

    ASSERT_DOUBLE_EQ((double)A(0), (double)P(0));
    ASSERT_DOUBLE_EQ((double)A(1), (double)P(1));
    ASSERT_DOUBLE_EQ((double)A(2), (double)P(2));

    ASSERT_DOUBLE_EQ((double)Q(0), (double)C(0));
    ASSERT_DOUBLE_EQ((double)Q(1), (double)C(1));
    ASSERT_DOUBLE_EQ((double)Q(2), (double)C(2));
}

TEST_F(hydrostaticTest, correct_immerged_polygon_when_two_points_are_exactly_on_the_surface)
{
    Eigen::Matrix<double,3,4> M;
    M <<  0, -1, 0, 1,
          0,  0, 0, 0,
         -1,  0, 1, 0;
    const std::vector<double> v = {-1,0,1,0};

    Mesh mesh(MeshBuilder(M).build());
    mesh.update_intersection_with_free_surface(v);
    const auto p = mesh.coordinates_of_facet(mesh.list_of_facets_immersed.at(0));
    const auto i = mesh.immersions_of_facet(mesh.list_of_facets_immersed.at(0));

    ASSERT_EQ(3, p.cols());

    ASSERT_DOUBLE_EQ(1,(double)p(0,2));
    ASSERT_DOUBLE_EQ(0,(double)p(1,2));
    ASSERT_DOUBLE_EQ(0,(double)p(2,2));

    ASSERT_DOUBLE_EQ(-1,(double)p(0,0));
    ASSERT_DOUBLE_EQ( 0,(double)p(1,0));
    ASSERT_DOUBLE_EQ( 0,(double)p(2,0));

    ASSERT_DOUBLE_EQ(0,(double)p(0,1));
    ASSERT_DOUBLE_EQ(0,(double)p(1,1));
    ASSERT_DOUBLE_EQ(1,(double)p(2,1));

    ASSERT_EQ(3, i.size());
    ASSERT_DOUBLE_EQ(0, i[2]);
    ASSERT_DOUBLE_EQ(0, i[0]);
    ASSERT_DOUBLE_EQ(1, i[1]);
}

TEST_F(hydrostaticTest, bug2_in_immerged_polygon)
{
    Eigen::Matrix<double,3,4> M;
    M <<  -1, 0, 1,  0,
           0, 0, 0,  0,
           0, 1, 0, -1;
    const std::vector<double> v = {0,1,0,-1};

    Mesh mesh(MeshBuilder(M).build());
    mesh.update_intersection_with_free_surface(v);
    const auto p = mesh.coordinates_of_facet(mesh.list_of_facets_immersed.at(0));
    const auto i = mesh.immersions_of_facet(mesh.list_of_facets_immersed.at(0));
    ASSERT_EQ(3, p.cols());

    ASSERT_DOUBLE_EQ(-1,(double)p(0,0));
    ASSERT_DOUBLE_EQ(0,(double)p(1,0));
    ASSERT_DOUBLE_EQ(0,(double)p(2,0));

    ASSERT_DOUBLE_EQ(0,(double)p(0,1));
    ASSERT_DOUBLE_EQ(0,(double)p(1,1));
    ASSERT_DOUBLE_EQ(1,(double)p(2,1));

    ASSERT_DOUBLE_EQ(1,(double)p(0,2));
    ASSERT_DOUBLE_EQ(0,(double)p(1,2));
    ASSERT_DOUBLE_EQ(0,(double)p(2,2));

    ASSERT_EQ(3, i.size());
    ASSERT_DOUBLE_EQ(0, i[0]);
    ASSERT_DOUBLE_EQ(1, i[1]);
    ASSERT_DOUBLE_EQ(0, i[2]);
}

TEST_F(hydrostaticTest, bug3_in_immerged_polygon)
{
    Eigen::Matrix<double,3,3> M;
    M <<  0.5,   0.5,   0.5,
          0.375, 0.5, 0.375,
         -0.25, -0.5,  0.25;
    const std::vector<double> v = {0,-0.25,0.5};

    Mesh mesh(MeshBuilder(M).build());
    mesh.update_intersection_with_free_surface(v);
    const auto p = mesh.coordinates_of_facet(mesh.list_of_facets_immersed.at(0));

    ASSERT_LT(0, (p.col(0)-p.col(1)).norm());
    ASSERT_LT(0, (p.col(0)-p.col(2)).norm());
    ASSERT_LT(0, (p.col(1)-p.col(2)).norm());
}
