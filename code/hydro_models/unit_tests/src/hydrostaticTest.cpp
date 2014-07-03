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
        ASSERT_DOUBLE_EQ(5*h/12., average_immersion(mesh.nodes, mesh.facets.front().index, {0,0,h,h}));
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
    ASSERT_DOUBLE_EQ(7/3., average_immersion(mesh.nodes, mesh.facets.back().index, {1,2,3,4}));
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_for_one_immerged_node)
{
//! [hydrostaticTest immerged_polygon_example_1]
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1; M(0,1) = 4; M(0,2) = 7;
    M(1,0) = 1; M(1,1) = 6; M(1,2) = 8;
    M(2,0) = 3; M(2,1) = 6; M(2,2) = 9;

    std::vector<size_t> index(3);
    index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({-1,-1,1});
    const auto polygon_and_immersions = immerged_polygon(M, index, v);
    const auto polygon = polygon_and_immersions.first;
    ASSERT_EQ(3, polygon.cols());
    ASSERT_DOUBLE_EQ(4,polygon(0,0));
    ASSERT_DOUBLE_EQ(4.5,polygon(1,0));
    ASSERT_DOUBLE_EQ(6,polygon(2,0));
    ASSERT_DOUBLE_EQ(5.5,polygon(0,1));
    ASSERT_DOUBLE_EQ(7,polygon(1,1));
    ASSERT_DOUBLE_EQ(7.5,polygon(2,1));
    ASSERT_DOUBLE_EQ(7,polygon(0,2));
    ASSERT_DOUBLE_EQ(8,polygon(1,2));
    ASSERT_DOUBLE_EQ(9,polygon(2,2));
    ASSERT_EQ(3, polygon_and_immersions.second.size());
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(0));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(1));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(2));
//! [hydrostaticTest immerged_polygon_example_1]
}

TEST_F(hydrostaticTest, can_find_the_next_point)
{
    const std::vector<size_t> idx({1,3,8});
    ASSERT_THROW(next(idx, 2), HydrostaticException);
    ASSERT_EQ(3, next(idx,1));
    ASSERT_EQ(8, next(idx,3));
    ASSERT_EQ(1, next(idx,8));
}

