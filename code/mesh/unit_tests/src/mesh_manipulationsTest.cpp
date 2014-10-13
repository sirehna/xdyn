/*
 * mesh_manipulationsTest.cpp
 *
 *  Created on: May 20, 2014
 *      Author: cady
 */

#include "base91x.hpp"
#include "mesh_manipulationsTest.hpp"
#include "GeometricTypes3d.hpp"
#include "mesh_manipulations.hpp"
#include "StlReader.hpp"
#include "TriMeshTestData.hpp"
#include "STL_data.hpp"
#include "MeshException.hpp"
#include "MeshIntersector.hpp"
#include "MeshBuilder.hpp"
#include "generate_anthineas.hpp"

#include <ssc/macros.hpp>

mesh_manipulationsTest::mesh_manipulationsTest() : a(ssc::random_data_generator::DataGenerator(321))
{
}

mesh_manipulationsTest::~mesh_manipulationsTest()
{
}

void mesh_manipulationsTest::SetUp()
{
}

void mesh_manipulationsTest::TearDown()
{
}

TEST_F(mesh_manipulationsTest, unit_normal)
{
//! [mesh_manipulationsTest unit_normal_example]
    const Matrix3x M = Eigen::MatrixXd::Identity(3,3);
    const Eigen::Vector3d n = unit_normal(M);
    ASSERT_DOUBLE_EQ(1, n.norm());
    ASSERT_DOUBLE_EQ(1/sqrt(3.), (double)n(0));
    ASSERT_DOUBLE_EQ(1/sqrt(3.), (double)n(1));
    ASSERT_DOUBLE_EQ(1/sqrt(3.), (double)n(2));
//! [mesh_manipulationsTest unit_normal_example]
}

TEST_F(mesh_manipulationsTest, barycenter)
{
//! [mesh_manipulationsTest barycentre_example]
    const Matrix3x M = Eigen::MatrixXd::Identity(3,3);
    const Eigen::Vector3d G = barycenter(M);
    ASSERT_DOUBLE_EQ(1/3., (double)G(0));
    ASSERT_DOUBLE_EQ(1/3., (double)G(1));
    ASSERT_DOUBLE_EQ(1/3., (double)G(2));
//! [mesh_manipulationsTest barycentre_example]
}

TEST_F(mesh_manipulationsTest, area)
{
//! [mesh_manipulationsTest area_example]
    Matrix3x M(3,4);
    M(0,0) = 0; M(0,1) = 1; M(0,2) = 1; M(0,3) = 0;
    M(1,0) = 0; M(1,1) = 0; M(1,2) = 1; M(1,3) = 1;
    M(2,0) = 0; M(2,1) = 0; M(2,2) = 0; M(2,3) = 0;
    ASSERT_DOUBLE_EQ(1, area(M));
//! [mesh_manipulationsTest area_example]
}

TEST_F(mesh_manipulationsTest, centre_of_gravity)
{
//! [mesh_manipulationsTest centre_of_gravity_example]
    Matrix3x M(3,4);
    M(0,0) = 0; M(0,1) = 4; M(0,2) = 3; M(0,3) = 1;
    M(1,0) = 0; M(1,1) = 0; M(1,2) = 1; M(1,3) = 1;
    M(2,0) = 0; M(2,1) = 0; M(2,2) = 0; M(2,3) = 0;
    const Eigen::Vector3d G = centre_of_gravity(M);
    ASSERT_DOUBLE_EQ(2,(double)G(0));
    ASSERT_DOUBLE_EQ(4./9.,(double)G(1));
    ASSERT_DOUBLE_EQ(0,(double)G(2));
//! [mesh_manipulationsTest centre_of_gravity_example]
}

TEST_F(mesh_manipulationsTest, centre_of_gravity_2)
{
    Matrix3x M(3,4);
    M(0,0) = 0; M(0,1) = 1; M(0,2) = 1; M(0,3) = 0;
    M(1,0) = 0; M(1,1) = 0; M(1,2) = 1; M(1,3) = 1;
    M(2,0) = 0; M(2,1) = 0; M(2,2) = 0; M(2,3) = 0;
    const Eigen::Vector3d G = centre_of_gravity(M);
    ASSERT_DOUBLE_EQ(0.5,(double)G(0));
    ASSERT_DOUBLE_EQ(0.5,(double)G(1));
    ASSERT_DOUBLE_EQ(0,(double)G(2));
}

