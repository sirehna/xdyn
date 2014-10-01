/*
 * MeshIntersectorTest.cpp
 */

#include "TriMeshTestData.hpp"
#include "MeshBuilder.hpp"
#include "MeshIntersectorTest.hpp"
#include "mesh_manipulations.hpp"
#include <ssc/macros.hpp>
#include "STL_data.hpp"
#include "StlReader.hpp"

#define EPS 1E-6

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

TEST_F(MeshIntersectorTest, can_compute_immerged_polygon_for_one_immerged_node)
{
//! [MeshIntersectorTest immerged_polygon_example_1]
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1; M(0,1) = 4; M(0,2) = 7;
    M(1,0) = 1; M(1,1) = 6; M(1,2) = 8;
    M(2,0) = 3; M(2,1) = 6; M(2,2) = 9;

    const std::vector<double> v({-1,-1,1});

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v);

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


TEST_F(MeshIntersectorTest, can_compute_immerged_polygon_for_two_immerged_node)
{
    Eigen::Matrix<double,3,3> M;
    M(0,0) = 1; M(0,1) = 4; M(0,2) = 7;
    M(1,0) = 1; M(1,1) = 6; M(1,2) = 8;
    M(2,0) = 3; M(2,1) = 6; M(2,2) = 9;

    const std::vector<double> v({-1,1,1});

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v);

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

TEST_F(MeshIntersectorTest, can_compute_immerged_polygon_when_emerged_nodes_are_the_first_nodes)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    const std::vector<double> v({-1,-1,-1,1,1});

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v);

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

TEST_F(MeshIntersectorTest, can_compute_immerged_polygon_when_emerged_nodes_are_the_last_nodes)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    const std::vector<double> v({1,1,-1,-1,-1});

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v);

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

TEST_F(MeshIntersectorTest, can_compute_immerged_polygon_when_emerged_nodes_are_in_the_middle)
{
    Eigen::Matrix<double,3,5> M;
    M(0,0) = 1; M(0,1) = 2; M(0,2) = 4; M(0,3) = 5; M(0,4) = 9;
    M(1,0) = 1; M(1,1) = 3; M(1,2) = 4; M(1,3) = 6; M(1,4) = 9;
    M(2,0) = 2; M(2,1) = 3; M(2,2) = 5; M(2,3) = 6; M(2,4) = 11;

    const std::vector<double> v({1,-1,-1,-1,1});

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v);

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

TEST_F(MeshIntersectorTest, immerged_polygon_should_throw_if_all_points_are_immerged)
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
    intersector.update_intersection_with_free_surface(v);

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

TEST_F(MeshIntersectorTest, immerged_polygon_should_throw_if_no_points_are_immerged)
{
    Eigen::Matrix<double,3,3> M;
    M <<
        1 , 4 , 7 ,
        2 , 5 , 8 ,
        3 , 6 , 9 ;
    const std::vector<double> v({-1,-2,-5});

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v);

    ASSERT_EQ(0,intersector.index_of_immersed_facets.size());
    ASSERT_EQ(0,intersector.index_of_emerged_facets.at(0));
}

TEST_F(MeshIntersectorTest, another_bug_in_immerged_polygon)
{
    MeshPtr mesh(new Mesh(MeshBuilder(two_triangles()).build()));
    const std::vector<double> v = {-1,-1,-2,1};

    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v);

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

TEST_F(MeshIntersectorTest, correct_immerged_polygon_when_two_points_are_exactly_on_the_surface)
{
    Eigen::Matrix<double,3,4> M;
    M <<  0, -1, 0, 1,
          0,  0, 0, 0,
         -1,  0, 1, 0;
    const std::vector<double> v = {-1,0,1,0};

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v);
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

TEST_F(MeshIntersectorTest, bug2_in_immerged_polygon)
{
    Eigen::Matrix<double,3,4> M;
    M <<  -1, 0, 1,  0,
           0, 0, 0,  0,
           0, 1, 0, -1;
    const std::vector<double> v = {0,1,0,-1};

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v);
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

TEST_F(MeshIntersectorTest, bug3_in_immerged_polygon)
{
    Eigen::Matrix<double,3,3> M;
    M <<  0.5,   0.5,   0.5,
          0.375, 0.5, 0.375,
         -0.25, -0.5,  0.25;
    const std::vector<double> v = {0,-0.25,0.5};

    MeshPtr mesh(new Mesh(MeshBuilder(M).build()));
    MeshIntersector intersector(mesh);
    intersector.update_intersection_with_free_surface(v);
    const auto p = intersector.coordinates_of_facet(intersector.index_of_immersed_facets.at(0));

    ASSERT_LT(0, (p.col(0)-p.col(1)).norm());
    ASSERT_LT(0, (p.col(0)-p.col(2)).norm());
    ASSERT_LT(0, (p.col(1)-p.col(2)).norm());
}
