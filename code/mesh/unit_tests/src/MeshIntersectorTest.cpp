/*
 * MeshIntersectorTest.cpp
 */

#include <ssc/macros.hpp>
#include <ssc/kinematics.hpp>

#include "TriMeshTestData.hpp"
#include "MeshBuilder.hpp"
#include "MeshIntersectorTest.hpp"
#include "mesh_manipulations.hpp"
#include "stl_data.hpp"
#include "stl_reader.hpp"
#include "MeshIntersector.hpp"
#include "generate_test_ship.hpp"

#define EPS 1E-6

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

std::vector<double> get_cube_immersions(const double z0);
std::vector<double> get_cube_immersions(const double z0)
{
    return std::vector<double>({z0+0.5,z0+0.5,z0+0.5,z0+0.5,z0-0.5,z0-0.5,z0-0.5,z0-0.5});
}

std::vector<double> get_U_immersions(const double z0);
std::vector<double> get_U_immersions(const double z0)
{
    return std::vector<double>({z0, z0, z0-1,  z0-1, z0-2, z0-2,  z0, z0-1, z0,  z0-1, z0-2, z0-2,  z0,  z0, z0-1, z0-1,  z0-2, z0-2, z0,  z0-1,z0, z0-1, z0-2, z0-2});
}

void check_vector(const Eigen::Vector3d& v_to_check, const double x_expected, const double y_expected, const double z_expected);
void check_vector(const Eigen::Vector3d& v_to_check, const double x_expected, const double y_expected, const double z_expected)
{
    ASSERT_DOUBLE_EQ(x_expected, (double)v_to_check(0)) << "Expected: " << Eigen::Vector3d(x_expected, y_expected, z_expected).transpose() << ", actual: " << v_to_check.transpose();
    ASSERT_DOUBLE_EQ(y_expected, (double)v_to_check(1)) << "Expected: " << Eigen::Vector3d(x_expected, y_expected, z_expected).transpose() << ", actual: " << v_to_check.transpose();;
    ASSERT_DOUBLE_EQ(z_expected, (double)v_to_check(2)) << "Expected: " << Eigen::Vector3d(x_expected, y_expected, z_expected).transpose() << ", actual: " << v_to_check.transpose();;
}

MeshIntersectorTest::MeshIntersectorTest() : a(ssc::random_data_generator::DataGenerator(2))
{
}

MeshIntersectorTest::~MeshIntersectorTest()
{
}

void MeshIntersectorTest::SetUp()
{
}

void MeshIntersectorTest::TearDown()
{
}

TEST_F(MeshIntersectorTest, can_compute_immersed_polygon_for_one_immersed_node)
{
//! [MeshIntersectorTest immersed_polygon_example_1]
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1; M(0,1) = 4; M(0,2) = 7;
    M(1,0) = 1; M(1,1) = 6; M(1,2) = 8;
    M(2,0) = 3; M(2,1) = 6; M(2,2) = 9;

    const std::vector<double> v({-1,-1,1});

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);

    const auto polygon    = intersector.coordinates_of_facet(intersector.index_of_immersed_facets.at(0));
    const auto immersions = intersector.immersions_of_facet(intersector.index_of_immersed_facets.at(0));

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


TEST_F(MeshIntersectorTest, can_compute_immersed_polygon_for_two_immersed_nodes)
{
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1; M(0,1) = 4; M(0,2) = 7;
    M(1,0) = 1; M(1,1) = 6; M(1,2) = 8;
    M(2,0) = 3; M(2,1) = 6; M(2,2) = 9;

    const std::vector<double> v({-1,1,1});

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);

    const auto polygon    = intersector.coordinates_of_facet(intersector.index_of_immersed_facets.at(0));
    const auto immersions = intersector.immersions_of_facet(intersector.index_of_immersed_facets.at(0));

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

TEST_F(MeshIntersectorTest, can_compute_immersed_polygon_when_emerged_nodes_are_the_first_nodes)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    const std::vector<double> v({-1,-1,-1,1,1});

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);

    const auto polygon    = intersector.coordinates_of_facet(intersector.index_of_immersed_facets.at(0));
    const auto immersions = intersector.immersions_of_facet(intersector.index_of_immersed_facets.at(0));

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

TEST_F(MeshIntersectorTest, can_compute_immersed_polygon_when_emerged_nodes_are_the_last_nodes)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    const std::vector<double> v({1,1,-1,-1,-1});

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);

    const auto polygon    = intersector.coordinates_of_facet(intersector.index_of_immersed_facets.at(0));
    const auto immersions = intersector.immersions_of_facet(intersector.index_of_immersed_facets.at(0));

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

TEST_F(MeshIntersectorTest, can_compute_immersed_polygon_when_emerged_nodes_are_in_the_middle)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    const std::vector<double> v({1,-1,-1,-1,1});

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);

    const auto polygon    = intersector.coordinates_of_facet(intersector.index_of_immersed_facets.at(0));
    const auto immersions = intersector.immersions_of_facet(intersector.index_of_immersed_facets.at(0));

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

TEST_F(MeshIntersectorTest, immersed_polygon_should_throw_if_all_points_are_immersed)
{
    Eigen::Matrix<double,3,3> M;
    M <<
        1 , 4 , 7 ,
        2 , 5 , 8 ,
        3 , 6 , 9 ;
    std::vector<size_t> index(3);
    const std::vector<double> v({1,2,5});

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);

    ASSERT_EQ(0,intersector.index_of_emerged_facets.size());
    ASSERT_EQ(0,intersector.index_of_immersed_facets.at(0));
}