TEST_F(mesh_manipulationsTest, can_read_and_write_binary_stl)
{
    const VectorOfVectorOfPoints mesh = unit_cube();
    std::stringstream ss;

    write_binary_stl(mesh, ss);
    const VectorOfVectorOfPoints parsed_mesh = read_binary_stl(ss);

    ASSERT_EQ(mesh.size(), parsed_mesh.size());
    for (size_t i = 0 ; i < mesh.size() ; ++i)
    {
        ASSERT_EQ(mesh[i].size(), parsed_mesh[i].size()) << " (for i = " << i << ")";
        for (size_t j = 0 ; j < mesh[i].size() ; ++j)
        {
            ASSERT_DOUBLE_EQ((double)mesh[i][j](0), (double)parsed_mesh[i][j](0)) << " (for i = " << i << " and j = " << j << ", x-coordinate)";
            ASSERT_DOUBLE_EQ((double)mesh[i][j](1), (double)parsed_mesh[i][j](1)) << " (for i = " << i << " and j = " << j << ", y-coordinate)";
            ASSERT_DOUBLE_EQ((double)mesh[i][j](2), (double)parsed_mesh[i][j](2)) << " (for i = " << i << " and j = " << j << ", z-coordinate)";
        }
    }
}

TEST_F(mesh_manipulationsTest, can_read_binary_stl_from_string)
{
    const VectorOfVectorOfPoints from_ascii = read_stl(test_data::cube());
    const std::string s = ";C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$A;C2W$AMAAAAAAAAAAAAAAA4}BAC'EBAA1hAAT|DAAA#AAA1hAAT|DAAA#AAAHLAAT|DAAAAAAAAAAAAAk_HAAASEAAcsBA+\\PAAA+CAA4}AAv(fAAA.QAAT|BABt#AAAAAAAAAAAAAC'JCAAOWAA+\\XAAAAAAAP(AA+\\XAAAAAAAP(AA+\\HAAAAAAAAAAAAAAAAAAATEAAcsAAv(vAAAAAAAcsBAv(PAAAAAAAcsAAv(PAAAAAAAAA4}FAAAAAAAAAAA4}CABtEBAAAAAAT|FAC'EBAAPLAAk_LAAA#AAAPLAAAA+\\vAAAAAAAAAAA+\\XAAASEAAAAAA+\\XAAAjBAA8sAAv(vAAA+CAAAAAAAABt#AAAAAAAAAAABtfAAA.QAA4}BAC'fAAA.QAAAAAABtfAAAxFAAAAAAAAC'#AAAuWAAAAAAC'fAAA.QAA4}BAC'fAAAxFAAAAAABtfAAAxFAA4}BAAAAAAAAAf(AAAAAAAAJCAAP(AAAAAAAA#AAA1hAAAAAAC'fAAA.QAA4}BAAAAAAAAAf(AAAAAAAAJCAAP(AAAAAAAA#AAA1hAAT|DAAAJCAAP(AAk_HAAAAAXLAA4}BAAAAAAA.QAAT|BABt#AAAHLAAk_DAC'#AAAHLAAk_DAAAAAAAAAAAAAk_HAAAAAAAP(AA+\\HAAAkBAAcsAAv(PAAAAAAAcsBAv(PAAAAAAA";
    const VectorOfVectorOfPoints from_binary = read_binary_stl(base<91>::decode(s));

    ASSERT_EQ(from_ascii.size(),from_binary.size());
    for (size_t i = 0 ; i < from_ascii.size() ; ++i)
    {
        ASSERT_EQ(from_ascii[i].size(),from_binary[i].size());
        for (size_t j = 0 ; j < from_ascii[i].size() ; ++j)
        {
            ASSERT_DOUBLE_EQ((double)from_ascii[i][j](0),(double)from_binary[i][j](0));
            ASSERT_DOUBLE_EQ((double)from_ascii[i][j](1),(double)from_binary[i][j](1));
            ASSERT_DOUBLE_EQ((double)from_ascii[i][j](2),(double)from_binary[i][j](2));
        }
    }
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_single_point)
{
    const VectorOfPoints single_point(1,a.random<EPoint>());
    // Doesn't much matter because there is no surface
    ASSERT_TRUE(oriented_clockwise(single_point, a.random<EPoint>()));
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_segment)
{
    VectorOfPoints segment;
    segment.push_back(a.random<EPoint>());
    segment.push_back(a.random<EPoint>());
    // Doesn't much matter because there is no surface
    ASSERT_TRUE(oriented_clockwise(segment, a.random<EPoint>()));
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_degenerated_triangle)
{
    // Doesn't much matter because there is no surface (so we don't know what "inside" means)
    ASSERT_TRUE(oriented_clockwise(degenerated_triangle(), a.random<EPoint>()));
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_triangle)
{
    ASSERT_TRUE(oriented_clockwise(one_triangle(), EPoint(1,2,3)));
}

TEST_F(mesh_manipulationsTest, throws_if_not_all_facets_have_the_same_orientation)
{
    ASSERT_THROW(oriented_clockwise(unit_cube_with_incorrect_orientation(), a.random<EPoint>()), MeshException);
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_tetrahedron)
{
    ASSERT_FALSE(oriented_clockwise(tetrahedron(1,4,5,6), EPoint(4,5,6+sqrt(6.)/4.)));
    ASSERT_TRUE(oriented_clockwise(tetrahedron_clockwise(1,2,3,4), EPoint(2,3,4+sqrt(6.)/4.)));
}

