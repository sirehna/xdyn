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
#include "MeshIntersector.hpp"
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


TEST_F(hydrostaticTest, can_compute_the_hydrostatic_force_on_two_triangles)
{
    const MeshPtr mesh(new Mesh(MeshBuilder(two_triangles()).build()));
    const double rho = 1024;
    const EPoint g(0, 0, 10);
    const Point G(a.random<std::string>(), 1,2,4);
    const std::vector<double> z = {-0.5,-0.5,-2.5,0.5};
    MeshIntersector intersector(mesh,z);
    intersector.update_intersection_with_free_surface();
    const Wrench Fhs = force(const_MeshIntersectorPtr(new MeshIntersector(intersector)), G, rho, g);

    ASSERT_EQ(1,intersector.index_of_immersed_facets.size());
    ASSERT_DOUBLE_EQ(0.5,::area(intersector.coordinates_of_facet(intersector.index_of_immersed_facets.at(0))));
    ASSERT_DOUBLE_EQ(0.5,mesh->facets[intersector.index_of_immersed_facets.at(0)].area);
    ASSERT_DOUBLE_EQ(0.5/3,hydrostatic::average_immersion(mesh->all_nodes, mesh->facets[intersector.index_of_immersed_facets.at(0)].vertex_index, intersector.all_immersions));

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
        MeshIntersector intersector(mesh,dz);
        intersector.update_intersection_with_free_surface();
        const Wrench Fhs = force(const_MeshIntersectorPtr(new MeshIntersector(intersector)), G, rho, g);
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
        MeshIntersector intersector(mesh,dz);
        intersector.update_intersection_with_free_surface();
        force(const_MeshIntersectorPtr(new MeshIntersector(intersector)), G, rho, g);
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
    MeshIntersector intersector(mesh,dz);
    intersector.update_intersection_with_free_surface();
    const Wrench Fhs = force(const_MeshIntersectorPtr(new MeshIntersector(intersector)), G, rho, g);
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
        MeshIntersector intersector(mesh,dz);
        intersector.update_intersection_with_free_surface();
        const Wrench Fhs = force(const_MeshIntersectorPtr(new MeshIntersector(intersector)), G, rho, g);
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
    MeshIntersector intersector(mesh,dz);
    intersector.update_intersection_with_free_surface();
    const Wrench Fhs = force(const_MeshIntersectorPtr(new MeshIntersector(intersector)), G, a.random<double>(), a.random<EPoint>());

    ASSERT_EQ(G.get_frame(), Fhs.get_point().get_frame());
    ASSERT_EQ(G.get_frame(), Fhs.get_frame());
    ASSERT_DOUBLE_EQ(G.x(), Fhs.get_point().x());
    ASSERT_DOUBLE_EQ(G.y(), Fhs.get_point().y());
    ASSERT_DOUBLE_EQ(G.z(), Fhs.get_point().z());
}