TEST_F(MeshIntersectorTest, can_compute_intersection_with_the_surface)
{
    const EPoint A(1,5,9);
    const EPoint B(98,2,0);
    const double dzA = 77;
    const double dzB = -1;
    const EPoint I = MeshIntersector::edge_intersection(A, dzA, B, dzB);
    ASSERT_NEAR(77./78.*97+1, (double)I(0),EPS*EPS);
    ASSERT_NEAR(5-77./78.*3, (double)I(1),EPS*EPS);
    ASSERT_NEAR(9-77./78.*9, (double)I(2),EPS*EPS);
}

TEST_F(MeshIntersectorTest, can_compute_intersection_with_the_surface_second_test)
{
    const EPoint A(-98,-2,9);
    const EPoint B(98,2,-9);
    const double dzA = 9;
    const double dzB = -9;
    const EPoint I = MeshIntersector::edge_intersection(A, dzA, B, dzB);
    ASSERT_DOUBLE_EQ(0, I(0));
    ASSERT_DOUBLE_EQ(0, I(1));
    ASSERT_DOUBLE_EQ(0, I(2));
}

TEST_F(MeshIntersectorTest, can_compute_intersection_with_the_surface_third_test)
{
    const EPoint A(-1,-2,10);
    const EPoint B(99,2,-10);
    const double dzA = 1;
    const double dzB = -3;
    const EPoint I = MeshIntersector::edge_intersection(A, dzA, B, dzB);
    ASSERT_DOUBLE_EQ(24, I(0));
    ASSERT_DOUBLE_EQ(-1, I(1));
    ASSERT_DOUBLE_EQ(5, I(2));
}

TEST_F(MeshIntersectorTest, can_compute_intersection_with_the_surface_bug_detected_in_integration_test)
{
    const EPoint A(-1,-2,10);
    const EPoint B(99,2,-10);
    const double dzA = 1;
    const double dzB = -3;
    const EPoint I = MeshIntersector::edge_intersection(A, dzA, B, dzB);
    ASSERT_DOUBLE_EQ(24, I(0));
    ASSERT_DOUBLE_EQ(-1, I(1));
    ASSERT_DOUBLE_EQ(5, I(2));
}

TEST_F(MeshIntersectorTest, immersed_polygon_should_throw_if_no_points_are_immersed)
{
    Eigen::Matrix<double,3,3> M;
    M <<
        1 , 4 , 7 ,
        2 , 5 , 8 ,
        3 , 6 , 9 ;
    const std::vector<double> v({-1,-2,-5});

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);

    ASSERT_EQ(0,intersector.index_of_immersed_facets.size());
    ASSERT_EQ(0,intersector.index_of_emerged_facets.at(0));
}

TEST_F(MeshIntersectorTest, another_bug_in_immersed_polygon)
{
    MeshPtr mesh(new Mesh(MeshBuilder(two_triangles()).build()));
    const std::vector<double> v = {-1,-1,-2,1};

    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);

    const auto polygon = intersector.coordinates_of_facet(intersector.index_of_immersed_facets.at(0));
    const auto immersions = intersector.immersions_of_facet(intersector.index_of_immersed_facets.at(0));

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


TEST_F(MeshIntersectorTest, should_be_able_to_compute_intersection_even_if_one_of_the_points_is_exactly_on_the_surface)
{
    const EPoint A(1,2,3);
    const EPoint B(4,5,6);
    const EPoint C(7,8,9);
    const EPoint D(11,12,13);
    const EPoint P = MeshIntersector::edge_intersection(A,0,B,-1);
    const EPoint Q = MeshIntersector::edge_intersection(C,0,D,-1);

    ASSERT_DOUBLE_EQ((double)A(0), (double)P(0));
    ASSERT_DOUBLE_EQ((double)A(1), (double)P(1));
    ASSERT_DOUBLE_EQ((double)A(2), (double)P(2));

    ASSERT_DOUBLE_EQ((double)Q(0), (double)C(0));
    ASSERT_DOUBLE_EQ((double)Q(1), (double)C(1));
    ASSERT_DOUBLE_EQ((double)Q(2), (double)C(2));
}

TEST_F(MeshIntersectorTest, correct_immersed_polygon_when_two_points_are_exactly_on_the_surface)
{
    Eigen::Matrix<double,3,4> M;
    M <<  0, -1, 0, 1,
          0,  0, 0, 0,
         -1,  0, 1, 0;
    const std::vector<double> v = {-1,0,1,0};

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);
    const auto p = intersector.coordinates_of_facet(intersector.index_of_immersed_facets.at(0));
    const auto i = intersector.immersions_of_facet(intersector.index_of_immersed_facets.at(0));

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

TEST_F(MeshIntersectorTest, bug2_in_immersed_polygon)
{
    Eigen::Matrix<double,3,4> M;
    M <<  -1, 0, 1,  0,
           0, 0, 0,  0,
           0, 1, 0, -1;
    const std::vector<double> v = {0,1,0,-1};

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);
    const auto p = intersector.coordinates_of_facet(intersector.index_of_immersed_facets.at(0));
    const auto i = intersector.immersions_of_facet(intersector.index_of_immersed_facets.at(0));
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