TEST_F(mesh_manipulationsTest, can_deduce_the_orientation_of_the_normals_on_a_cube)
{
    ASSERT_FALSE(oriented_clockwise(unit_cube(), EPoint(0,0,0)));
    ASSERT_TRUE(oriented_clockwise(unit_cube_clockwise(), EPoint(0,0,0)));
}

TEST_F(mesh_manipulationsTest, can_compute_the_volume_of_a_single_facet)
{
    ASSERT_DOUBLE_EQ(0, volume(VectorOfVectorOfPoints(1, one_triangle())));
    ASSERT_DOUBLE_EQ(0, volume(VectorOfVectorOfPoints(1, degenerated_triangle())));
}

TEST_F(mesh_manipulationsTest, DISABLED_can_compute_the_volume_of_a_tetrahedron)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const double l = a.random<double>().between(0,1000);
        ASSERT_DOUBLE_EQ(l*l*l/6./sqrt(2.), volume(tetrahedron(l,a.random<double>(),a.random<double>(),a.random<double>())));
    }
}

TEST_F(mesh_manipulationsTest, DISABLED_can_compute_the_volume_of_a_cube)
{
    ASSERT_DOUBLE_EQ(1, volume(unit_cube_clockwise()));
    ASSERT_DOUBLE_EQ(1, volume(unit_cube()));
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const double l = a.random<double>().between(0, 10000);
        ASSERT_DOUBLE_EQ(l*l*l, volume(cube(l, a.random<double>(), a.random<double>(), a.random<double>())));
    }
}

TEST_F(mesh_manipulationsTest, DISABLED_can_compute_the_volume_of_a_cube_with_lots_of_irregular_facets)
{
    ASSERT_DOUBLE_EQ(1, volume(read_stl(test_data::big_cube())));
}

TEST_F(mesh_manipulationsTest, DISABLED_can_compute_the_volume_of_an_immersed_cube)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
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
        MeshBuilder builder(cube(l, a.random<double>(), a.random<double>(), a.random<double>()));

        MeshPtr mesh(new Mesh(builder.build()));
        MeshIntersector intersector(mesh);

        intersector.update_intersection_with_free_surface(all_immersed);
        ASSERT_DOUBLE_EQ(0, volume(intersector.begin_emerged(), intersector.end_emerged()));
        ASSERT_DOUBLE_EQ(l*l*l, volume(intersector.begin_immersed(), intersector.end_immersed()));
    }
}

TEST_F(mesh_manipulationsTest, DISABLED_can_compute_the_volume_of_an_emerged_cube)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const double l = a.random<double>().between(0, 10000);
        std::vector<double> all_emerged(8);
        all_emerged[0] = 1;
        all_emerged[1] = 1;
        all_emerged[2] = 1;
        all_emerged[3] = 1;
        all_emerged[4] = 1+l;
        all_emerged[5] = 1+l;
        all_emerged[6] = 1+l;
        all_emerged[7] = 1+l;
        MeshBuilder builder(cube(l, a.random<double>(), a.random<double>(), a.random<double>()));

        MeshPtr mesh(new Mesh(builder.build()));
        MeshIntersector intersector(mesh);

        intersector.update_intersection_with_free_surface(all_emerged);
        ASSERT_DOUBLE_EQ(l*l*l, volume(intersector.begin_emerged(), intersector.end_emerged()));
        ASSERT_DOUBLE_EQ(0, volume(intersector.begin_immersed(), intersector.end_immersed()));
    }
}

TEST_F(mesh_manipulationsTest, DISABLED_can_compute_the_volume_of_a_partially_immersed_cube)
{
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const double l = a.random<double>().between(0, 10000);
        const double immersed_ratio = a.random<double>().between(0,1);
        std::vector<double> half_immersed(8);
        half_immersed[0] = -l*immersed_ratio;
        half_immersed[1] = -l*immersed_ratio;
        half_immersed[2] = -l*immersed_ratio;
        half_immersed[3] = -l*immersed_ratio;
        half_immersed[4] = l*(1-immersed_ratio);
        half_immersed[5] = l*(1-immersed_ratio);
        half_immersed[6] = l*(1-immersed_ratio);
        half_immersed[7] = l*(1-immersed_ratio);
        MeshBuilder builder(cube(l, a.random<double>(), a.random<double>(), a.random<double>()));

        MeshPtr mesh(new Mesh(builder.build()));
        MeshIntersector intersector(mesh);

        intersector.update_intersection_with_free_surface(half_immersed);
        ASSERT_DOUBLE_EQ(l*l*l*(1-immersed_ratio), volume(intersector.begin_emerged(), intersector.end_emerged()));
        ASSERT_DOUBLE_EQ(l*l*l*immersed_ratio, volume(intersector.begin_immersed(), intersector.end_immersed()));
    }
}

TEST_F(mesh_manipulationsTest, DISABLED_can_compute_the_volume_of_the_anthineas)
{
    ASSERT_NEAR(601.726, volume(anthineas()), 1E-3);
}
