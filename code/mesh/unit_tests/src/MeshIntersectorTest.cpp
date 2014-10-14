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
#include "MeshIntersector.hpp"
#include "generate_anthineas.hpp"

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


TEST_F(MeshIntersectorTest, can_compute_the_volume_of_a_single_facet)
{
    MeshIntersector intersector(VectorOfVectorOfPoints(1, one_triangle()));
    const std::vector<double> v = {0,-0.25,0.5};
    intersector.update_intersection_with_free_surface(v);
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
        intersector.update_intersection_with_free_surface(v);
        ASSERT_NEAR(l*l*l/6./sqrt(2.), intersector.immersed_volume() + intersector.emerged_volume(), 1E-2);
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
        intersector.update_intersection_with_free_surface(dz);
        ASSERT_SMALL_RELATIVE_ERROR(l*l*l, intersector.immersed_volume() + intersector.emerged_volume(), EPS);
    }
}

TEST_F(MeshIntersectorTest, can_compute_the_volume_of_a_cube_with_lots_of_irregular_facets)
{
    MeshIntersector intersector(read_stl(test_data::big_cube()));
    const std::vector<double> dz(1,0);
    intersector.update_intersection_with_free_surface(dz);
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

        intersector.update_intersection_with_free_surface(all_immersed);
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

        intersector.update_intersection_with_free_surface(all_emerged);
        ASSERT_SMALL_RELATIVE_ERROR(l*l*l, intersector.emerged_volume(), EPS);
        ASSERT_DOUBLE_EQ(0, intersector.immersed_volume());
    }
}

TEST_F(MeshIntersectorTest, can_compute_closing_facet)
{
    const MeshPtr mesh(new Mesh(MeshBuilder(cube(1, 0, 0, 0, false)).build()));
    MeshIntersector intersector(mesh);
    std::vector<double> dz(8);
    dz[0] = -1;
    dz[1] = -1;
    dz[2] = -1;
    dz[3] = -1;
    dz[4] = 1;
    dz[5] = 1;
    dz[6] = 1;
    dz[7] = 1;
    intersector.update_intersection_with_free_surface(dz);
    const Facet f = intersector.compute_closing_facet();
    ASSERT_DOUBLE_EQ(1, f.area);
    ASSERT_DOUBLE_EQ( 0, (double)f.unit_normal(0));
    ASSERT_DOUBLE_EQ( 0, (double)f.unit_normal(1));
    ASSERT_DOUBLE_EQ(-1, (double)f.unit_normal(2));
    ASSERT_DOUBLE_EQ(0, f.barycenter.norm());
    ASSERT_DOUBLE_EQ(4, f.vertex_index.size());
    const auto P1 = mesh->all_nodes.col(f.vertex_index[0]);
    const auto P2 = mesh->all_nodes.col(f.vertex_index[1]);
    const auto P3 = mesh->all_nodes.col(f.vertex_index[2]);
    const auto P4 = mesh->all_nodes.col(f.vertex_index[3]);
    ASSERT_DOUBLE_EQ(-0.5, (double)P1(0));
    ASSERT_DOUBLE_EQ(0.5, (double)P1(1));
    ASSERT_DOUBLE_EQ(0, (double)P1(2));
    ASSERT_DOUBLE_EQ(0.5, (double)P2(0));
    ASSERT_DOUBLE_EQ(0.5, (double)P2(1));
    ASSERT_DOUBLE_EQ(0, (double)P2(2));
    ASSERT_DOUBLE_EQ(0.5, (double)P3(0));
    ASSERT_DOUBLE_EQ(-0.5, (double)P3(1));
    ASSERT_DOUBLE_EQ(0, (double)P3(2));
    ASSERT_DOUBLE_EQ(-0.5, (double)P4(0));
    ASSERT_DOUBLE_EQ(-0.5, (double)P4(1));
    ASSERT_DOUBLE_EQ(0, (double)P4(2));
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
    intersector.update_intersection_with_free_surface(dz);
    Facet f;
    f.vertex_index.push_back(0);
    f.vertex_index.push_back(1);
    f.vertex_index.push_back(2);
    f.vertex_index.push_back(3);
    ASSERT_TRUE(intersector.has(f));
    f.vertex_index.back() = 4;
    ASSERT_FALSE(intersector.has(f));
    const Facet zero = intersector.compute_closing_facet();
    ASSERT_TRUE(intersector.has(zero));
}

TEST_F(MeshIntersectorTest, can_compute_the_volume_of_a_partially_immersed_cube)
{
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double l = a.random<double>().between(0, 10000);
        const double immersed_ratio = a.random<double>().between(0,1);
        std::vector<double> half_immersed(8);
        const double z0 = l*(immersed_ratio-0.5);
        half_immersed[0] = z0+l/2;
        half_immersed[1] = z0+l/2;
        half_immersed[2] = z0+l/2;
        half_immersed[3] = z0+l/2;
        half_immersed[4] = z0-l/2;
        half_immersed[5] = z0-l/2;
        half_immersed[6] = z0-l/2;
        half_immersed[7] = z0-l/2;

        const double x0 = a.random<double>().between(-100,100);
        const double y0 = a.random<double>().between(-100,100);

        MeshIntersector intersector(cube(l, x0, y0, z0));

        intersector.update_intersection_with_free_surface(half_immersed);
        ASSERT_SMALL_RELATIVE_ERROR(l*l*l, intersector.immersed_volume()+intersector.emerged_volume(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(l*l*l*(1-immersed_ratio), intersector.emerged_volume(), EPS);
        ASSERT_SMALL_RELATIVE_ERROR(l*l*l*immersed_ratio, intersector.immersed_volume(), EPS);
    }
}

TEST_F(MeshIntersectorTest, can_compute_the_volume_of_the_anthineas)
{
    MeshIntersector intersector(anthineas());
    const std::vector<double> dz(intersector.mesh->all_nodes.cols(),1);
    intersector.update_intersection_with_free_surface(dz);
    ASSERT_NEAR(601.726, intersector.emerged_volume()+intersector.immersed_volume(), 1E-3);
}