TEST_F(hydrostaticTest, can_find_the_previous_point)
{
    const std::vector<size_t> idx({1,3,8});
    ASSERT_THROW(previous(idx, 2), HydrostaticException);
    ASSERT_EQ(8, previous(idx,1));
    ASSERT_EQ(1, previous(idx,3));
    ASSERT_EQ(3, previous(idx,8));
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_for_two_immerged_node)
{
//! [hydrostaticTest immerged_polygon_example_2]
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1; M(0,1) = 4; M(0,2) = 7;
    M(1,0) = 1; M(1,1) = 6; M(1,2) = 8;
    M(2,0) = 3; M(2,1) = 6; M(2,2) = 9;

    std::vector<size_t> index(3);
    index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({-1,1,1});
    const auto polygon_and_immersions = immerged_polygon(M, index, v);
    const auto polygon = polygon_and_immersions.first;
    ASSERT_EQ(4, polygon.cols());
    ASSERT_DOUBLE_EQ(4,polygon(0,0));
    ASSERT_DOUBLE_EQ(4.5,polygon(1,0));
    ASSERT_DOUBLE_EQ(6,polygon(2,0));
    ASSERT_DOUBLE_EQ(2.5,polygon(0,1));
    ASSERT_DOUBLE_EQ(3.5,polygon(1,1));
    ASSERT_DOUBLE_EQ(4.5,polygon(2,1));
    ASSERT_DOUBLE_EQ(4,polygon(0,2));
    ASSERT_DOUBLE_EQ(6,polygon(1,2));
    ASSERT_DOUBLE_EQ(6,polygon(2,2));
    ASSERT_DOUBLE_EQ(7,polygon(0,3));
    ASSERT_DOUBLE_EQ(8,polygon(1,3));
    ASSERT_DOUBLE_EQ(9,polygon(2,3));
    ASSERT_EQ(4, polygon_and_immersions.second.size());
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(0));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(1));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(2));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(3));
//! [hydrostaticTest immerged_polygon_example_2]
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_when_emerged_nodes_are_the_first_nodes)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    std::vector<size_t> index(5);
    index[0] = 0; index[1] = 1; index[2] = 2; index[3] = 3; index[4] = 4;
    const std::vector<double> v({-1,-1,-1,1,1});
    const auto polygon_and_immersions = immerged_polygon(M, index, v);
    const auto polygon = polygon_and_immersions.first;
    ASSERT_EQ(4, polygon.cols());
    ASSERT_DOUBLE_EQ(5,polygon(0,0));
    ASSERT_DOUBLE_EQ(5,polygon(1,0));
    ASSERT_DOUBLE_EQ(6.5,polygon(2,0));
    ASSERT_DOUBLE_EQ(4.5,polygon(0,1));
    ASSERT_DOUBLE_EQ(5,polygon(1,1));
    ASSERT_DOUBLE_EQ(5.5,polygon(2,1));
    ASSERT_DOUBLE_EQ(5,polygon(0,2));
    ASSERT_DOUBLE_EQ(6,polygon(1,2));
    ASSERT_DOUBLE_EQ(6,polygon(2,2));
    ASSERT_DOUBLE_EQ(9,polygon(0,3));
    ASSERT_DOUBLE_EQ(9,polygon(1,3));
    ASSERT_DOUBLE_EQ(11,polygon(2,3));
    ASSERT_EQ(4, polygon_and_immersions.second.size());
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(0));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(1));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(2));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(3));
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_when_emerged_nodes_are_the_last_nodes)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    std::vector<size_t> index(5);
    index[0] = 0; index[1] = 1; index[2] = 2; index[3] = 3; index[4] = 4;
    const std::vector<double> v({1,1,-1,-1,-1});
    const auto polygon_and_immersions = immerged_polygon(M, index, v);
    const auto polygon = polygon_and_immersions.first;
    ASSERT_EQ(4, polygon.cols());
    ASSERT_DOUBLE_EQ(1,polygon(0,0));
    ASSERT_DOUBLE_EQ(1,polygon(1,0));
    ASSERT_DOUBLE_EQ(2,polygon(2,0));
    ASSERT_DOUBLE_EQ(2,polygon(0,1));
    ASSERT_DOUBLE_EQ(3,polygon(1,1));
    ASSERT_DOUBLE_EQ(3,polygon(2,1));
    ASSERT_DOUBLE_EQ(3,polygon(0,2));
    ASSERT_DOUBLE_EQ(3.5,polygon(1,2));
    ASSERT_DOUBLE_EQ(4,polygon(2,2));
    ASSERT_DOUBLE_EQ(5,polygon(0,3));
    ASSERT_DOUBLE_EQ(5,polygon(1,3));
    ASSERT_DOUBLE_EQ(6.5 ,polygon(2,3));
    ASSERT_EQ(4, polygon_and_immersions.second.size());
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(0));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(1));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(2));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(3));
}

TEST_F(hydrostaticTest, can_compute_immerged_polygon_when_emerged_nodes_are_in_the_middle)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    std::vector<size_t> index(5);
    index[0] = 0; index[1] = 1; index[2] = 2; index[3] = 3; index[4] = 4;
    const std::vector<double> v({1,-1,-1,-1,1});
    const auto polygon_and_immersions = immerged_polygon(M, index, v);
    const auto polygon = polygon_and_immersions.first;
    ASSERT_EQ(4, polygon.cols());
    ASSERT_DOUBLE_EQ(1,polygon(0,0));
    ASSERT_DOUBLE_EQ(1,polygon(1,0));
    ASSERT_DOUBLE_EQ(2,polygon(2,0));
    ASSERT_DOUBLE_EQ(1.5,polygon(0,1));
    ASSERT_DOUBLE_EQ(2,polygon(1,1));
    ASSERT_DOUBLE_EQ(2.5,polygon(2,1));
    ASSERT_DOUBLE_EQ(7,polygon(0,2));
    ASSERT_DOUBLE_EQ(7.5,polygon(1,2));
    ASSERT_DOUBLE_EQ(8.5,polygon(2,2));
    ASSERT_DOUBLE_EQ(9,polygon(0,3));
    ASSERT_DOUBLE_EQ(9,polygon(1,3));
    ASSERT_DOUBLE_EQ(11 ,polygon(2,3));
    ASSERT_EQ(4, polygon_and_immersions.second.size());
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(0));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(1));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(2));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(3));
}