TEST_F(MeshIntersectorTest, bug3_in_immersed_polygon)
{
    Eigen::Matrix<double,3,3> M;
    M <<  0.5,   0.5,   0.5,
          0.375, 0.5, 0.375,
         -0.25, -0.5,  0.25;
    const std::vector<double> v = {0,-0.25,0.5};

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v,v);
    const auto p = intersector.coordinates_of_facet(intersector.index_of_immersed_facets.at(0));

    ASSERT_LT(0, (p.col(0)-p.col(1)).norm());
    ASSERT_LT(0, (p.col(0)-p.col(2)).norm());
    ASSERT_LT(0, (p.col(1)-p.col(2)).norm());
}


TEST_F(MeshIntersectorTest, can_compute_the_volume_of_a_single_facet)
{
    MeshIntersector intersector(VectorOfVectorOfPoints(1, one_triangle()));
    const std::vector<double> v = {0,-0.25,0.5};
    intersector.update_intersection_with_free_surface(v,v);
    ASSERT_DOUBLE_EQ(0, intersector.immersed_volume());
    ASSERT_DOUBLE_EQ(0, intersector.emerged_volume());
}

TEST_F(MeshIntersectorTest, can_compute_the_volume_of_a_tetrahedron)
{
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double l = a.random<double>().between(0,1000);
        const double z = a.random<double>().between(-100,100);
        const std::vector<double> v = {z,z+l*sqrt(6)/6,z+l*sqrt(6)/6,z+l*sqrt(6)/6};
        MeshIntersector intersector(tetrahedron(l,a.random<double>().between(-100,100),a.random<double>().between(-100,100),z));
        intersector.update_intersection_with_free_surface(v,v);
        ASSERT_NEAR(l*l*l/6./sqrt(2.), intersector.immersed_volume() + intersector.emerged_volume(), 1E-2) << "z = " << z << ", l = " << l;
    }
}

TEST_F(MeshIntersectorTest, can_compute_the_volume_of_a_cube)
{
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double l = a.random<double>().between(0, 10000);
        const double z = a.random<double>().between(-100,100);
        MeshIntersector intersector(cube(l, a.random<double>().between(-100,100), a.random<double>().between(-100,100), z));
        const std::vector<double> dz = {z-l/2,z-l/2,z-l/2,z-l/2,z+l/2,z+l/2,z+l/2,z+l/2};
        intersector.update_intersection_with_free_surface(dz,dz);
        ASSERT_SMALL_RELATIVE_ERROR(l*l*l, intersector.immersed_volume() + intersector.emerged_volume(), EPS);
    }
}

TEST_F(MeshIntersectorTest, can_compute_the_volume_of_a_cube_with_lots_of_irregular_facets)
{
    MeshIntersector intersector(read_stl(test_data::big_cube()));
    const std::vector<double> dz(intersector.mesh->total_number_of_nodes,1);
    intersector.update_intersection_with_free_surface(dz,dz);
    ASSERT_SMALL_RELATIVE_ERROR(1, intersector.immersed_volume() + intersector.emerged_volume(), EPS);
}

TEST_F(MeshIntersectorTest, can_compute_the_volume_of_an_immersed_cube)
{
    for (size_t i = 0 ; i < 10 ; ++i)
    {
        const double l = a.random<double>().between(0, 10000);
        std::vector<double> all_immersed(8);
        all_immersed[0] = 1;
        all_immersed[1] = 1;
        all_immersed[2] = 1;
        all_immersed[3] = 1;
        all_immersed[4] = 1+l;
        all_immersed[5] = 1+l;
        all_immersed[6] = 1+l;
        all_immersed[7] = 1+l;
        MeshIntersector intersector(cube(l, a.random<double>().between(-100,100), a.random<double>().between(-100,100), a.random<double>().between(-100,100)));

        intersector.update_intersection_with_free_surface(all_immersed,all_immersed);
        ASSERT_DOUBLE_EQ(0, intersector.emerged_volume());
        ASSERT_SMALL_RELATIVE_ERROR(l*l*l, intersector.immersed_volume(), EPS);
    }
}

TEST_F(MeshIntersectorTest, can_compute_the_volume_of_an_emerged_cube)
{
    for (size_t i = 0 ; i < 10 ; ++i)
    {
        const double l = a.random<double>().between(0, 10000);
        std::vector<double> all_emerged(8);
        all_emerged[0] = -1;
        all_emerged[1] = -1;
        all_emerged[2] = -1;
        all_emerged[3] = -1;
        all_emerged[4] = -1-l;
        all_emerged[5] = -1-l;
        all_emerged[6] = -1-l;
        all_emerged[7] = -1-l;
        MeshIntersector intersector(cube(l, a.random<double>().between(-100,100), a.random<double>().between(-100,100), a.random<double>().between(-100,100)));

        intersector.update_intersection_with_free_surface(all_emerged,all_emerged);
        ASSERT_SMALL_RELATIVE_ERROR(l*l*l, intersector.emerged_volume(), EPS);
        ASSERT_DOUBLE_EQ(0, intersector.immersed_volume());
    }
}