TEST_F(hydrostaticTest, immerged_polygon_should_throw_if_all_points_are_immerged)
{
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1;
    M(1,0) = 2;
    M(2,0) = 3;
    M(0,1) = 4;
    M(1,1) = 5;
    M(2,1) = 6;
    M(0,2) = 7;
    M(1,2) = 8;
    M(2,2) = 9;
    std::vector<size_t> index(3);
    index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({1,2,5});
    ASSERT_THROW(immerged_polygon(M, index, v), HydrostaticException);
}

TEST_F(hydrostaticTest, can_compute_intersection_with_the_surface)
{
    const EPoint A(1,5,9);
    const EPoint B(98,2,0);
    const double dzA = 77;
    const double dzB = -1;
    const EPoint I = intersection(A, dzA, B, dzB);
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
    const EPoint I = intersection(A, dzA, B, dzB);
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
    const EPoint I = intersection(A, dzA, B, dzB);
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
    const EPoint I = intersection(A, dzA, B, dzB);
    ASSERT_DOUBLE_EQ(24, I(0));
    ASSERT_DOUBLE_EQ(-1, I(1));
    ASSERT_DOUBLE_EQ(5, I(2));
}

TEST_F(hydrostaticTest, immerged_polygon_should_throw_if_no_points_are_immerged)
{
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1;
    M(1,0) = 2;
    M(2,0) = 3;
    M(0,1) = 4;
    M(1,1) = 5;
    M(2,1) = 6;
    M(0,2) = 7;
    M(1,2) = 8;
    M(2,2) = 9;
    std::vector<size_t> index(3);
    index[0] = 0; index[1] = 1; index[2] = 2;
    const std::vector<double> v({-1,-2,-5});
    ASSERT_THROW(immerged_polygon(M, index, v), HydrostaticException);
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

TEST_F(hydrostaticTest, bug_in_immerged_polygon)
{
    const Mesh mesh = MeshBuilder(two_triangles()).build();
    const std::vector<double> z = {-1,-1,-2,1};
    ASSERT_NO_THROW(immerged_polygon(mesh.nodes, mesh.facets.at(1).index, z));
}

TEST_F(hydrostaticTest, another_bug_in_immerged_polygon)
{
    const Mesh mesh = MeshBuilder(two_triangles()).build();
    const std::vector<double> z = {-1,-1,-2,1};
    const auto polygon_and_immersions = immerged_polygon(mesh.nodes, mesh.facets.at(1).index, z);
    const auto polygon = polygon_and_immersions.first;

    ASSERT_DOUBLE_EQ(1, polygon(0,0));
    ASSERT_DOUBLE_EQ(-0.5, polygon(1,0));
    ASSERT_DOUBLE_EQ(0, polygon(2,0));

    ASSERT_DOUBLE_EQ(2, polygon(0,1));
    ASSERT_DOUBLE_EQ(-1, polygon(1,1));
    ASSERT_DOUBLE_EQ(0, polygon(2,1));

    ASSERT_DOUBLE_EQ(3, polygon(0,2));
    ASSERT_DOUBLE_EQ(-0.5, polygon(1,2));
    ASSERT_DOUBLE_EQ(0, polygon(2,2));

    ASSERT_EQ(3, polygon_and_immersions.second.size());
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(0));
    ASSERT_DOUBLE_EQ(1,polygon_and_immersions.second.at(1));
    ASSERT_DOUBLE_EQ(0,polygon_and_immersions.second.at(2));
}

TEST_F(hydrostaticTest, bug_in_first_and_last_emerged_points)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,1,-1});
    ASSERT_EQ(2,first_and_last.first);
    ASSERT_EQ(0,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T01)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,-1,-1,1,1,1,-1,-1,-1});
    ASSERT_EQ(6,first_and_last.first);
    ASSERT_EQ(2,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T02)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({1,1,1,-1,-1,1,1});
    ASSERT_EQ(3,first_and_last.first);
    ASSERT_EQ(4,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T03)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,-1,-1,1,1,1});
    ASSERT_EQ(0,first_and_last.first);
    ASSERT_EQ(2,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T04)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({1,1,1,-1,-1,-1});
    ASSERT_EQ(3,first_and_last.first);
    ASSERT_EQ(5,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T05)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,-1,-1,1});
    ASSERT_EQ(0,first_and_last.first);
    ASSERT_EQ(2,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T06)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,-1,1,-1});
    ASSERT_EQ(3,first_and_last.first);
    ASSERT_EQ(1,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T07)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,1,-1,-1});
    ASSERT_EQ(2,first_and_last.first);
    ASSERT_EQ(0,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T08)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({-1,1,1,1});
    ASSERT_EQ(0,first_and_last.first);
    ASSERT_EQ(0,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T09)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({1,1,1,-1});
    ASSERT_EQ(3,first_and_last.first);
    ASSERT_EQ(3,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_T10)
{
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points({1,1,-1,1});
    ASSERT_EQ(2,first_and_last.first);
    ASSERT_EQ(2,first_and_last.second);
}

TEST_F(hydrostaticTest, first_and_last_emerged_points_should_throw_if_set_of_emerged_points_is_not_convex)
{
    ASSERT_THROW(first_and_last_emerged_points({1,1,-1,-1,1,-1,-1}), HydrostaticException);
    ASSERT_THROW(first_and_last_emerged_points({1,1,-1,-1,1,-1,-1,1}), HydrostaticException);
    ASSERT_THROW(first_and_last_emerged_points({-1,-1,1,-1,-1,1}), HydrostaticException);
    ASSERT_THROW(first_and_last_emerged_points({1,-1,-1,1,-1,-1}), HydrostaticException);
}

TEST_F(hydrostaticTest, can_compute_the_hydrostatic_force_on_two_triangles)
{
    const MeshPtr mesh(new Mesh(MeshBuilder(two_triangles()).build()));
    const double rho = 1024;
    const double g = 10;
    const Point G(a.random<std::string>(), 1,2,4);
    const std::vector<double> z = {-0.5,-0.5,-2.5,0.5};
    const Wrench Fhs = force(mesh, G, rho, g, z);
    ASSERT_DOUBLE_EQ(0, Fhs.X());
    ASSERT_DOUBLE_EQ(0, Fhs.Y());
    const double dz = 0.5/3;
    const double dS = 0.5;
    ASSERT_DOUBLE_EQ(0, Fhs.X());
    ASSERT_DOUBLE_EQ(0, Fhs.Y());
    ASSERT_DOUBLE_EQ(-rho*g*dz*dS, Fhs.Z());
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
        const double g = 9.81;
        const Wrench Fhs = force(mesh, G, rho, g, dz);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.X(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.Y(), EPS);
        const double V = L*L*L;
        ASSERT_SMALL_RELATIVE_ERROR(-rho*g*V, Fhs.Z(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(-rho*g*V*y0, Fhs.K(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(+rho*g*V*x0, Fhs.M(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.N(), EPS);
    }
}

TEST_F(hydrostaticTest, DISABLED_can_compute_the_hydrostatic_force_on_a_rotated_cube)
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
        const double g = 9.81;
        const Wrench Fhs = force(mesh, G, rho, g, dz);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.X(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.Y(), EPS);
        const double V = L*L*L;
        ASSERT_SMALL_RELATIVE_ERROR(-0.5*rho*g*V, Fhs.Z(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(-0.5*rho*g*V*y0, Fhs.K(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(+0.5*rho*g*V*x0, Fhs.M(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.N(), EPS);
    }
}

TEST_F(hydrostaticTest, can_compute_the_hydrostatic_force_on_a_stl_cube)
{
    const VectorOfVectorOfPoints mesh_cube(read_stl(test_data::cube()));
    const MeshPtr mesh(new Mesh(MeshBuilder(mesh_cube).build()));
    const std::vector<double> dz = {-1,-1,-1,-1,0,0,0,0};
    const Point G(a.random<std::string>(), 0, 0, 0);
    const double rho = 1000;
    const double g = 9.81;
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
        const double g = 9.81;
        const Wrench Fhs = force(mesh, G, rho, g, dz);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.X(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(0, Fhs.Y(), EPS);
        const double V = L*L*L/(6.*sqrt(2));
        ASSERT_SMALL_RELATIVE_ERROR(-rho*g*V, Fhs.Z(), EPS);
    }
}

TEST_F(hydrostaticTest, bug_discovered_when_implementing_sum_of_forces)
{
    Eigen::Matrix<double,3,4> M;
    M(0,0) = 1; M(0,1) = -1.879706489;M(0,2) = -1.879706489;M(0,3) = 6.759412979;
    M(1,0) = 2; M(1,1) = -2.98779795; M(1,2) = 6.98779795;  M(1,3) = 2;
    M(2,0) = 3; M(2,1) = 11.14503995; M(2,2) = 11.14503995; M(2,3) = 11.14503995;

    const std::vector<double> v = {-204294e4,953171e4,38521e5,-990113e4};
    const std::vector<size_t> idx = {2,1,3};

    immerged_polygon(M, idx, v);
    ASSERT_NO_THROW(immerged_polygon(M, idx, v));

    const double L = a.random<double>().between(0,10);
    const double x0 = 1;
    const double y0 = 2;
    const double z0 = 3;
    const Point G(a.random<std::string>(), -3.63075e+09, -7.71511e+09,  9.07162e+09);
    const MeshPtr mesh(new Mesh(MeshBuilder(tetrahedron(L,x0,y0,z0)).build()));
    const std::vector<double> dz = {-2.04294e+09, 9.53171e+09, 3.8521e+09, -9.90113e+09};
    force(mesh, G, a.random<double>(), a.random<double>(), dz);
    ASSERT_NO_THROW(force(mesh, G, a.random<double>(), a.random<double>(), dz));
}

TEST_F(hydrostaticTest, hydrostatic_force_should_be_computed_at_the_right_point)
{
    const double L = a.random<double>().between(0,10);
    const double x0 = 1;
    const double y0 = 2;
    const double z0 = 3;
    const Point G(a.random<std::string>(), a.random<double>(), a.random<double>(), a.random<double>());
    const MeshPtr mesh(new Mesh(MeshBuilder(tetrahedron(L,x0,y0,z0)).build()));
    const std::vector<double> dz = a.random_vector_of<double>().of_size(4);
    const Wrench Fhs = force(mesh, G, a.random<double>(), a.random<double>(), dz);

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
    const EPoint P = intersection(A,0,B,-1);
    const EPoint Q = intersection(C,0,D,-1);

    ASSERT_DOUBLE_EQ((double)A(0), (double)P(0));
    ASSERT_DOUBLE_EQ((double)A(1), (double)P(1));
    ASSERT_DOUBLE_EQ((double)A(2), (double)P(2));

    ASSERT_DOUBLE_EQ((double)Q(0), (double)C(0));
    ASSERT_DOUBLE_EQ((double)Q(1), (double)C(1));
    ASSERT_DOUBLE_EQ((double)Q(2), (double)C(2));
}

TEST_F(hydrostaticTest, immerged_polygon_should_not_throw_an_exception_if_two_points_are_exactly_on_the_surface)
{
    Eigen::Matrix<double,3,4> M;
    M <<  0,-1,0,1,
          0, 0,0,0,
         -1, 0,1,0;
    const std::vector<size_t>idx = {0,1,2,3};
    const std::vector<double> v = {-1,0,1,0};
    immerged_polygon(M,idx,v);
    ASSERT_NO_THROW(immerged_polygon(M,idx,v));
}

TEST_F(hydrostaticTest, correct_immerged_polygon_when_two_points_are_exactly_on_the_surface)
{
    Eigen::Matrix<double,3,4> M;
    M <<  0,-1,0,1,
          0, 0,0,0,
         -1, 0,1,0;
    const std::vector<size_t>idx = {0,1,2,3};
    const std::vector<double> v = {-1,0,1,0};
    const std::pair<Matrix3x,std::vector<double> > P = immerged_polygon(M,idx,v);

    const auto p = P.first;

    ASSERT_EQ(3, p.cols());

    ASSERT_DOUBLE_EQ(1,(double)p(0,0));
    ASSERT_DOUBLE_EQ(0,(double)p(1,0));
    ASSERT_DOUBLE_EQ(0,(double)p(2,0));

    ASSERT_DOUBLE_EQ(-1,(double)p(0,1));
    ASSERT_DOUBLE_EQ(0,(double)p(1,1));
    ASSERT_DOUBLE_EQ(0,(double)p(2,1));

    ASSERT_DOUBLE_EQ(0,(double)p(0,2));
    ASSERT_DOUBLE_EQ(0,(double)p(1,2));
    ASSERT_DOUBLE_EQ(1,(double)p(2,2));

    ASSERT_EQ(3, P.second.size());
    ASSERT_DOUBLE_EQ(0, P.second[0]);
    ASSERT_DOUBLE_EQ(0, P.second[1]);
    ASSERT_DOUBLE_EQ(1, P.second[2]);
}