TEST_F(MeshIntersectorTest, can_detect_if_facet_is_already_included)
{
    const MeshPtr mesh(new Mesh(MeshBuilder(cube(1, 0, 0, 0, false)).build()));
    MeshIntersector intersector(mesh);
    std::vector<double> dz(8);
    dz[0] = 0;
    dz[1] = 0;
    dz[2] = 0;
    dz[3] = 0;
    dz[4] = 1;
    dz[5] = 1;
    dz[6] = 1;
    dz[7] = 1;
    intersector.update_intersection_with_free_surface(dz,dz);
    Facet f;
    f.vertex_index.push_back(0);
    f.vertex_index.push_back(1);
    f.vertex_index.push_back(2);
    f.vertex_index.push_back(3);
    f.unit_normal(0) = 0;
    f.unit_normal(1) = 0;
    f.unit_normal(2) = -1;
    ASSERT_TRUE(intersector.has(f));
    f.vertex_index.back() = 4;
    ASSERT_FALSE(intersector.has(f));
}

TEST_F(MeshIntersectorTest, can_compute_the_volume_of_a_partially_immersed_cube)
{
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double l = a.random<double>().between(0, 10000);
        const double immersed_ratio = a.random<double>().between(0,1);
        std::vector<double> half_immersed(8);
        const double z0 = l*(immersed_ratio-0.5);
        half_immersed[0] = z0-l/2;
        half_immersed[1] = z0-l/2;
        half_immersed[2] = z0-l/2;
        half_immersed[3] = z0-l/2;
        half_immersed[4] = z0+l/2;
        half_immersed[5] = z0+l/2;
        half_immersed[6] = z0+l/2;
        half_immersed[7] = z0+l/2;

        const double x0 = a.random<double>().between(-100,100);
        const double y0 = a.random<double>().between(-100,100);

        MeshIntersector intersector(cube(l, x0, y0, z0));

        intersector.update_intersection_with_free_surface(half_immersed,half_immersed);
        ASSERT_SMALL_RELATIVE_ERROR(l*l*l, intersector.immersed_volume()+intersector.emerged_volume(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(l*l*l*(1-immersed_ratio), intersector.emerged_volume(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(l*l*l*immersed_ratio, intersector.immersed_volume(), EPS);
    }
}

TEST_F(MeshIntersectorTest, LONG_can_compute_the_volume_of_the_anthineas)
{
    MeshIntersector intersector(test_ship());
    const std::vector<double> dz(intersector.mesh->total_number_of_nodes,1);
    intersector.update_intersection_with_free_surface(dz,dz);
    ASSERT_NEAR(601.726, intersector.emerged_volume()+intersector.immersed_volume(), 1E-3);
}

TEST_F(MeshIntersectorTest, can_compute_the_barycenter_of_a_partially_immersed_cube)
{
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double l = a.random<double>().between(0, 10);
        const double immersed_ratio = a.random<double>().between(0,1);
        std::vector<double> half_immersed(8);
        const double z0 = l*(immersed_ratio-0.5);
        half_immersed[0] = z0-l/2;
        half_immersed[1] = z0-l/2;
        half_immersed[2] = z0-l/2;
        half_immersed[3] = z0-l/2;
        half_immersed[4] = z0+l/2;
        half_immersed[5] = z0+l/2;
        half_immersed[6] = z0+l/2;
        half_immersed[7] = z0+l/2;

        const double x0 = a.random<double>().between(-100,100);
        const double y0 = a.random<double>().between(-100,100);

        MeshIntersector intersector(cube(l, x0, y0, z0));

        intersector.update_intersection_with_free_surface(half_immersed,half_immersed);

        const EPoint G_emerged = intersector.center_of_mass_emerged().G;
        const EPoint G_immersed = intersector.center_of_mass_immersed().G;

        ASSERT_NEAR(x0, (double)G_emerged(0),EPS);
        ASSERT_NEAR(y0, (double)G_emerged(1),EPS);
        ASSERT_NEAR(l*(immersed_ratio-1)/2, (double)G_emerged(2),EPS);
        ASSERT_NEAR(x0, (double)G_immersed(0),EPS);
        ASSERT_NEAR(y0, (double)G_immersed(1),EPS);
        ASSERT_NEAR(l*immersed_ratio/2, (double)G_immersed(2),EPS);
    }
}

TEST_F(MeshIntersectorTest, bug_2715_emerged_volume_for_cube_just_beneath_the_surface)
{
    MeshIntersector intersector(unit_cube());
    const auto dz = get_cube_immersions(0.5);
    intersector.update_intersection_with_free_surface(dz,dz);
    ASSERT_DOUBLE_EQ(0, intersector.emerged_volume());
}

TEST_F(MeshIntersectorTest, bug_2715_immersed_volume_for_cube_just_beneath_the_surface)
{
    MeshIntersector intersector(unit_cube());
    const auto dz = get_cube_immersions(0.5);
    intersector.update_intersection_with_free_surface(dz,dz);
    ASSERT_DOUBLE_EQ(1, intersector.immersed_volume());
}

TEST_F(MeshIntersectorTest, bug_2715_emerged_volume_for_cube_just_above_the_surface)
{
    MeshIntersector intersector(unit_cube());
    const auto dz = get_cube_immersions(-0.5);
    intersector.update_intersection_with_free_surface(dz,dz);
    ASSERT_DOUBLE_EQ(1, intersector.emerged_volume());
}

TEST_F(MeshIntersectorTest, bug_2715_immersed_volume_for_cube_just_above_the_surface)
{
    MeshIntersector intersector(unit_cube());
    const auto dz = get_cube_immersions(-0.5);
    intersector.update_intersection_with_free_surface(dz,dz);
    ASSERT_DOUBLE_EQ(0, intersector.immersed_volume());
}

TEST_F(MeshIntersectorTest, bug_in_facet_volume)
{
    MeshIntersector intersector(cube(1, 0, 0, 0));
    const std::vector<double> dz(8, 2);
    intersector.update_intersection_with_free_surface(dz,dz);
    ASSERT_DOUBLE_EQ(1./6.,intersector.facet_volume(*(intersector.begin_immersed())));
}

TEST_F(MeshIntersectorTest, bug_detected_in_potential_energy)
{
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double l = a.random<double>().between(0, 10);
        const double immersed_ratio = a.random<double>().between(0,1);
        std::vector<double> half_immersed(8);
        const double z0 = l*(immersed_ratio-0.5);
        half_immersed[0] = z0-l/2;
        half_immersed[1] = z0-l/2;
        half_immersed[2] = z0-l/2;
        half_immersed[3] = z0-l/2;
        half_immersed[4] = z0+l/2;
        half_immersed[5] = z0+l/2;
        half_immersed[6] = z0+l/2;
        half_immersed[7] = z0+l/2;

        const double x0 = a.random<double>().between(-100,100);
        const double y0 = a.random<double>().between(-100,100);

        MeshIntersector intersector(cube(l, x0, y0, z0));

        intersector.update_intersection_with_free_surface(half_immersed,half_immersed);

        const EPoint G_immersed = intersector.center_of_mass_immersed().G;

        ASSERT_NEAR(l*immersed_ratio/2, (double)G_immersed(2),EPS) << "l = " << l << ", immersed_ratio = " << immersed_ratio << ", z0 = " << z0;
    }
}

std::vector<double> get_L_immersions(const double z0);
std::vector<double> get_L_immersions(const double z0)
{
    return {z0,z0-1,z0-1,z0,z0-2,z0-2,z0-1,z0,z0-2,z0-2,z0-1,z0-1,z0,z0,z0-1,z0};
}

TEST_F(MeshIntersectorTest, bug_in_centroid)
{
    MeshIntersector intersector(L(),false);
    std::vector<double> dz = get_L_immersions(0);
    intersector.update_intersection_with_free_surface(dz,dz);
    auto C = intersector.center_of_mass_immersed();
    ASSERT_DOUBLE_EQ(0, C.volume);

    for (double z0=0.1;z0<1;z0+=0.1)
    {
        dz = get_L_immersions(z0);
        intersector.update_intersection_with_free_surface(dz,dz);
        auto C = intersector.center_of_mass_immersed();
        EXPECT_DOUBLE_EQ(2*z0, C.volume) << "z0 = " << z0;
        // Coordinates are in the mesh frame
        EXPECT_DOUBLE_EQ(0.5,  (double)C.G(0)) << "z0 = " << z0;
        EXPECT_DOUBLE_EQ(1,    (double)C.G(1)) << "z0 = " << z0;
        EXPECT_DOUBLE_EQ(-z0/2, (double)C.G(2)) << "z0 = " << z0;
    }
}

TEST_F(MeshIntersectorTest, immersed_volume_of_unit_cube)
{
    MeshIntersector intersector(unit_cube());
    const std::vector<double> immersions = {0.4,0.3,0.2,0.1,-0.1,-0.2,-0.3,-0.4};
    for (const auto z0:immersions)
    {
        std::vector<double> dz = get_cube_immersions(z0);
        intersector.update_intersection_with_free_surface(dz,dz);
        ASSERT_DOUBLE_EQ(z0+0.5, intersector.immersed_volume()) << "z0 = " << z0;
    }
}

TEST_F(MeshIntersectorTest, can_compute_volume_of_fully_immersed_L)
{
    MeshIntersector intersector(L(),false);
    std::vector<double> dz = get_L_immersions(10);
    intersector.update_intersection_with_free_surface(dz,dz);
    ASSERT_DOUBLE_EQ(3, intersector.immersed_volume());
    ASSERT_DOUBLE_EQ(0, intersector.emerged_volume());
}

TEST_F(MeshIntersectorTest, immersed_volume_of_L)
{
    MeshIntersector intersector(L(),false);
    std::vector<double> immersions = {0.4,0.3,0.2,0.1};

    for (const auto z0:immersions)
    {
        std::vector<double> dz = get_L_immersions(z0);
        intersector.update_intersection_with_free_surface(dz,dz);
        ASSERT_DOUBLE_EQ(2*z0, intersector.immersed_volume()) << "z0 = " << z0;
    }

    immersions = {1.5,1.6,1.7,1.8,1.9,2};
    for (const auto z0:immersions)
    {
        std::vector<double> dz = get_L_immersions(z0);
        intersector.update_intersection_with_free_surface(dz,dz);
        ASSERT_DOUBLE_EQ(1+z0, intersector.immersed_volume()) << "z0 = " << z0;
    }
}

using namespace ssc::kinematics;

RotationMatrix rot(const double phi, const double theta, const double psi);
RotationMatrix rot(const double phi, const double theta, const double psi)
{
    return rotation_matrix<INTRINSIC, CHANGING_ANGLE_ORDER, 3, 2, 1>(ssc::kinematics::EulerAngles(phi,theta,psi));
}

TEST_F(MeshIntersectorTest, can_display_facet_in_NED_frame)
{
    MeshIntersector intersector(unit_cube());
    std::vector<double> dz = get_cube_immersions(10);
    intersector.update_intersection_with_free_surface(dz,dz);
    const double phi = PI/4;
    const double theta = 0;
    const double psi = 0;
    const std::string actual = intersector.display_facet_in_NED(*intersector.begin_immersed(),EPoint(0,0,0),rot(phi,theta,psi));
    std::string expected;
    std::stringstream ss;
    ss << 1e-20;
    if (ss.str() == "1e-020")
    {
        expected = "Facet:\n"
                   "Unit normal: 0 0 1\n"
                   "Area: 0.5\n"
                   "Center of gravity:  0.166667 -0.471405  0.235702\n"
                   "Vertex indices: [0, 1, 2]\n"
                   "Coordinates in NED frame (one column per point):\n"
                   "        -0.5          0.5          0.5\n"
                   "   -0.707107    -0.707107 5.55112e-017\n"
                   "5.55112e-017 5.55112e-017     0.707107";
    }
    else
    {
        expected = "Facet:\n"
                   "Unit normal: 0 0 1\n"
                   "Area: 0.5\n"
                   "Center of gravity:  0.166667 -0.471405  0.235702\n"
                   "Vertex indices: [0, 1, 2]\n"
                   "Coordinates in NED frame (one column per point):\n"
                   "       -0.5         0.5         0.5\n"
                   "  -0.707107   -0.707107 5.55112e-17\n"
                   "5.55112e-17 5.55112e-17    0.707107";
    }
    ASSERT_EQ(expected, actual);
}

TEST_F(MeshIntersectorTest, can_calculate_the_volume_of_fully_immersed_U)
{
    MeshIntersector intersector(U(),false);
    const std::vector<double> dz = get_U_immersions(10);
    intersector.update_intersection_with_free_surface(dz,dz);
    ASSERT_DOUBLE_EQ(5, intersector.immersed_volume());
}

TEST_F(MeshIntersectorTest, can_calculate_the_volume_of_a_unit_cube)
{
    std::vector<double> half_immersed(8);
    half_immersed[0] = +0.5;
    half_immersed[1] = +0.5;
    half_immersed[2] = +0.5;
    half_immersed[3] = +0.5;
    half_immersed[4] = -0.5;
    half_immersed[5] = -0.5;
    half_immersed[6] = -0.5;
    half_immersed[7] = -0.5;

    MeshIntersector intersector(read_stl(test_data::cube()));

    intersector.update_intersection_with_free_surface(half_immersed,half_immersed);
    ASSERT_SMALL_RELATIVE_ERROR(1, intersector.immersed_volume()+intersector.emerged_volume(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(0.5, intersector.emerged_volume(), EPS);
    ASSERT_SMALL_RELATIVE_ERROR(0.5, intersector.immersed_volume(), EPS);
}

TEST_F(MeshIntersectorTest, can_retrieve_edges_exactly_on_the_surface_for_U)
{
    MeshIntersector intersector(U(),false);
    const std::vector<double> dz = get_U_immersions(2);
    intersector.update_intersection_with_free_surface(dz,dz);
    ASSERT_EQ(10, intersector.index_of_edges_exactly_on_surface.size());
}

TEST_F(MeshIntersectorTest, can_retrieve_edges_exactly_on_the_surface_for_L)
{
    MeshIntersector intersector(L(),false);
    const std::vector<double> dz = get_L_immersions(2);
    intersector.update_intersection_with_free_surface(dz,dz);
    ASSERT_EQ(5, intersector.index_of_edges_exactly_on_surface.size());
}

TEST_F(MeshIntersectorTest, can_retrieve_edges_exactly_on_the_surface_for_cube_rotated_45_deg)
{
    MeshIntersector intersector(unit_cube());
    const std::vector<double> dz = {0,0,sqrt(2)/2,sqrt(2)/2,-sqrt(2)/2,-sqrt(2)/2,0,0};
    intersector.update_intersection_with_free_surface(dz,dz);
    // 5 because one diagonal edge is split by the free surface
    ASSERT_EQ(5, intersector.index_of_edges_exactly_on_surface.size());
}

TEST_F(MeshIntersectorTest, can_retrieve_edges_exactly_on_the_surface_for_partially_immersed_cube)
{
    MeshIntersector intersector(unit_cube());
    const std::vector<double> dz = get_cube_immersions(0.3);
    intersector.update_intersection_with_free_surface(dz,dz);
    ASSERT_EQ(8, intersector.index_of_edges_exactly_on_surface.size());
}

TEST_F(MeshIntersectorTest, area_of_immersed_facets_is_properly_computed)
{
    MeshIntersector intersector(unit_cube());
    std::vector<double> dz = get_cube_immersions(0.4);
    intersector.update_intersection_with_free_surface(dz,dz);
    std::vector<Facet> immersed_facets;
    for (auto it = intersector.begin_immersed() ; it != intersector.end_immersed() ; ++it)
    {
        immersed_facets.push_back(*it);
    }
    ASSERT_DOUBLE_EQ(0.5,   immersed_facets.at(0).area);
    ASSERT_DOUBLE_EQ(0.5,   immersed_facets.at(1).area);
    ASSERT_DOUBLE_EQ(0.495, immersed_facets.at(2).area);
    ASSERT_DOUBLE_EQ(0.405, immersed_facets.at(3).area);
    ASSERT_DOUBLE_EQ(0.405, immersed_facets.at(4).area);
    ASSERT_DOUBLE_EQ(0.495, immersed_facets.at(5).area);
    ASSERT_DOUBLE_EQ(0.405, immersed_facets.at(6).area);
    ASSERT_DOUBLE_EQ(0.495, immersed_facets.at(7).area);
    ASSERT_DOUBLE_EQ(0.495, immersed_facets.at(8).area);
    ASSERT_DOUBLE_EQ(0.405, immersed_facets.at(9).area);
}

TEST_F(MeshIntersectorTest, can_retrieve_facets_exactly_on_the_surface)
{
    MeshIntersector intersector(U(),false);
    std::vector<double> dz = get_U_immersions(2);
    intersector.update_intersection_with_free_surface(dz,dz);
    std::vector<Facet> facet_exactly_on_the_surface;
    for (auto it = intersector.begin_surface() ; it != intersector.end_surface() ; ++it)
    {
        facet_exactly_on_the_surface.push_back(*it);
    }
    ASSERT_EQ(2, facet_exactly_on_the_surface.size());
}

TEST_F(MeshIntersectorTest, center_of_gravity_of_immersed_facets_is_properly_computed)
{
    MeshIntersector intersector(unit_cube());
    std::vector<double> dz = get_cube_immersions(0.4);
    intersector.update_intersection_with_free_surface(dz,dz);
    std::vector<Facet> immersed_facets;
    for (auto it = intersector.begin_immersed() ; it != intersector.end_immersed() ; ++it)
    {
        immersed_facets.push_back(*it);
    }
    check_vector(immersed_facets.at(0).centre_of_gravity, 1./6., -1./6, 0.5);
    check_vector(immersed_facets.at(1).centre_of_gravity, -1./6., 1./6, 0.5);
    check_vector(immersed_facets.at(2).centre_of_gravity, -0.5, -162./990., 171./990.);
    check_vector(immersed_facets.at(3).centre_of_gravity, -0.5, 0.2, -0.1);
    check_vector(immersed_facets.at(4).centre_of_gravity, +0.5, -0.2, -0.1);
    check_vector(immersed_facets.at(5).centre_of_gravity, +0.5, 162/990., 171/990.);
    check_vector(immersed_facets.at(6).centre_of_gravity, 0.2, -0.5, -0.1);
    check_vector(immersed_facets.at(7).centre_of_gravity, -162/990., -0.5, 171/990.);
    check_vector(immersed_facets.at(8).centre_of_gravity, 162/990., +0.5, 171/990.);
    check_vector(immersed_facets.at(9).centre_of_gravity, -0.2, +0.5, -0.1);
}

TEST_F(MeshIntersectorTest, unit_normal_of_immersed_facets_is_properly_computed)
{
    MeshIntersector intersector(unit_cube());
    std::vector<double> dz = get_cube_immersions(0.4);
    intersector.update_intersection_with_free_surface(dz,dz);
    std::vector<Facet> immersed_facets;
    for (auto it = intersector.begin_immersed() ; it != intersector.end_immersed() ; ++it)
    {
        immersed_facets.push_back(*it);
    }
    ASSERT_EQ(10, immersed_facets.size());
    check_vector(immersed_facets.at(0).unit_normal, 0, 0, 1);
    check_vector(immersed_facets.at(1).unit_normal, 0, 0, 1);
    check_vector(immersed_facets.at(2).unit_normal, -1, 0, 0);
    check_vector(immersed_facets.at(3).unit_normal, -1, 0, 0);
    check_vector(immersed_facets.at(4).unit_normal, 1, 0, 0);
    check_vector(immersed_facets.at(5).unit_normal, 1, 0, 0);
    check_vector(immersed_facets.at(6).unit_normal, 0, -1, 0);
    check_vector(immersed_facets.at(7).unit_normal, 0, -1, 0);
    check_vector(immersed_facets.at(8).unit_normal, 0, 1, 0);
    check_vector(immersed_facets.at(9).unit_normal, 0, 1, 0);
}

TEST_F(MeshIntersectorTest, center_of_gravity_of_facets_on_surface_is_properly_computed)
{
    MeshIntersector intersector(U(),false);
    std::vector<double> dz = get_U_immersions(2);
    intersector.update_intersection_with_free_surface(dz,dz);
    std::vector<Facet> facets_on_surface;
    for (auto it = intersector.begin_surface() ; it != intersector.end_surface() ; ++it)
    {
        facets_on_surface.push_back(*it);
    }
    ASSERT_EQ(2, facets_on_surface.size());
    check_vector(facets_on_surface.at(0).centre_of_gravity, 0.5, 0.5, -2);
    check_vector(facets_on_surface.at(1).centre_of_gravity, 0.5, 2.5, -2);
}

TEST_F(MeshIntersectorTest, area_of_facets_on_surface_is_properly_computed)
{
    MeshIntersector intersector(U(),false);
    std::vector<double> dz = get_U_immersions(2);
    intersector.update_intersection_with_free_surface(dz,dz);
    std::vector<Facet> facets_on_surface;
    for (auto it = intersector.begin_surface() ; it != intersector.end_surface() ; ++it)
    {
        facets_on_surface.push_back(*it);
    }
    ASSERT_EQ(2, facets_on_surface.size());
    ASSERT_DOUBLE_EQ(1, facets_on_surface.at(0).area);
    ASSERT_DOUBLE_EQ(1, facets_on_surface.at(1).area);
}

TEST_F(MeshIntersectorTest, unit_normal_of_facets_on_surface_is_properly_computed)
{
    MeshIntersector intersector(U(),false);
    std::vector<double> dz = get_U_immersions(2);
    intersector.update_intersection_with_free_surface(dz,dz);
    std::vector<Facet> facets_on_surface;
    for (auto it = intersector.begin_surface() ; it != intersector.end_surface() ; ++it)
    {
        facets_on_surface.push_back(*it);
    }
    ASSERT_EQ(2, facets_on_surface.size());
    check_vector(facets_on_surface.at(0).unit_normal, 0, 0, -1);
    check_vector(facets_on_surface.at(1).unit_normal, 0, 0, -1);
}

TEST_F(MeshIntersectorTest, center_of_gravity_of_facets_on_surface_is_properly_computed_2)
{
    MeshIntersector intersector(U(),false);
    std::vector<double> dz = get_U_immersions(1.5);
    intersector.update_intersection_with_free_surface(dz,dz);
    std::vector<Facet> facets_on_surface;
    for (auto it = intersector.begin_surface() ; it != intersector.end_surface() ; ++it)
    {
        facets_on_surface.push_back(*it);
    }
    ASSERT_EQ(2, facets_on_surface.size());
    check_vector(facets_on_surface.at(0).centre_of_gravity, 0.5, 0.5, -1.5);
    check_vector(facets_on_surface.at(1).centre_of_gravity, 0.5, 2.5, -1.5);
}

TEST_F(MeshIntersectorTest, area_of_facets_on_surface_is_properly_computed_2)
{
    MeshIntersector intersector(U(),false);
    std::vector<double> dz = get_U_immersions(1.5);
    intersector.update_intersection_with_free_surface(dz,dz);
    std::vector<Facet> facets_on_surface;
    for (auto it = intersector.begin_surface() ; it != intersector.end_surface() ; ++it)
    {
        facets_on_surface.push_back(*it);
    }
    ASSERT_EQ(2, facets_on_surface.size());
    ASSERT_DOUBLE_EQ(1, facets_on_surface.at(0).area);
    ASSERT_DOUBLE_EQ(1, facets_on_surface.at(1).area);
}

TEST_F(MeshIntersectorTest, unit_normal_of_facets_on_surface_is_properly_computed_2)
{
    MeshIntersector intersector(U(),false);
    std::vector<double> dz = get_U_immersions(1.5);
    intersector.update_intersection_with_free_surface(dz,dz);
    std::vector<Facet> facets_on_surface;
    for (auto it = intersector.begin_surface() ; it != intersector.end_surface() ; ++it)
    {
        facets_on_surface.push_back(*it);
    }
    ASSERT_EQ(2, facets_on_surface.size());
    check_vector(facets_on_surface.at(0).unit_normal, 0, 0, -1);
    check_vector(facets_on_surface.at(1).unit_normal, 0, 0, -1);
}

TEST_F(MeshIntersectorTest, center_of_gravity_of_facets_on_surface_is_properly_computed_3)
{
    MeshIntersector intersector(L(),false);
    std::vector<double> dz = get_L_immersions(1.5);
    intersector.update_intersection_with_free_surface(dz,dz);
    std::vector<Facet> facets_on_surface;
    for (auto it = intersector.begin_surface() ; it != intersector.end_surface() ; ++it)
    {
        facets_on_surface.push_back(*it);
    }
    ASSERT_EQ(1, facets_on_surface.size());
    check_vector(facets_on_surface.at(0).centre_of_gravity, 0.5, 0.5, -1.5);
}

TEST_F(MeshIntersectorTest, area_of_facets_on_surface_is_properly_computed_3)
{
    MeshIntersector intersector(L(),false);
    std::vector<double> dz = get_L_immersions(1.5);
    intersector.update_intersection_with_free_surface(dz,dz);
    std::vector<Facet> facets_on_surface;
    for (auto it = intersector.begin_surface() ; it != intersector.end_surface() ; ++it)
    {
        facets_on_surface.push_back(*it);
    }
    ASSERT_EQ(1, facets_on_surface.size());
    ASSERT_DOUBLE_EQ(1, facets_on_surface.at(0).area);
}

TEST_F(MeshIntersectorTest, unit_normal_of_facets_on_surface_is_properly_computed_3)
{
    MeshIntersector intersector(L(),false);
    std::vector<double> dz = get_L_immersions(1.5);
    intersector.update_intersection_with_free_surface(dz,dz);
    std::vector<Facet> facets_on_surface;
    for (auto it = intersector.begin_surface() ; it != intersector.end_surface() ; ++it)
    {
        facets_on_surface.push_back(*it);
    }
    ASSERT_EQ(1, facets_on_surface.size());
    check_vector(facets_on_surface.at(0).unit_normal, 0, 0, -1);
}
